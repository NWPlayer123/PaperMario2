#pragma once

#include "mgr/dvdmgr.h"
#include <dolphin/ai.h>
#include <dolphin/ar.h>
#include <dolphin/arq.h>
#include <dolphin/dvd.h>
#pragma warn_padding off
#include <musyx.h>
#pragma warn_padding on

typedef void (*ReverbCallback)(u8 reason, SND_AUX_INFO* info, void* user);

typedef struct SoundGroup { //TODO: rename
	u16 flags; //0x0
	u16 field_0x2; //0x2
	SND_GROUPID groupId; //0x4
	SND_SONGID songId; //0x6
	void* arrfile; //0x8
	struct SoundGroup* next; //0xC
} SoundGroup;

typedef struct SoundSong {
	SND_SEQID seqId; //0x0
	u32 vol; //0x4, casts to u8
	void* field_0x8; //0x8
	u32 mask; //0xC
} SoundSong;

typedef struct SoundEffect {
	u16 flags; //0x0
	u16 field_0x2; //0x2, count or active or smth
	u16 vol; //0x4
	u16 pan; //0x6
	u16 span; //0x8
	u8 field_0xA[0xC - 0xA]; //0xA
	SND_VOICEID voiceId; //0xC
	u16 fid; //0x10
	u8 field_0x12[0x14 - 0x12]; //0x12
	SND_EMITTER emitter; //0x14
	u8 field_0x64[0x88 - 0x64]; //0x64
} SoundEffect;

typedef struct SoundDVD {
	u16 flags; //0x0
	u8 field_0x2[0x118 - 0x2]; //0x2
	DVDFileInfo info; //0x118
} SoundDVD;

typedef struct SoundStreamEntry {
	DVDEntry* dvd; //0x0
	ARQRequest request; //0x4
	SND_STREAMID streamId; //0x24
	void* field_0x28; //0x28, TODO: re-type? buffer
	void* field_0x2C; //0x2C, TODO: re-type?
	void* field_0x30; //0x30, TODO: re-type?
	u8 field_0x34[0x88 - 0x34]; //0x34
} SoundStreamEntry;

typedef struct SoundStream {
	u16 flags; //0x0
	u16 field_0x2; //0x2
	u8 field_0x4[0x6 - 0x4]; //0x4
	u16 pan; //0x6
	u16 span; //0x8
	u8 field_0xA[0xC - 0xA]; //0xA
	f32 vol; //0xC
	f32 field_0x10; //0x10, some volume setting
	f32 field_0x14; //0x14, some other volume setting
	u16 freq; //0x18
	u16 count; //0x1A
	u8 field_0x1C[0x28 - 0x1C]; //0x1C
	SoundStreamEntry entries[2]; //0x28
} SoundStream;

typedef struct SoundWork {
	void* pool_ptrs[8]; //0x0
	void* proj_ptrs[8]; //0x20
	void* sdir_ptrs[8]; //0x40
	void* slib_ptrs[8]; //0x60
	u8 field_0x80[0xC0 - 0x80]; //0x80, tables for etbl/samp/slib/stbl
	u32 unk4_ptrs[8]; //0xC0, etbl/samp/slib/stbl?
	u32 ptr_index; //0xE0, not incremented, memory leak if SoundLoadDVD2 called multiple times
	u32 field_0xE4; //0xE4, TODO: change ptr_index and this for slib name
	u32 field_0xE8; //0xE8
	SoundGroup* groups; //0xEC
	SoundSong* songs; //0xF0
	SoundEffect* effects; //0xF4
	SoundDVD* dvds; //0xF8
	s32 field_0xFC; //0xFC
	SoundStream* streams; //0x100
	s32 field_0x104; //0x104
	ReverbCallback reverb_hi_cb; //0x108
	SND_AUX_REVERBHI* reverb_hi; //0x10C
	u8 field_0x110; //0x110
	u8 align_0x111[3]; //0x111
	s32 field_0x114; //0x114
	ReverbCallback chorus_cb; //0x118
	SND_AUX_CHORUS* chorus; //0x11C
	u8 field_0x120; //0x120
	u8 align_0x121[3]; //0x121
	s32 field_0x124; //0x124
	u8 field_0x128[0x208 - 0x128]; //0x128
	u16 fadeInTime; //0x208
	u16 fadeOutTime; //0x20A
	u16 field_0x20C; //0x20C
	u8 field_0x20E[0x210 - 0x20E]; //0x20E
	BOOL interrupts; //0x210
	u16 field_0x214; //0x214, some "mute" thing
	u8 pad_0x216[2]; //0x216
	SND_OUTPUTMODE outputMode; //0x218
	u8 field_0x21C[0x224 - 0x21C]; //0x21C
	u16 field_0x224; //0x224
	u16 field_0x226[2]; //0x226
	char filepath[0x100]; //0x22A
	u8 pad_0x32A[2]; //0x32A
	void* filedata; //0x32C
	s16 field_0x330; //0x330
	u8 pad_0x332[2]; //0x332
	u8* dmaAddress; //0x334
	u32 dmaLength; //0x338
	u8 field_0x33C[0x5BC - 0x33C]; //0x33C
	AIDCallback callback; //0x5BC
	u8 field_0x5C0[0x15C0 - 0x5C0]; //0x5C0
	s32 field_0x15C0; //0x15C0
} SoundWork;

void SoundInit(void);
void SoundMain(void);
void SoundMainInt(void);
void SoundSetFadeTime(u16 in, u16 out);
BOOL SoundLoadDVD2(const char* path);
BOOL SoundLoadDVD2PushGroup(u8* group_ids);
void SoundLoadDVD2Free(void);
BOOL SoundSLibLoadDVD(const char* path);
BOOL SoundDropData(void);
void SoundSetOutputMode(SND_OUTPUTMODE mode);
void SoundOpenCover(void);
void SoundCloseCover(void);
void SoundSongPlayCh(s32 id, u32 mask);







