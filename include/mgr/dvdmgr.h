#pragma once

#include <dolphin/dvd.h>

typedef enum DVDFlags {
	DVDMGR_READING = (1 << 0),
	DVDMGR_FINISHED = (1 << 1),
	DVDMGR_CLOSED = (1 << 3),
	DVDMGR_UNK_ERROR1 = (1 << 5),
	DVDMGR_UNK_ERROR2 = (1 << 7),
	DVDMGR_INUSE = (1 << 15)
} DVDFlags;

typedef struct DVDEntry {
	char name[64]; //0x0
	DVDFileInfo info; //0x40
	void* address; //0x7C
	u32 bytesLeft; //0x80
	u32 offset; //0x84
	u32 position; //0x88
	void (*callback)(s32 error, DVDFileInfo* info); //0x8C
	u16 status; //0x90
	u16 priority; //0x92
	u16 unknown; //0x94
	u8 pad_96[2]; //0x96
} DVDEntry;

void DVDMgrInit(void);
void DVDMgrDelete(void);
void DVDMgrMain(void);
DVDEntry* DVDMgrOpen(const char* path, u32 priority, u16 unknown);
void DVDMgrRead(DVDEntry* entry, void* address, u32 size, u32 offset);
void DVDMgrReadAsync(DVDEntry* entry, void* address, u32 size, u32 offset, void (*callback)(s32, DVDFileInfo*));
void DVDMgrClose(DVDEntry* entry);
u32 DVDMgrGetLength(DVDEntry* entry);
void DVDMgrSetupCallback(BOOL (*callback)(void));
