#pragma once

#include "texPalette.h"
#include <dolphin/gx.h>
#include <dolphin/mtx.h>

typedef enum FadeType {
	FADE_IN_BLACK = 9,
	FADE_OUT_BLACK = 10
} FadeType;

typedef struct FadeEntry {
	u16 flags; //0x0, 4 = use virtual pos
	u8 pad_2[2]; //0x2
	u32 type; //0x4, FadeType
	u8 field_0x8[0x1C - 0x8]; //0x8
	f32 FadeSpotX; //0x1C
	f32 FadeSpotY; //0x20
	u8 field_0x24[0x6C - 0x24]; //0x24
	s32 field_0x6C; //0x6C
	s32 field_0x70; //0x70
	u8 field_0x74[0x78 - 0x74]; //0x74
	f32 AnimOffsetX; //0x78
	f32 AnimOffsetY; //0x7C
	Vec VirtualPos; //0x80
	u32 field_0x8C; //0x8C, TODO re-type
	f32 field_0x90; //0x90
	u8 field_0x94[0x98 - 0x94]; //0x94
	void (*OffscreenCallback)(void* param, void* unused); //0x9C
	void* OffscreenParam; //0xA0
	u32 field_0xA4; //0xA4
} FadeEntry;

typedef struct FadeWork {
	u16 flags; //0x0
	u8 pad_2[2]; //0x2
	u32 field_0x4; //0x4
	FadeEntry entries[5]; //0x8
	s32 field_0x350; //0x350
	f32 field_0x354; //0x354, softness? fadeTecSoftOn
	TPLHeader* tpl_header; //0x358
	BOOL tpl_loaded; //0x35C
} FadeWork;

void fadeInit(void);
void fadeTexSetup(void);
void fadeEntry(s32 type, s32 duration, GXColor color);
void fadeTecOn(void);
void fadeTecOff(void);
void fadeTecSoftOn(void);
void fadeTecSoftOff(void);
void fadeSoftFocusOn(void);
void fadeSoftFocusOff(void);
void fadeWhiteOutOn(void);
void fadeWhiteOutOff(void);
void fadeSetSpotPos(f32 x, f32 y);
void fadeSetAnimOfsPos(f32 x, f32 y);
void fadeSetOffscreenCallback(void (*callback)(void*, void*), void* param);
void fadeMain(void);
BOOL fadeIsFinish(void);
void fadeReset(s32 id);
TPLHeader* fadeGetTpl(void);
void fadeSetNarrowFast(void);
