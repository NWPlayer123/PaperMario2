#pragma once

#include <dolphin/types.h>

typedef struct CardSave {
	char game_string[0x20]; //0x0
	char date_string[0x20]; //0x20
	u8 banner_tex[0x1800]; //0x40
	u8 icon_tex[0x400]; //0x1840
	u8 icon_tlut[0x200]; //0x1C40
	u8 field_0x1E40[0x1E46 - 0x1E40]; //0x1E40
} CardSave;

typedef struct CardWork {
	u8 field_0x0[0xA8 - 0x0]; //0x0
	CardSave* save; //0xA8
	u8 field_0xAC[0xF0 - 0xAC]; //0xAC
} CardWork;

void cardInit(void);



BOOL cardIsExec(void);

void cardMain(void);