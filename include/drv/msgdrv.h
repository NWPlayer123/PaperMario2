#pragma once

#include <dolphin/types.h>
#include "memory.h"

typedef struct MessageEntry {
	u32 length; //0x0
	void* address; //0x4
	u32 count; //0x8
	smartEntry* lookup; //0xC
} MessageEntry;

typedef struct MessageWork {
	MessageEntry entries[2]; //0x0
	s32 poseId; //0x20
} MessageWork;

void msgInit(void);
void msgLoad(const char* filename, s32 index);


MessageWork* msgGetWork(void);

const char* msgSearch(const char* tag);
BOOL _ismbblead(u32 a1);