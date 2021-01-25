#pragma once

#include <dolphin/types.h>

typedef struct WinMgrEntry {
	u8 field_0x0[0x44]; //0x0
} WinMgrEntry;

typedef struct WinMgrWork {
	u32 mNumEntries; //0x0
	WinMgrEntry* mEntries;
} WinMgrWork;

void winMgrInit(void);
void winMgrReInit(void);