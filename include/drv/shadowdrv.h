#pragma once

#include <dolphin/types.h>

typedef struct DepthShadowWork {
	u8 field_0x0[0x104 - 0x0]; //0x0
} DepthShadowWork;

typedef struct ProjShadowWork {
	u8 field_0x0[0x104 - 0x0]; //0x0
} ProjShadowWork;

typedef struct CharShadowWork {
	u8 field_0x0[0x11C - 0x0]; //0x0
} CharShadowWork;

void shadowInit(void);
void shadowMain(void);