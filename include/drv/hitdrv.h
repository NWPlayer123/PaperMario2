#pragma once

#include <dolphin/mtx.h>
#include "drv/mapdrv.h"

typedef struct HitEntry {
	u8 field_0x0[0xE4 - 0x0]; //0x0
} HitEntry;

void hitInit(void);
void hitMain(void);