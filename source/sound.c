#include "sound.h"
#include "memory.h"
#include <dolphin/ai.h>
#include <dolphin/ar.h>
#include <dolphin/arq.h>

typedef struct somestruct_0xEC {
	u8 field_0x0[0x10 - 0x0]; //0x0
} somestruct_0xEC;

typedef struct somestruct_0xF0 {
	SND_SEQID seqId; //0x0
	u32 volume; //0x4, casts to u8
	void* field_0x8; //0x8
	u8 field_0xC[4]; //0xC
} somestruct_0xF0;

typedef struct somestruct_0xF4 {
	u16 field_0x0; //0x0
	u16 field_0x2; //0x2
	u16 field_0x4; //0x4
	u16 field_0x6; //0x6
	u8 field_0x8[0xC - 0x8]; //0x8
	s32 field_0xC; //0xC
	u8 field_0x10[0x88 - 0x10]; //0x10
} somestruct_0xF4;

typedef struct SoundData {
	u8 field_0x0[0xE0 - 0x0]; //0x0
	u32 field_0xE0; //0xE0
	u32 field_0xE4; //0xE4
	u32 field_0xE8; //0xE8
	somestruct_0xEC* field_0xEC; //0xEC
	somestruct_0xF0* field_0xF0; //0xF0
	somestruct_0xF4* field_0xF4; //0xF4
	u8 field_0xF8[0x10C - 0xF8]; //0xF8
	SND_AUX_REVERBHI* reverb_hi; //0x10C
	u8 field_0x110[0x208 - 0x110]; //0x110
	u16 field_0x208; //0x208
	u16 field_0x20A; //0x20A
	u8 field_0x20C[0x214 - 0x20C]; //0x20C
	u16 field_0x214; //0x214
	u8 pad_0x216[2]; //0x216
	SND_OUTPUTMODE outputMode; //0x218
	u8 field_0x21C[0x224 - 0x21C]; //0x21C
	u16 field_0x224; //0x224
	u8 pad_0x226[2]; //0x226
	u8 field_0x228[0x22A - 0x228]; //0x228
	char filepath[0x100]; //0x22A
	u8 pad_0x32A[2]; //0x32A
	void* filedata; //0x32C
	u16 field_0x330; //0x330
	u8 pad_0x332[2]; //0x332
	u8 field_0x334[0x5BC - 0x334]; //0x334
	AIDCallback dma_callback; //0x5BC
	u8 field_0x5C0[0x15C0 - 0x5C0]; //0x5C0
	u32 field_0x15C0; //0x15C0
} SoundData;

//.sbss
SoundData sound;
SND_AUX_DELAY dly;
SND_AUX_CHORUS cho;
SND_AUX_REVERBSTD revS;
SND_AUX_REVERBHI revH;
u32 aramMemArray[8];

//local prototypes
void* sndMalloc(u32 size);
void sndFree(void* ptr);



void SoundAIDMACallback(void);

void* sndMalloc(u32 size) {
	return __memAlloc(HEAP_DEFAULT, size);
}

void sndFree(void* ptr) {
	__memFree(HEAP_DEFAULT, ptr);
}

void SoundInit(void) {
	SND_HOOKS hooks;
	int i;

	hooks.malloc = sndMalloc;
	hooks.free = sndFree;

	ARInit(aramMemArray, 8);
	ARQInit();
	AIInit(NULL);
	ARAlloc(0xBFC000); //12MB-16KB (reserved)
	sndSetHooks(&hooks);
	sndInit(0x40, 0xC, 0x2C, 1, 1, 0xBFC000);

	revH.tempDisableFX = FALSE;
	revH.time = 3.0f;
	revH.preDelay = 0.1f;
	revH.damping = 0.3f;
	revH.coloration = 0.0f;
	revH.crosstalk = 0.0f;
	revH.mix = 0.8f;
	sndAuxCallbackPrepareReverbHI(&revH);

	revS.tempDisableFX = FALSE;
	revS.time = 2.0f;
	revS.preDelay = 0.1f;
	revS.damping = 0.5f;
	revS.coloration = 0.5f;
	revS.mix = 0.5f;
	sndAuxCallbackPrepareReverbSTD(&revS);

	cho.baseDelay = 10;
	cho.variation = 0;
	cho.period = 500;
	sndAuxCallbackPrepareChorus(&cho);

	dly.delay[0] = 60;
	dly.delay[1] = 90;
	dly.delay[2] = 70;
	dly.feedback[0] = 20;
	dly.feedback[1] = 20;
	dly.feedback[2] = 20;
	dly.output[0] = 50;
	dly.output[1] = 50;
	dly.output[2] = 50;
	sndAuxCallbackPrepareDelay(&dly);
	sndSetAuxProcessingCallbacks(0, sndAuxCallbackReverbHI, &revH, 0xFF, 0, sndAuxCallbackChorus, &cho, 0xFF, 0);
	sndVolume(0x7F, 0, 0xFF);
	if (sound.outputMode != SND_OUTPUTMODE_STEREO) {
		sound.outputMode = SND_OUTPUTMODE_STEREO;
		sndOutputMode(SND_OUTPUTMODE_STEREO);
	}
	sound.dma_callback = AIRegisterDMACallback(SoundAIDMACallback);
	sound.field_0x330 = 0;
	sound.field_0x15C0 = 0;
	sound.field_0x208 = 10;
	sound.field_0x20A = 500;
	sound.field_0xE0 = 0;
	sound.field_0xE4 = 0;
	sound.field_0x214 = 0;
	sound.field_0x224 = 0;
	sound.field_0xE8 = 0;

	sound.filedata = __memAlloc(HEAP_DEFAULT, 0x20000);
	if (!sound.filedata) {
		while (1) ; //infinite loop
	}

	sound.field_0xEC = NULL;

	sound.field_0xF0 = __memAlloc(HEAP_DEFAULT, sizeof(somestruct_0xF0) * 4);
	if (!sound.field_0xF0) {
		while (1) ; //infinite loop
	}
	sound.field_0xF0[0].seqId = (SND_SEQID)-1;
	sound.field_0xF0[0].field_0x8 = 0;
	sound.field_0xF0[1].seqId = (SND_SEQID)-1;
	sound.field_0xF0[1].field_0x8 = 0;
	sound.field_0xF0[2].seqId = (SND_SEQID)-1;
	sound.field_0xF0[2].field_0x8 = 0;
	sound.field_0xF0[3].seqId = (SND_SEQID)-1;
	sound.field_0xF0[3].field_0x8 = 0;

	sound.field_0xF4 = __memAlloc(HEAP_DEFAULT, sizeof(somestruct_0xF4) * 40);
	if (!sound.field_0xF4) {
		while (1) {} //infinite loop
	}
	for (i = 0; i < 40; i++) {
		sound.field_0xF4[i].field_0x0 = 0;
		sound.field_0xF4[i].field_0xC = -1;
		sound.field_0xF4[i].field_0x2 = 0;
		sound.field_0xF4[i].field_0x4 = 127;
		sound.field_0xF4[i].field_0x6 = 64;
	}




}

void SoundMain(void) {

}

BOOL SoundLoadDVD2(const char* path) {
	return FALSE;
}











void SoundSetOutputMode(SND_OUTPUTMODE mode) {
	if (sound.outputMode != mode) {
		sound.outputMode = mode;
		sndOutputMode(mode);
	}
}

void SoundOpenCover(void) {
	if (!sound.field_0x214) {
		sound.field_0x214 = 1;
	}
}

void SoundCloseCover(void) {
	if (sndIsInstalled()) {

	}
}










void SoundAIDMACallback(void) {

}