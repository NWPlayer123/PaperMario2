#include "sound.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include "system.h"
#include <math.h>
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
void SoundEfxMain(void);
void SoundDVDMain(void);



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
	u32 size;
	int i, j;

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
	sound.dataId = 0;
	sound.slibId = 0;
	sound.field_0x214 = 0;
	sound.field_0x224 = 0;
	sound.field_0xE8 = 0;

	sound.filedata = __memAlloc(HEAP_DEFAULT, 0x20000);
	if (!sound.filedata) {while (1) ;}

	sound.groups = NULL;

	songs = __memAlloc(HEAP_DEFAULT, sizeof(SoundSong) * 4);
	sound.songs = songs;
	if (!songs) {while (1) ;}
	for (i = 0; i < 4; i++) {
		songs[i].seqId = (SND_SEQID)-1;
		songs[i].field_0x8 = 0;
	}

	effects = __memAlloc(HEAP_DEFAULT, sizeof(SoundEffect) * 40);
	sound.effects = effects;
	if (!effects) {while (1) ;}
	for (i = 0; i < 40; i++) {
		effects[i].flags = 0;
		effects[i].voiceId = (SND_VOICEID)-1;
		effects[i].field_0x2 = 0;
		effects[i].vol = 127;
		effects[i].pan = 64;
	}

	dvds = __memAlloc(HEAP_DEFAULT, sizeof(SoundDVD) * 2);
	sound.dvds = dvds;
	if (!dvds) {while (1) ;}
	for (i = 0; i < 2; i++) {
		dvds[i].flags = 0;
	}

	streams = __memAlloc(HEAP_DEFAULT, sizeof(SoundStream) * 3);
	sound.streams = streams;
	if (!streams) {while (1) ;}
	for (i = 0; i < 3; i++, streams++) {
		streams->flags = 0;

		for (j = 0; j < 2; j++) {
			unk1 = __memAlloc(HEAP_DEFAULT, 0x8000);
			if (!unk1) {while (1) ;}
			streams->entries[0].field_0x2C = unk1;

			unk2 = __memAlloc(HEAP_DEFAULT, 0x8000);
			if (!unk2) {while (1) ;}
			streams->entries[0].field_0x30 = unk2;

			size = sndStreamAllocLength(0xE00, 0);
			unk3 = __memAlloc(HEAP_DEFAULT, size);
			if (!unk3) {while (1) ;}
			streams->entries[0].field_0x28 = unk3;
		}
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

void* loadDVD_callback(u32 offset, u32 bytes) { //1:1
	DVDFileInfo info;

	if (!DVDOpen(sound.filepath, &info)) {
		return NULL;
	}
	if (DVDRead(&info, sound.filedata, (s32)bytes, (s32)offset) <= 0) {
		return NULL;
	}
	DVDClose(&info);
	return sound.filedata;
}

BOOL SoundLoadDVD2(const char* path) { //almost 1:1, needs register changes
	DVDEntry* entry;
	void* ptr;
	u32 size;

	if (sound.dataId >= 8) {
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
	sound.poolData[sound.dataId] = ptr;

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
	sound.projData[sound.dataId] = ptr;

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
	sound.sdirData[sound.dataId] = ptr;

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".samp");
	strcpy(sound.filepath, str);
	sndSetSampleDataUploadCallback(loadDVD_callback, 0x20000);
	return TRUE;
}

BOOL SoundLoadDVD2PushGroup(u8* groupIds) { //1:1
	SND_GROUPID id;
	s32 temp;

	if (sound.dataId >= 8) {
		return 0;
	}
	sound.groupCount[sound.dataId] = 0;
	if (groupIds) {
		while (*groupIds != 0xFF) {
			id = *groupIds;
			sndPushGroup(
				sound.projData[sound.dataId],
				id,
				0,
				sound.sdirData[sound.dataId],
				sound.poolData[sound.dataId]
			);
			sound.groupCount[sound.dataId]++;
			groupIds++;
		};
	}
	else {
		temp = sound.field_0xE8;
		while (temp < 0xFF) {
			if (sndPushGroup(
				sound.projData[sound.dataId],
				(SND_GROUPID)temp,
				0,
				sound.sdirData[sound.dataId],
				sound.poolData[sound.dataId]
			) != TRUE) {
				break;
			}
			sound.groupCount[sound.dataId]++;
			sound.field_0xE8++;
			temp++;
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
	SoundGroup *group, *iter;
	void* ptr;
	DVDEntry* entry;
	u32 *data, *v14;
	u32 size, groupSize;
	u16 id;

	if (sound.slibId >= 8) {
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
	data = ptr;
	if (!ptr) {
		return FALSE;
	}
	sound.slibData[sound.slibId] = ptr;

	while (1) {
		groupSize = *data;
		if (*data == 0xFFFFFFFF) {
			break;
		}
		group = __memAlloc(HEAP_DEFAULT, sizeof(SoundGroup));
		if (!group) {while (1) ;}
		id = 0;
		if (!sound.groups) {
			sound.groups = group;
		}
		else {
			for (iter = sound.groups ;; iter = iter->next) {
				id++;
				if (!iter->next) {
					break;
				}
			}
			iter->next = group;
		}
		v14 = data + 8;
		group->flags = 1;
		data = (u32*)((u32)data + groupSize);
		group->field_0x2 = id;
		group->groupId = 0;
		group->songId = 0;
		group->arrfile = v14;
		group->next = 0;
	}

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".stbl");
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
	sound.stblData[sound.slibId] = ptr;

	sprintf(str, "%s/%s%s", getMarioStDvdRoot(), path, ".etbl");
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
	sound.etblData[sound.slibId] = ptr;
	sound.slibId++;
	return TRUE;
}

BOOL SoundDropData(void) {
    SoundSong *song;
    SoundEffect *effect;
    SoundDVD *dvd;
    SoundStream *stream;
    SoundGroup *group, *temp;
    u32 i, *j;

    group = sound.groups;
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

    for (i = 0; i < sound.dataId; i++) {
		for (j = sound.groupCount; *j;) {
			sndPopGroup();
			sound.groupCount[i]--;
		}
    }
    
    for (i = 0; i < sound.dataId; i++) {
        __memFree(HEAP_DEFAULT, sound.poolData[i]);
    }

    for (i = 0; i < sound.dataId; i++) {
        __memFree(HEAP_DEFAULT, sound.projData[i]);
    }

    for (i = 0; i < sound.dataId; i++) {
        __memFree(HEAP_DEFAULT, sound.sdirData[i]);
    }

    for (i = 0; i < sound.slibId; i++) {
        __memFree(HEAP_DEFAULT, sound.slibData[i]);
        __memFree(HEAP_DEFAULT, sound.stblData[i]);
        __memFree(HEAP_DEFAULT, sound.etblData[i]);
    }

    sound.dataId = 0;
    sound.slibId = 0;
    sound.field_0xE8 = 0;
    
    while (1) {
        if (!group) {
            break;
        }
        temp = group->next;
        if (!(group->flags & 1)) {
            __memFree(HEAP_DEFAULT, group->arrfile);
        }
        __memFree(HEAP_DEFAULT, group);
        group = temp;
    }
    sound.groups = 0;

    return TRUE;
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
	SoundStream* stream;
	BOOL close;
	int i;

	if (sndIsInstalled() && sound.field_0x214 == 1) {
		close = FALSE;
		stream = sound.streams;
		for (i = 0; i < 3; i++, stream++) {
			if (stream->flags & 8 && !(stream->flags & 0x400) && !(stream->flags & 2)) {
				if (stream->flags & 0x1000) {
					close = TRUE;
				}
				if (stream->count == 2 && stream->flags & 0x2000) {
					close = TRUE;
				}
			}
		}
		if (close) {
			sound.field_0x214 = 2;
		}
		else {
			sound.field_0x214 = 0;
		}
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
	group = SoundSongGetGroup((u8)a2);
	if (group) {
		song->field_0x8 = group;
		group->groupId = (SND_GROUPID)((a2 >> 16) & 0xFF);
		group->songId = (SND_SONGID)((a2 >> 8) & 0xFF);
		song->mask = 0xFFFFFFFF;
		song->seqId = sndSeqPlayEx(group->groupId, group->songId, group->arrfile, 0, 0);
		sndSeqVolume((u8)song->vol, 0, song->seqId, 0);
		sndSeqContinue(song->seqId);
		sndSeqMute(song->seqId, song->mask, 0);
	}
}

void SoundSongContinueCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqContinue(song->seqId);
		sndSeqVolume((u8)song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
	}
}

void SoundSongStopCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqStop(song->seqId);
		song->seqId = (SND_SEQID)-1;
	}
}

void SoundSongFadeoutCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqVolume(0, sound.fadeOutTime, song->seqId, SND_SEQVOL_STOP);
		song->seqId = (SND_SEQID)-1;
	}
}

void SoundSongFadeinCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		sndSeqVolume((u8)song->vol, sound.fadeInTime, song->seqId, SND_SEQVOL_CONTINUE);
	}
}

void SoundSongSetVolCh(s32 chan, u8 vol) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		if (vol > 127) {
			vol = 127;
		}
		song->vol = vol;
		sndSeqVolume((u8)song->vol, 0, song->seqId, SND_SEQVOL_CONTINUE);
	}
}

u8 SoundSongGetVolCh(s32 chan) {
	SoundSong* song = &sound.songs[chan];

	if (sndSeqGetValid(song->seqId)) {
		return (u8)song->vol;
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
		sndFXPitchBend(vid, (u16)(pitch + 0x2000));
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
	return (u8)sound.effects[chan].vol;
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
				offset = (u16)(3 * (span - 64));
			else
				offset = (u16)(3 * (64 - span));
			doppler = (u16)(0x2000 - offset);
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

inline void SoundEfxCheckCh(s32 chan) {
    SoundEffect* effect;
    SND_VOICEID vid;

    effect = &sound.effects[chan];
    if (effect->flags & 0x10) {
        vid = sndFXCheck(sndEmitterVoiceID(&effect->emitter));
    }
    else {
        vid = sndFXCheck(effect->voiceId);
    }
    if (sndFXCheck(vid) == -1) {
        SoundEfxMuteCh(chan);
    }
}

void SoundEfxMain(void) {
    SoundEffect* effect;
    f32 x, y, z;
    f32 v6, v9, v10;
    int i;

    x = sound.prevPos.x;
    y = sound.prevPos.y;
    z = sound.prevPos.z;
    v6 = ((TWO_PI * sound.field_0x1F4) / 360.0f);
    sound.prevPos.x = sound.pos.x;
    sound.prevPos.y = sound.pos.y;
    sound.prevPos.z = sound.pos.z;
    sound.dir.x = sound.pos.x - x;
    sound.dir.y = sound.pos.y - y;
    sound.dir.z = sound.pos.z - z;
    v9 = ((1.0f * (f32)sin(v6)) + 0.0f);
    v10 = (f32)cos(v6);
    sound.heading.x = v9;
    sound.heading.y = 0.0f;
    sound.heading.z = -((1.0f * v10) - 0.0f);
    sound.up.x = 0.0f;
    sound.up.y = 1.0f;
    sound.up.z = 0.0f;
    sndUpdateListener(&sound.listener, &sound.pos, &sound.dir, &sound.heading, &sound.up, (u8)sound.listenerVol, NULL);

    effect = sound.effects;
    for (i = 0; i < 40; i++, effect++) {
        if (effect->flags) {
            if (effect->flags & 0x10) {
                effect->dir.x = effect->pos.x - effect->prevPos.x;
                effect->dir.y = effect->pos.y - effect->prevPos.y;
                effect->dir.z = effect->pos.z - effect->prevPos.z;
                effect->prevPos.x = effect->pos.x;
                effect->prevPos.y = effect->pos.y;
                effect->prevPos.z = effect->pos.z;
                sndUpdateEmitter(&effect->emitter, &effect->pos, &effect->dir, (u8)effect->vol, NULL);
            }
        }
    }

    effect = sound.effects;
    for (i = 0; i < 40; i++, effect++) {
        if (effect->flags) {
            if (!(effect->flags & 0x10)) {
                SoundEfxCheckCh(i);
            }
            else {
                if (!sndCheckEmitter(&effect->emitter)) {
                    SoundEfxMuteCh(i);
                }
                if (effect->field_0xA) {
                    effect->field_0xA--;
                }
                else {
                    SoundEfxCheckCh(i);
                }
            }
        }
    }
}

inline void SoundDVDStopCh(s32 chan) {
    SoundDVD* dvd = &sound.dvds[chan];
    if (dvd->flags & 3) {
        AISetStreamPlayState(0);
        dvd->flags = 0;
        sound.field_0xFC = -1;
        DVDCancelStream(&dvd->info.cb);
        DVDClose(&dvd->info);
    }
}

inline void SoundDVDSetPanCh(s32 chan, u8 pan) { //I think this is the correct func name
    SoundDVD* dvd;
    f32 left, right;

    dvd = &sound.dvds[chan];
    dvd->field_0x2 = pan;
    left = dvd->field_0x8;
    right = left;
    if (pan < 64) {
        right = (((f32)pan * dvd->field_0x8) / 63.0f);
    }
    if (!((u32)(pan - 64) >> 31)) {
        left = (((f32)(127 - pan) * dvd->field_0x8) / 63.0f);
    }
    AISetStreamVolLeft((u8)left);
    AISetStreamVolRight((u8)right);
}

void SoundDVDMain(void) {
    SoundDVD *dvd;
    int i;

    for (i = 0; i < 2; i++) {
        dvd = &sound.dvds[i];
        if (!(dvd->flags & 2)) {
            if (dvd->flags & 0x40) {
                if (dvd->field_0x8 == 0.0f) {
                    dvd->samples = AIGetStreamSampleCount();
                    SoundDVDStopCh(i);
                    dvd->flags |= 0x2;
                    dvd->flags &= ~0x40;
                    continue;
                }
                dvd->field_0x8 -= dvd->field_0xC;
                if ( dvd->field_0x8 <= 0.0f ) {
                    dvd->field_0x8 = 0.0f;
                }
                SoundDVDSetPanCh(i, (u8)dvd->field_0x2);
            }
            if (dvd->flags & 0x20) {
                if (dvd->field_0x8 == 0.0f) {
                    dvd->flags &= ~0x20;
                    SoundDVDStopCh(i);
                    continue;
                }
                dvd->field_0x8 -= dvd->field_0xC;
                if (dvd->field_0x8 <= 0.0f) {
                    dvd->field_0x8 = 0.0f;
                }
                SoundDVDSetPanCh(i, (u8)dvd->field_0x2);
            }
            if (dvd->flags & 0x10) {
                dvd->field_0x8 += dvd->field_0xC;
                if (dvd->field_0x8 >= dvd->field_0x4) {
                    dvd->field_0x8 = dvd->field_0x4;
                    dvd->field_0xC = 0.0f;
                    dvd->flags &= ~0x10;
                }
                SoundDVDSetPanCh(i, (u8)dvd->field_0x2);
            }
        }
    }
}

void _ssDVDReadAsync_activeChk(s32 result, DVDFileInfo* fileInfo) {

}

void cache_flush(u32 pointerToARQRequest) {

}

void _ssDVDReadAsync_cache_aram(u32 pointerToARQRequest) {

}

void _ssDVDReadAsync_cache_next(s32 result, DVDFileInfo* fileInfo) {

}






u32 _sscallback(void* buffer1, u32 len1, void* buffer2, u32 len2, u32 user) {

}

void SoundSSPlayChEx_main(s32 result, DVDFileInfo* fileInfo) {
	if (result >= 0) {

	}
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
