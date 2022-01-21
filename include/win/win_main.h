#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"

typedef struct WindowWork {
	s16 flags; //0x0
	u8 align_2[2]; //0x2
	u32 buttonTrg; //0x4
	u32 buttonRep; //0x8
	u32 dirTrg; //0xC
	u32 dirRep; //0x10
	u8 field_0x14[0x20 - 0x14]; //0x14
	s32 menuState; //0x20, TODO: rename?
	u8 field_0x24[0x1210 - 0x24]; //0x24
} WindowWork;

WindowWork* winGetPtr(void);
void winInit(void);
void winReInit(void);
BOOL winCheck(void);
void winOpenEnable(void);
void winOpenDisable(void);
USERFUNC_DEF(itemUseFunc);



void winMain(void);


void winTexInit(TPLHeader* texture);
void winTexSet(u32 textureId, Vec translate, Vec scale, GXColor color);
