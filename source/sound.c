#include "sound.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include "system.h"
#include <string.h>

extern int sprintf(char* str, const char* format, ...);

//.bss
static char str[0x100];
SoundData sound;
SND_AUX_DELAY dly;
SND_AUX_CHORUS cho;
SND_AUX_REVERBSTD revS;
SND_AUX_REVERBHI revH;

//.sbss
u32 aramMemArray[8];

//local prototypes
void* sndMalloc(u32 size);
void sndFree(void* ptr);
void* loadDVD_callback(u32 offset, u32 length);



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
	sound.fadeInTime = 10;
	sound.fadeOutTime = 500;
	sound.ptr_index = 0;
	sound.field_0xE4 = 0;
	sound.field_0x214 = 0;
	sound.field_0x224 = 0;
	sound.field_0xE8 = 0;

	sound.filedata = __memAlloc(HEAP_DEFAULT, 0x20000);
	if (!sound.filedata) {
		while (1) { ; } //infinite loop
	}

	sound.groups = NULL;

	sound.songs = __memAlloc(HEAP_DEFAULT, sizeof(SoundSong) * 4);
	if (!sound.songs) {
		while (1) { ; } //infinite loop
	}
	sound.songs[0].seqId = (SND_SEQID)-1;
	sound.songs[0].field_0x8 = 0;
	sound.songs[1].seqId = (SND_SEQID)-1;
	sound.songs[1].field_0x8 = 0;
	sound.songs[2].seqId = (SND_SEQID)-1;
	sound.songs[2].field_0x8 = 0;
	sound.songs[3].seqId = (SND_SEQID)-1;
	sound.songs[3].field_0x8 = 0;

	sound.effects = __memAlloc(HEAP_DEFAULT, sizeof(SoundEffect) * 40);
	if (!sound.effects) {
		while (1) { ; } //infinite loop
	}
	for (i = 0; i < 40; i++) {
		sound.effects[i].flags = 0;
		sound.effects[i].voiceId = -1;
		sound.effects[i].field_0x2 = 0;
		sound.effects[i].field_0x4 = 127;
		sound.effects[i].field_0x6 = 64;
	}

	sound.dvds = __memAlloc(HEAP_DEFAULT, sizeof(SoundDVD) * 2);
	if (!sound.dvds) {
		while (1) { ; } //infinite loop
	}
	sound.dvds[0].flags = 0;
	sound.dvds[1].flags = 0;



}

void SoundMain(void) {

}

void SoundMainInt(void) {

}

void SoundSetFadeTime(u16 in, u16 out) { //1:1
	sound.fadeInTime = in;
	sound.fadeOutTime = out;
}

void* loadDVD_callback(u32 offset, u32 length) { //1:1
	DVDFileInfo info;

	if (!DVDOpen(sound.filepath, &info)) {
		return NULL;
	}
	if (DVDRead(&info, sound.filedata, (s32)length, (s32)offset) <= 0) {
		return NULL;
	}
	DVDClose(&info);
	return sound.filedata;
}

BOOL SoundLoadDVD2(const char* path) { //almost 1:1, needs register changes
	DVDEntry* entry;
	void* ptr;
	u32 size;

	if (sound.ptr_index >= 8) {
		return FALSE;
	}

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".pool");
	entry = DVDMgrOpen(str, 2, 0);
	if (!entry) {
		ptr = NULL;
	}
	else {
		size = DVDMgrGetLength(entry);
		if (!size) {
			ptr = NULL;
		}
		else {
			size = OSRoundUp32B(size);
			ptr = __memAlloc(HEAP_DEFAULT, size);
			if (!ptr) {
				ptr = NULL;
			}
			else {
				if ((s32)DVDMgrRead(entry, ptr, size, 0) <= 0) {
					__memFree(HEAP_DEFAULT, ptr);
					ptr = NULL;
				}
				else {
					DVDMgrClose(entry);
				}
			}
		}
	}
	if (!ptr) {
		return FALSE;
	}
	sound.pool_ptrs[sound.ptr_index] = ptr;

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".proj");
	entry = DVDMgrOpen(str, 2, 0);
	if (!entry) {
		ptr = NULL;
	}
	else {
		size = DVDMgrGetLength(entry);
		if (!size) {
			ptr = NULL;
		}
		else {
			size = OSRoundUp32B(size);
			ptr = __memAlloc(HEAP_DEFAULT, size);
			if (!ptr) {
				ptr = NULL;
			}
			else {
				if ((s32)DVDMgrRead(entry, ptr, size, 0) <= 0) {
					__memFree(HEAP_DEFAULT, ptr);
					ptr = NULL;
				}
				else {
					DVDMgrClose(entry);
				}
			}
		}
	}
	if (!ptr) {
		return FALSE;
	}
	sound.proj_ptrs[sound.ptr_index] = ptr;

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".sdir");
	entry = DVDMgrOpen(str, 2, 0);
	if (!entry) {
		ptr = NULL;
	}
	else {
		size = DVDMgrGetLength(entry);
		if (!size) {
			ptr = NULL;
		}
		else {
			size = OSRoundUp32B(size);
			ptr = __memAlloc(HEAP_DEFAULT, size);
			if (!ptr) {
				ptr = NULL;
			}
			else {
				if ((s32)DVDMgrRead(entry, ptr, size, 0) <= 0) {
					__memFree(HEAP_DEFAULT, ptr);
					ptr = NULL;
				}
				else {
					DVDMgrClose(entry);
				}
			}
		}
	}
	if (!ptr) {
		return FALSE;
	}
	sound.sdir_ptrs[sound.ptr_index] = ptr;

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".samp");
	strcpy(sound.filepath, str);
	sndSetSampleDataUploadCallback(loadDVD_callback, 0x20000);
	return TRUE;
}

BOOL SoundLoadDVD2PushGroup(u8* group_ids) { //1:1
	u32 i;

	if (sound.ptr_index >= 8) {
		return FALSE;
	}
	sound.unk4_ptrs[sound.ptr_index] = 0;
	if (group_ids) {
		while (*group_ids != 0xFF) {
			sndPushGroup(
				sound.proj_ptrs[sound.ptr_index],
				*group_ids,
				NULL,
				sound.sdir_ptrs[sound.ptr_index],
				sound.pool_ptrs[sound.ptr_index]);
			group_ids++;
			sound.unk4_ptrs[sound.ptr_index]++;
		}
	}
	else {
		i = sound.field_0xE8;
		while (i != 0xFF) {
			sndPushGroup(
				sound.proj_ptrs[sound.ptr_index],
				(SND_GROUPID)i,
				NULL,
				sound.sdir_ptrs[sound.ptr_index],
				sound.pool_ptrs[sound.ptr_index]);
			i++;
			sound.unk4_ptrs[sound.ptr_index]++;
			sound.field_0xE8++;
		}
	}
	return TRUE;
}

void SoundLoadDVD2Free(void) {
	if (sound.filedata) {
		__memFree(HEAP_DEFAULT, sound.filedata);
		sound.filedata = NULL;
	}
}

BOOL SoundSLibLoadDVD(const char* path) {
	DVDEntry* entry;
	void* ptr;
	u32 size;
	u32* r27;

	if (sound.field_0xE4 >= 8) {
		return FALSE;
	}

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".slib");
	entry = DVDMgrOpen(str, 2, 0);
	if (!entry) {
		ptr = NULL;
	}
	else {
		size = DVDMgrGetLength(entry);
		if (!size) {
			ptr = NULL;
		}
		else {
			size = OSRoundUp32B(size);
			ptr = __memAlloc(HEAP_DEFAULT, size);
			if (!ptr) {
				ptr = NULL;
			}
			else {
				if ((s32)DVDMgrRead(entry, ptr, size, 0) <= 0) {
					__memFree(HEAP_DEFAULT, ptr);
					ptr = NULL;
				}
				else {
					DVDMgrClose(entry);
				}
			}
		}
	}
	r27 = ptr;
	if (!ptr) {
		return FALSE;
	}
	sound.slib_ptrs[sound.field_0xE4] = ptr;

	while (*r27 != -1) { //+ 0x10000 != 0xFFFF

	}
}

BOOL SoundDropData(void) {
	SoundSong* song;
	SoundEffect* effect;
	SoundDVD* dvd;
	SoundStream* stream;
	int i;
	u32* ptr;

	for (i = 0; i < 4; i++) {
		song = &sound.songs[i];
		if (sndSeqGetValid(song->seqId)) {
			sndSeqStop(song->seqId);
			song->seqId = (SND_SEQID)-1;
		}
	}

	for (i = 0; i < 40; i++) {
		effect = &sound.effects[i];
		if (effect->flags) {
			sndFXKeyOff(effect->voiceId);
			if (effect->flags & 0x10) {
				sndRemoveEmitter(&effect->emitter);
			}
			effect->flags = 0;
		}
	}

	for (i = 0; i < 2; i++) {
		dvd = &sound.dvds[i];
		if (dvd->flags & 3) {
			AISetStreamPlayState(0);
			dvd->flags = 0;
			sound.field_0xFC = -1;
			DVDCancelStream(&dvd->info.cb);
			DVDClose(&dvd->info);
		}
	}

	for (i = 0; i < 3; i++) {
		stream = &sound.streams[i];
		if (stream->flags) {
			if (!sound.field_0x20C) {
				sound.interrupts = OSDisableInterrupts();
			}
			sound.field_0x20C++;
			if (stream->flags & 0x400) {
				sound.field_0x226[stream->field_0x2] = 0;
			}
			stream->flags = 0;
			DVDMgrClose(stream->entries[0].entry);
			DVDMgrClose(stream->entries[1].entry);
			sndStreamFree(stream->entries[0].streamId);
			if (stream->field_0x1A == 2) {
				sndStreamFree(stream->entries[1].streamId);
			}
			if (sound.field_0x20C) {
				if (!--sound.field_0x20C) {
					OSRestoreInterrupts(sound.interrupts);
				}
			}
			sound.field_0x104 = -1;
		}
	}

	sndSilence();

	for (i = 0; i < 4; i++) {
		song = &sound.songs[i];
		if (sndSeqGetValid(song->seqId)) {
			sndSeqStop(song->seqId);
			song->seqId = (SND_SEQID)-1;
		}
	}

	for (i = 0; i < 40; i++) {
		effect = &sound.effects[i];
		if (effect->flags) {
			sndFXKeyOff(effect->voiceId);
			if (effect->flags & 0x10) {
				sndRemoveEmitter(&effect->emitter);
			}
			effect->flags = 0;
		}
	}

	for (i = 0; i < 2; i++) {
		dvd = &sound.dvds[i];
		if (dvd->flags & 3) {
			AISetStreamPlayState(0);
			dvd->flags = 0;
			sound.field_0xFC = -1;
			DVDCancelStream(&dvd->info.cb);
			DVDClose(&dvd->info);
		}
	}

	for (i = 0; i < 3; i++) {
		stream = &sound.streams[i];
		if (stream->flags) {
			if (!sound.field_0x20C) {
				sound.interrupts = OSDisableInterrupts();
			}
			sound.field_0x20C++;
			if (stream->flags & 0x400) {
				sound.field_0x226[stream->field_0x2] = 0;
			}
			stream->flags = 0;
			DVDMgrClose(stream->entries[0].entry);
			DVDMgrClose(stream->entries[1].entry);
			sndStreamFree(stream->entries[0].streamId);
			if (stream->field_0x1A == 2) {
				sndStreamFree(stream->entries[1].streamId);
			}
			if (sound.field_0x20C) {
				if (!--sound.field_0x20C) {
					OSRestoreInterrupts(sound.interrupts);
				}
			}
			sound.field_0x104 = -1;
		}
	}

	/*for (i = 0; i < sound.ptr_index; i++) {
		for (ptr = &sound.unk4_ptrs; *ptr; *ptr--) {
			sndPopGroup();
		}
	}*/

	for (i = 0; i < sound.ptr_index; i++) {
		__memFree(HEAP_DEFAULT, sound.pool_ptrs[i]);
	}

	for (i = 0; i < sound.ptr_index; i++) {
		__memFree(HEAP_DEFAULT, sound.proj_ptrs[i]);
	}

	for (i = 0; i < sound.ptr_index; i++) {
		__memFree(HEAP_DEFAULT, sound.sdir_ptrs[i]);
	}

	//TODO: finish
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
		//TODO: finish
	}
}

//mask, B = SND_GROUPID groupId, C = SND_SONGID songId, D = group->field_0x2
void SoundSongPlayCh(s32 id, u32 mask) {
	SoundSong* song;
	SoundGroup* group;

	song = &sound.songs[id];
	if (sndSeqGetValid(song->seqId)) {
		sndSeqStop(song->seqId);
	}
	song->volume = 127;
	for (group = sound.groups; group; group = group->next) {
		if (group->field_0x2 == (u8)mask) {
			break;
		}
	}
	if (group) {

	}
}













void SoundAIDMACallback(void) {

}