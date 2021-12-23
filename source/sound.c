#include "sound.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include "system.h"
#include <string.h>

extern int sprintf(char* str, const char* format, ...);

//.bss
SND_AUX_REVERBHI revH;
SND_AUX_REVERBSTD revS;
SND_AUX_CHORUS cho;
SND_AUX_DELAY dly;
SoundWork sound;
static char str[0x100];

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
	SoundSong* songs;
	SoundEffect* effects;
	SoundDVD* dvds;
	SoundStream* streams;
	void *unk1, *unk2, *unk3;
	u32 length;
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
	sound.callback = AIRegisterDMACallback(SoundAIDMACallback);
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

	songs = __memAlloc(HEAP_DEFAULT, sizeof(SoundSong) * 4);
	sound.songs = songs;
	if (!songs) {
		while (1) { ; } //infinite loop
	}
	songs[0].seqId = (SND_SEQID)-1;
	songs[0].field_0x8 = 0;
	songs[1].seqId = (SND_SEQID)-1;
	songs[1].field_0x8 = 0;
	songs[2].seqId = (SND_SEQID)-1;
	songs[2].field_0x8 = 0;
	songs[3].seqId = (SND_SEQID)-1;
	songs[3].field_0x8 = 0;

	effects = __memAlloc(HEAP_DEFAULT, sizeof(SoundEffect) * 40);
	sound.effects = effects;
	if (!effects) {
		while (1) { ; } //infinite loop
	}
	for (i = 0; i < 40; i++) {
		effects[i].flags = 0;
		effects[i].voiceId = -1;
		effects[i].field_0x2 = 0;
		effects[i].vol = 127;
		effects[i].pan = 64;
	}

	dvds = __memAlloc(HEAP_DEFAULT, sizeof(SoundDVD) * 2);
	sound.dvds = dvds;
	if (!dvds) {
		while (1) { ; } //infinite loop
	}
	dvds[0].flags = 0;
	dvds[1].flags = 0;

	streams = __memAlloc(HEAP_DEFAULT, sizeof(SoundStream) * 3);
	sound.streams = streams;
	if (!streams) {
		while (1) { ; } //infinite loop
	}
	for (i = 0; i < 3; i++, streams++) {
		streams->flags = 0;

		unk1 = __memAlloc(HEAP_DEFAULT, 0x8000);
		if (!unk1) {
			while (1) { ; } //infinite loop
		}
		streams->entries[0].field_0x2C = unk1;

		unk2 = __memAlloc(HEAP_DEFAULT, 0x8000);
		if (!unk2) {
			while (1) { ; } //infinite loop
		}
		streams->entries[0].field_0x30 = unk2;

		length = sndStreamAllocLength(0xE00, 0);
		unk3 = __memAlloc(HEAP_DEFAULT, length);
		if (!unk3) {
			while (1) { ; } //infinite loop
		}
		streams->entries[0].field_0x28 = unk3;
		//---------------------------------------------
		unk1 = __memAlloc(HEAP_DEFAULT, 0x8000);
		if (!unk1) {
			while (1) { ; } //infinite loop
		}
		streams->entries[1].field_0x2C = unk1;

		unk2 = __memAlloc(HEAP_DEFAULT, 0x8000);
		if (!unk2) {
			while (1) { ; } //infinite loop
		}
		streams->entries[1].field_0x30 = unk2;

		length = sndStreamAllocLength(0xE00, 0);
		unk3 = __memAlloc(HEAP_DEFAULT, length);
		if (!unk3) {
			while (1) { ; } //infinite loop
		}
		streams->entries[1].field_0x28 = unk3;
	}
	sound.reverb_hi_cb = sndAuxCallbackReverbHI;
	sound.reverb_hi = &revH;
	sound.field_0x110 = 0xFF;
	sound.field_0x114 = 0;
	sound.chorus_cb = sndAuxCallbackChorus;
	sound.chorus = &cho;
	sound.field_0x120 = 0xFF;
	sound.field_0x124 = 0;
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
			DVDMgrClose(stream->entries[0].dvd);
			DVDMgrClose(stream->entries[1].dvd);
			sndStreamFree(stream->entries[0].streamId);
			if (stream->count == 2) {
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
			DVDMgrClose(stream->entries[0].dvd);
			DVDMgrClose(stream->entries[1].dvd);
			sndStreamFree(stream->entries[0].streamId);
			if (stream->count == 2) {
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
















inline SoundGroup* SoundSongGetGroup(u8 a1) {
	u16 temp = a1;
	SoundGroup* group;

	for (group = sound.groups ;; group = group->next) {
		if (group == 0) {
			return 0;
		}
		if (group->field_0x2 == temp) {
			break;
		}
	}
	return group;
}

void SoundSongPlayCh(s32 chan, u32 a2) {
	SoundSong* song;
	SoundGroup* group;

	song = &sound.songs[chan];
	if (sndSeqGetValid(song->seqId) == 1) {
		sndSeqStop(song->seqId);
	}
	song->vol = 127;
	group = SoundSongGetGroup(a2);
	if (group) {
		song->field_0x8 = group;
		group->groupId = (a2 >> 16) & 0xFF;
		group->songId = (a2 >> 8) & 0xFF;
		song->mask = -1;
		song->seqId = sndSeqPlayEx(group->groupId, group->songId, group->arrfile, 0, 0);
		sndSeqVolume(song->vol, 0, song->seqId, 0);
		sndSeqContinue(song->seqId);
		sndSeqMute(song->seqId, song->mask, 0);
	}
}

void SoundSongContinueCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqContinue(song->seqId);
		sndSeqVolume(song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
	}
}

void SoundSongStopCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqStop(song->seqId);
		song->seqId = -1;
	}
}

void SoundSongFadeoutCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqVolume(0, sound.fadeOutTime, song->seqId, SND_SEQVOL_STOP);
		song->seqId = -1;
	}
}

void SoundSongFadeinCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqVolume(song->vol, sound.fadeInTime, song->seqId, SND_SEQVOL_CONTINUE);
	}
}

void SoundSongSetVolCh(s32 chan, u8 vol) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		if (vol > 127) {
			vol = 127;
		}
		song->vol = vol;
		sndSeqVolume(song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
	}
}

u8 SoundSongGetVolCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		return song->vol;
	}
	else {
		return 0;
	}
}

BOOL SoundSongCheck(s32 chan) {
	return sndSeqGetValid(sound.songs[chan].seqId) != 0;
}

inline void SoundEfxMuteCh(s32 chan) {
	SoundEffect* sfx = &sound.effects[chan];
	if (sfx->flags) {
		sndFXKeyOff(sfx->voiceId);
		if (sfx->flags & 0x10) {
			sndRemoveEmitter(&sfx->emitter);
		}
		sfx->flags = 0;
	}
}

inline s32 SoundEfxStopAll(u8 a1) {
	SoundEffect* sfx = sound.effects;
	s32 chan = 0;

	for (chan = 0; chan < 40; chan++, sfx++) {
		if (sfx->field_0x2 <= a1) {
			SoundEfxMuteCh(chan);
			break;
		}
	}
	return chan;
}

inline u32 SoundEfxCheckPlay(s32 chan) {
	SoundEffect* sfx = &sound.effects[chan];
	SND_VOICEID vid;
	if (!sfx->flags) {
		return FALSE;
	}
	if (!(sfx->flags & 0x10)) {
		if (sfx->flags & 0x10) {
			vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
		}
		else {
			vid = sndFXCheck(sfx->voiceId);
		}
		if (sndFXCheck(vid) == -1) {
			return FALSE;
		}
	}
	else if (!sndCheckEmitter(&sfx->emitter)) {
		return FALSE;
	}
	return TRUE;
}


s32 SoundEfxPlayEx(SND_FXID fid, u8 a2, u8 vol, u8 pan) {
	SoundEffect* sfx;
	s32 chan;

	sfx = sound.effects;
	for (chan = 0; chan < 40; chan++, sfx++) {
		if (!sfx->flags) {
			break;
		}
	}
	if (chan >= 40u && SoundEfxStopAll(a2) >= 40u) {
		return -1;
	}
	sfx->flags = 0;
	if (!sound.field_0x20C) {
		sound.interrupts = OSDisableInterrupts();
	}
	++sound.field_0x20C;
	sfx->voiceId = sndFXStart(fid, 127, pan);
	sndFXVolume(sfx->voiceId, vol);
	if (sound.field_0x20C && !--sound.field_0x20C) {
		OSRestoreInterrupts(sound.interrupts);
	}
	if (sfx->voiceId == -1) {
		return -1;
	}
	sfx->flags = 1;
	sfx->field_0x2 = a2;
	sfx->vol = vol;
	sfx->pan = pan;
	sfx->span = 0;
	sfx->fid = fid;
	if (sound.field_0x214) {
		sndFXVolume(sfx->voiceId, 0);
	}
	if (!SoundEfxCheckPlay(chan)) {
		chan = -1;
	}
	return chan;
}

void SoundEfxStop(s32 chan) {
	SoundEffect* sfx;

	sfx = &sound.effects[chan];
	if (sfx->flags) {
		sndFXKeyOff(sfx->voiceId);
		if (sfx->flags & 0x10) {
			sndRemoveEmitter(&sfx->emitter);
		}
		sfx->flags = 0;
	}
}

void SoundEfxSetPitch(s32 chan, u16 pitch) {
	SoundEffect* sfx;
	SND_VOICEID vid;

	sfx = &sound.effects[chan];
	if (sfx->flags & 0x10) {
		vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
	}
	else {
		vid = sndFXCheck(sfx->voiceId);
	}
	if (vid != -1) {
		sndFXPitchBend(vid, pitch + 0x2000);
	}
}

void SoundEfxSetVolume(s32 chan, u8 vol) {
	SoundEffect* sfx;
	SND_VOICEID vid;

	sfx = &sound.effects[chan];
	if (sfx->flags & 0x10) {
		vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
	}
	else {
		vid = sndFXCheck(sfx->voiceId);
	}
	if (vid != -1) {
		sfx->vol = vol;
		sndFXVolume(vid, vol);
		if (sound.field_0x214) {
			sndFXVolume(vid, 0);
		}
	}
}

u8 SoundEfxGetVolume(s32 chan) {
	return sound.effects[chan].vol;
}

void SoundEfxSetAux1(s32 chan, u8 reverb) {
	SoundEffect* sfx;
	SND_VOICEID vid;

	sfx = &sound.effects[chan];
	if (sfx->flags & 0x10) {
		vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
	}
	else {
		vid = sndFXCheck(sfx->voiceId);
	}
	if (vid != -1) {
		sndFXReverb(vid, reverb);
	}
}

void SoundEfxSetPan(s32 chan, u8 pan) {
	SoundEffect* sfx;
	SND_VOICEID vid;

	sound.effects[chan].pan = pan;
	sfx = &sound.effects[chan];
	if (sfx->flags & 0x10) {
		vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
	}
	else {
		vid = sndFXCheck(sfx->voiceId);
	}
	if (vid != -1) {
		sndFXPanning(vid, pan);
	}
}

void SoundEfxSetSrndPan(s32 chan, u8 span) {
	SoundEffect* sfx;
	SND_VOICEID vid;
	u16 doppler;
	u16 offset;

	sound.effects[chan].span = span;
	sfx = &sound.effects[chan];
	if (sfx->flags & 0x10) {
		vid = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
	}
	else {
		vid = sndFXCheck(sfx->voiceId);
	}
	if (vid != -1) {
		sndFXSurroundPanning(vid, span);
		if (span) {
			if (span >= 64)
				offset = 3 * (span - 64);
			else
				offset = 3 * (64 - span);
			doppler = 0x2000 - offset;
		}
		else {
			doppler = 0x2000;
		}
		sndFXDoppler(vid, doppler);
	}
}

void SoundEfxSetLPF(s32 chan, s16 freq) {
	SoundEffect* sfx;
	u32 voiceId;

	sfx = &sound.effects[chan];
	if (sfx->flags & 0x10) {
		voiceId = sndFXCheck(sndEmitterVoiceID(&sfx->emitter));
	}
	else {
		voiceId = sndFXCheck(sfx->voiceId);
	}
	if (voiceId != -1) {
		if (freq && freq < 16000) {
			sndFXSetFilter(voiceId, SND_FILTER_LOWPASS, freq);
		}
		else {
			sndFXSetFilter(voiceId, SND_FILTER_NONE, 0);
		}
	}
}

BOOL SoundEfxCheck(s32 chan) {
	SoundEffect* sfx;

	sfx = &sound.effects[chan];
	if (!sfx->flags) {
		return FALSE;
	}
	if (sfx->flags & 0x10 && !sndCheckEmitter(&sfx->emitter)) {
		return FALSE;
	}
	else if (sndFXCheck(sndFXCheck(sfx->voiceId)) == -1) {
		return FALSE;
	}
	return TRUE;
}








s32 SoundSSGetVolCh(s32 chan) {
	SoundStream* stream;

	stream = &sound.streams[chan];
	if (stream->flags) {
		return (s32)stream->vol;
	}
	else {
		return 0;
	}
}








void SoundSSMainInt(void) {

}

void SoundAIDMACallback(void) {
	u32 length;
	u8* address;

	sound.callback();
	sound.dmaAddress = (u8*)(AIGetDMAStartAddr() | 0x80000000);
	length = AIGetDMALength();
	address = sound.dmaAddress;
}
