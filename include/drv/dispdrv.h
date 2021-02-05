#pragma once

#include <dolphin/mtx.h>
#include <dolphin/mtx/mtx44ext.h>

typedef struct DispEntry {
	u8 field_0x0; //0x0
	u8 field_0x1; //0x1
	u8 pad_2[2]; //0x2, TODO see if unused
	f32 field_0x4; //0x4
	void (*mCallback)(u32, void*); //0x8, TODO re-type for exact func call
	void* field_0xC; //0xC, param?
} DispEntry;

void dispInit(void);
void dispReInit(void);
void dispEntry(u8 a1, u8 a2, void (*callback)(u32, void*), void* a4, f32 a5);
void dispSort(void);
void dispCalcZ(Vec* input);




void dispDraw(u32 camNo);