#pragma once

#include <dolphin/types.h>

typedef struct ItemEntry {
	u8 field_0x0[0x98 - 0x0]; //0x0
} ItemEntry;

typedef struct ItemWork {
	s32 count; //0x0
	ItemEntry* entries; //0x4
	u8 field_0x8[0x1C - 0x8]; //0x8
} ItemWork;

void itemInit(void);
void itemMain(void);