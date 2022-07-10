#pragma once

#include <dolphin/mtx.h>

/*
typedef union SoundData {
	const char* name;
	s32 value;
} SoundData;*/

#pragma warn_padding off
typedef struct SoundEnvList {
	u16 field_0x0; //0x0
	u16 field_0x2; //0x2
	u8 field_0x4; //0x4
	u8 field_0x5; //0x5
	u8 field_0x6; //0x6
	u8 field_0x7; //0x7
	u8 field_0x8; //0x8
	//pad 3 bytes
	u32 field_0xC; //0xC
	u32 field_0x10; //0x10
	u32 field_0x14; //0x14
	u8 field_0x18; //0x18
	u8 field_0x19; //0x19
	u8 field_0x1A; //0x1A
	//pad 1 byte
} SoundEnvList;
#pragma warn_padding on

typedef enum SoundEffectFlags {
	SFX_FLAG_2 = (1 << 29),
	SFX_FLAG_4 = (1 << 30),
	SFX_FLAG_8 = (1 << 31)
} SoundEffectFlags;

//most are translated in __psndSFXOn, TODO
typedef struct SoundEffectList {
	const char* name; //0x0, sfx name
	u32 field_0x4; //0x4, lower 2 bytes are SND_FXID
	const char* field_0x8; //0x8, .stm name
	u8 field_0xC; //0xC
	u8 field_0xD; //0xD
	u8 field_0xE; //0xE
	u8 field_0xF; //0xF, reverb
	u8 field_0x10; //0x10
	u8 field_0x11; //0x11
	u16 field_0x12; //0x12
} SoundEffectList;

typedef struct SoundBGMEntry {
	s32 field_0x0; //0x0
	s32 streamId; //0x4
	u8 field_0x8[0x20 - 0x8]; //0x8
	u8 field_0x20; //0x20
	u8 field_0x21[0x38 - 0x21]; //0x21
} SoundBGMEntry;

typedef struct SoundEffectEntry {
	s32 field_0x0; //0x0
	u8 field_0x4[0x6 - 0x4]; //0x4
	s16 field_0x6; //0x6
	u8 field_0x8[0x18 - 0x8]; //0x8
	s32 field_0x18; //0x18
	Vec position; //0x1C
} SoundEffectEntry;

typedef struct SoundEnvEntry {
	u8 field_0x0[0x4 - 0x0]; //0x0
	s16 field_0x4; //0x4
	u8 field_0x6[0x24 - 0x6]; //0x6
} SoundEnvEntry;

typedef struct PaperSoundWork {
	s8 field_0x0; //0x0
	u8 field_0x1[0xC - 0x1]; //0x1
	s8 field_0xC; //0xC
	u8 field_0xD[0x18 - 0xD]; //0xD
	s16 field_0x18; //0x18
	s16 field_0x1A; //0x1A
	u8 field_0x1C[0x30 - 0x1C]; //0x1C
	Vec position; //0x30
	f32 direction; //0x3C
	char field_0x40[0x10]; //0x40
	s16 field_0x50; //0x50
	u8 field_0x52[0x54 - 0x52]; //0x52
	s16 field_0x54; //0x54
	s16 field_0x56; //0x56
	s8 field_0x58; //0x58
	s8 field_0x59; //0x59
	s8 field_0x5A; //0x5A
	u8 field_0x5B; //0x5B, unused?
	s8 field_0x5C; //0x5C
	u8 field_0x5D[0x60 - 0x5D]; //0x5D
} PaperSoundWork;

s32 searchPSSFXList(const char* name);
void psndInit(void);
void psndMain(void);
void psndMainInt(void);
void psndExit(void);




BOOL psndPushGroup(void);


BOOL psndBGMOn(u32 r3, char* bgm_name);
void psndStopAllFadeOut(void);
BOOL psndENVOff(u32 r3);
void psndSFXAllOff(void);


void psndSFXOn(const char* name);


BOOL psndBGMOff_f_d(s32 flags, u16 fadetime, BOOL someswitch);
BOOL psndBGMStartCheck(s32 id);

void psndSetFlag(s16 flag);
void psndClearFlag(s16 flag);

void psndSetPosDirListener(Vec* position, f32 direction);