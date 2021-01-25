#pragma once

#include <dolphin/types.h>

typedef struct romFontWork {
	void* field_0x0; //0x0
	u32 field_0x4; //0x4
	u32 mLanguage; //0x8, TODO different name? 0 = SJIS, 1 = ASCII
} romFontWork;


void romFontInit(void);
const char* romFontGetMessage(s32 msg);