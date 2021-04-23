#pragma once

#include "mgr/dvdmgr.h"
#include <dolphin/types.h>
typedef struct fileObj fileObj;

//TODO: better name?
struct fileObj {
	u8 state; //0x0
	u8 field_0x1; //0x1, TODO enum archivetype/type
	u16 references; //0x2
	void* field_0x4; //0x4
	u8 field_0x8[0x20 - 0x8]; //0x8
	char filename[64]; //0x20
	u8 field_0x60[0xA0 - 0x60]; //0x60
	void** mppFileData; //0xA0
	fileObj* next; //0xA4
	void (*callback)(fileObj* file); //0xA8
	DVDEntry* entry; //0xAC
};

typedef struct filemgrWork {
	fileObj* entries; //0x0
	s32 mCurrentArchiveType; //0x4
	fileObj* firstused; //0x8
	fileObj* lastused; //0xC
	fileObj* firstavailable; //0x10
	fileObj* lastavailable; //0x14
} filemgrWork;

void fileInit(void);
void fileGarbageMoveMem(void* data, fileObj* file);
void _fileGarbage(BOOL a1);
fileObj* fileAllocf(u8 type, const char* format, ...);
fileObj* fileAlloc(const char* filename, u8 type);
void fileFree(fileObj* handle);
s32 fileAsyncf(u8 type, void (*callback)(fileObj*), const char* format, ...);
void fileSetCurrentArchiveType(s32 type);