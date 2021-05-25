#pragma once

#include <dolphin/mtx.h>

typedef struct SoundEffectList {
	const char* name; //0x0
	u32 field_0x4; //0x4
	const char* field_0x8; //0x8
	u8 field_0xC; //0xC
	u8 field_0xD; //0xD
	u8 field_0xE; //0xE
	u8 field_0xF; //0xF
	u8 field_0x10; //0x10
	u8 field_0x11; //0x11
	s16 field_0x12; //0x12
} SoundEffectList;

typedef struct SoundEffectEntry {
	u8 field_0x0[0x1C - 0x0]; //0x0
	Vec position; //0x1C
} SoundEffectEntry;

typedef struct SoundWork {
	u8 field_0x0[0x60 - 0x0]; //0x0
} SoundWork;

typedef struct SoundEnvironment {
	u8 field_0x0[0x240 - 0x0]; //0x0
} SoundEnvironment;

void psndInit(void);
void psndMain(void);

void psndExit(void);
BOOL psndBGMOn(u32 r3, char* bgm_name);
void psndStopAllFadeOut(void);
BOOL psndENVOff(u32 r3);
void psndSFXAllOff(void);


//s32 searchPSSFXList(const char* name);
