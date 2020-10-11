#pragma once

#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <string.h>

typedef struct dvdq_entry {
	char fileName[64];
	DVDFileInfo fileInfo; //0x40
	u32 address; //0x7C
	u32 bytesLeft; //0x80
	u32 field_0x84; //0x84
	u32 bytesRead; //0x88
	void (*callback)(s32 err, DVDFileInfo fileInfo); //0x8C
	u16 status; //0x90
	u16 field_0x92; //0x92
	u16 field_0x94; //0x94
	u16 field_0x96; //0x96, padding/alignment?
} dvdq_entry;

//TODO: undefine some e.g. Main
void DVDMgrInit(void);
void DVDMgrDelete(void);
void DVDMgrMain(void);
dvdq_entry* DVDMgrOpen(char* path, u8 r4, u16 r5);
void DVDMgrRead(dvdq_entry* entry, void* address, u32 bytesLeft, u32 r6);
void DVDMgrReadAsync(dvdq_entry* entry, void* address, u32 bytesLeft, u32 r6, void* callback);
void DVDMgrClose(dvdq_entry* entry);
u32 DVDMgrGetLength(dvdq_entry* entry);
void DVDMgrSetupCallback(void* callback);