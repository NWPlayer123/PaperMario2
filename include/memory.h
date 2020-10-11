#pragma once

#include <dolphin/types.h>
#include <dolphin/os.h>
#include <string.h>

typedef struct smartEntry smartEntry;
typedef struct smartWork_s smartWork_s;

void memInit(void);
void* __memAlloc(u32 heap, u32 size);

struct smartEntry {
	u32 field_0x0; //0x00
	u32 size; //0x04
	u32 field_0x8; //0x08
	u16 field_0xC; //0x0C
	u8  type; //0x0E, total guess, no way to know
	u8  ; //0xF
	u32 field_0x10; //0x10
	smartEntry* next; //0x14
	smartEntry* prev; //0x18
};

struct smartWork_s {
	void* ptr; //0x00
	smartEntry work[0x800]; //0x4-0xE004
	u32 bytesLeft; //0xE004
	smartEntry* field_0xE008; //0xE008
	smartEntry* field_0xE00C; //0xE00C
	smartEntry* head; //0xE010
	smartEntry* tail; //0xE014
	u32 waitsync; //0xE018
};