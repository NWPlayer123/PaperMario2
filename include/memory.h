#pragma once

#include <dolphin/gx.h>
#include "mgr/filemgr.h"

typedef struct smartEntry smartEntry;

typedef enum mMemHeaps {
	HEAP_DEFAULT = 0,
	HEAP_MALLOC = 1, //for memInit/Clear to do its allocation
	HEAP_EXT = 2, //TODO: proper name for EXT
	HEAP_EFFECT = 3,
	HEAP_BATTLE = 4,
	HEAP_UNK5 = 5
} mMemHeaps;

struct smartEntry {
	void* address; //0x00
	u32 size; //0x04
	fileObj* field_0x8; //0x08
	u16 field_0xC; //0x0C
	u8  type; //0x0E, total guess, no way to know
	u8  field_0xF; //0xF
	u32 field_0x10; //0x10
	smartEntry* next; //0x14
	smartEntry* prev; //0x18
};

typedef struct smartWork_s {
	void* ptr; //0x00
	smartEntry work[0x800]; //0x4-0xE004
	u32 bytesLeft; //0xE004
	smartEntry* field_0xE008; //0xE008
	smartEntry* field_0xE00C; //0xE00C
	smartEntry* head; //0xE010
	smartEntry* tail; //0xE014
	u32 waitsync; //0xE018
} smartWork_s;

void memInit(void);
void* __memAlloc(u32 heap, u32 size);
void __memFree(u32 heap, void* ptr);

smartEntry* smartAlloc(u32 size, u8 type);

GXTexObj* smartTexObj(GXTexObj* obj, void** image_ptr);

void smartReInit(void);





void _mapFree(OSModuleInfo* module);

void* _mapAlloc(u32 size);

void smartInit(void);