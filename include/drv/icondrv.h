#pragma once

#include <dolphin/mtx.h>

typedef struct IconEntry {
	s16 flags; //0x0
	u8 field_0x2; //0x2, padding?
	Vec position; //0x4
	f32 scale; //0x10
	u8 field_0x14[0x18 - 0x14]; //0x14
	char name[16]; //0x18
	s16 field_0x28; //0x28
	u8 field_0x2A[4]; //0x2A, padding + value?
	s16 field_0x2E; //0x2E
	s16 field_0x30; //0x30
	u8 field_0x32[2]; //0x32, padding?
	s16 field_0x34; //0x34
	u8 field_0x36[2]; //0x36, padding?
} IconEntry;

typedef struct IconWork {
	s32 count; //0x0
	IconEntry* entries; //0x4
} IconWork;

void iconInit(void);
void iconTexSetup(void);
void iconReInit(void);
void iconMain(void);



void iconDelete(const char* name);
