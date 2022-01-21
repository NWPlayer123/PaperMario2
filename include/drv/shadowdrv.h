#pragma once

#include <dolphin/types.h>

typedef struct DepthShadowWork {
	u8 field_0x0[0x104 - 0x0]; //0x0
} DepthShadowWork;

typedef struct ProjShadowWork {
	u8 field_0x0[0x104 - 0x0]; //0x0
} ProjShadowWork;

typedef struct CharShadowWork {
	s32 field_0x0; //0x0
	s32 field_0x4; //0x4
	s32 field_0x8; //0x8
	u8 field_0xC[0x11C - 0xC]; //0xC
} CharShadowWork;

void shadowInit(void);
void shadowMain(void);