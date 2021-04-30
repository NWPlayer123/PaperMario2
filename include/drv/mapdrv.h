#pragma once

#include <dolphin/types.h>

typedef struct MapError {
	u8 field_0x0[0x24 - 0x0]; //0x0
} MapError;

typedef struct MapObj {
	u8 field_0x0[0x134 - 0x0]; //0x0
} MapObj;

typedef struct MapEntry {
	u8 field_0x0[0x178 - 0x0]; //0x0
} MapEntry;

typedef struct MapWorkGroup {
	MapEntry entry[2]; //0x0
	s32 field_0x2F0; //0x2F0
} MapWorkGroup;

typedef struct MapWork {
	MapWorkGroup group[2]; //0x5E8
} MapWork;

void mapInit(void);
void mapMain(void);
void mapDisp(void);