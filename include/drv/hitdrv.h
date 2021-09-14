#pragma once

#include <dolphin/mtx.h>
#include "drv/mapdrv.h"

typedef struct HitEntry {
	u16 flags; //0x0
	u8 field_0x2[0x8 - 0x2]; //0x2
	MapFileJoint* joint; //0x8
	u8 field_0xC[0xE4 - 0xC]; //0xC
} HitEntry;

void hitInit(void);
void hitMain(void);





HitEntry* hitNametoPtr(const char* name);