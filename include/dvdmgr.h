#pragma once

#include <dolphin/dvd.h>
#include <dolphin/os.h>
#include <string.h>

typedef struct dvdq_entry {
	char fileName[64];
	DVDFileInfo fileInfo; //0x40
	u32 field_0x7C; //0x7C
	u32 field_0x80; //0x80
	u32 field_0x84; //0x84
	u32 field_0x88; //0x88
	void* callback; //0x8C
	u16 status; //0x90
	u16 field_0x92; //0x92
	u16 field_0x94; //0x94
	u16 field_0x96; //0x96, padding/alignment?
} dvdq_entry;