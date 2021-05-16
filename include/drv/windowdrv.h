#pragma once

#include <dolphin/gx.h>

typedef struct WindowEntry WindowEntry;

struct WindowEntry {
	u8 field_0x0[2]; //0x0
	u16 flags; //0x2
	u16 field_0x4; //0x4
	u8 field_0x6[0x28 - 0x6]; //0x6
	u32 field_0x28; //0x28
	u8 field_0x2C[0x3C - 0x2C]; //0x2C
	void (*main)(WindowEntry* entry); //0x3C
	void (*delete)(WindowEntry* entry); //0x40
	u8 field_0x44[0x48 - 0x44]; //0x44
};

void windowInit(void);
void windowTexSetup(void);
void windowReInit(void);
s32 windowEntry(u16 a1);
BOOL windowDelete(WindowEntry* entry);
BOOL windowDeleteID(s32 id);
void windowMain(void);
void windowDispGX_Kanban(s32 type, u8 alpha, f32 x, f32 y, f32 height, f32 width);




s32 windowCheckID(s32 id);
WindowEntry* windowGetPointer(s32 id);
GXTexObj* getWakuTexObj(s32 id);