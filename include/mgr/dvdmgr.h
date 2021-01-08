#pragma once

#include <dolphin/dvd.h>

typedef struct DVDEntry {
	char name[64]; //0x00
	DVDFileInfo info; //0x40
	void* address; //0x7C
	s32 bytesLeft; //0x80
	s32 field_0x84; //0x84
	s32 field_0x88; //0x88, "bytesRead"?
	void (*callback)(s32 error, DVDFileInfo* info); //0x8C
	u16 status; //0x90
	u16 field_0x92; //0x92
	u16 field_0x94; //0x94
	u16 field_0x96; //0x96, padding/alignment?
} DVDEntry;

void DVDMgrInit(void);
void DVDMgrMain(void);
void DVDMgrDelete(void);

DVDEntry* DVDMgrOpen(char* path, u8 a2, u16 a3);
void DVDMgrRead(DVDEntry* entry, void* address, s32 a3, s32 a4);
void DVDMgrReadAsync(DVDEntry* entry, void* address, s32 a3, s32 a4, void (*callback)(s32, DVDFileInfo*));
void DVDMgrClose(DVDEntry* entry);
u32 DVDMgrGetLength(DVDEntry* entry);
void DVDMgrSetupCallback(BOOL (*callback)(void));