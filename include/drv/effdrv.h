#pragma once

#include <dolphin/dvd.h>
#include "mgr/filemgr.h"
#include "texPalette.h"

typedef struct EffEntry EffEntry;

struct EffEntry {
	u32 flags; //0x0
	u32 inBattle; //0x4
	u32 effCount; //0x8
	void* field_0xC; //0xC
	void (*field_0x10)(EffEntry* effect); //0x10
	u32 field_0x14; //0x14
	char name[16]; //0x18
};

typedef struct effdrv_work {
	s32 numEntries; //0x0
	EffEntry* entries; //0x4
	TPLHeader* effTexture; //0x8
	BOOL effTextureLoaded; //0xC
	fileObj* handle; //0x10
	s32 language; //0x14
} effdrv_work;

void effInit(void);
void effTexSetup(void);
void effGetTexObj(u32 id, GXTexObj* obj);
void effAutoRelease(BOOL inBattle);
EffEntry* effEntry(void);
void effSetName(EffEntry* effect, const char* name);
void effMain(void);
void effDelete(EffEntry* effect);
void effSoftDelete(EffEntry* effect);










