#pragma once

#include <dolphin/dvd.h>
#include "texPalette.h"

typedef struct EffEntry {
	u8 field_0x0[0x28 - 0x0]; //0x0
} EffEntry;

typedef struct effdrv_work {
	u32 numEntries; //0x0
	EffEntry* entries; //0x4
	TPLHeader* effTexture; //0x8
	BOOL effTextureLoaded; //0xC
	void* field_0x10; //0x10
	s32 field_0x14; //0x14
} effdrv_work;

void effInit(void);