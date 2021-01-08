#pragma once

#include <dolphin/types.h>

typedef struct seqdrv_work {
	s32 field_0x0; //0x0, seq_num
	s32 field_0x4; //0x4, always 0?
	s32 field_0x8; //0x8, "next_p0"
	s32 field_0xC; //0xC, "next_p1"
	u32 field_0x10; //0x10
	u32 field_0x14; //0x14
	u32 field_0x18; //0x18
	u32 field_0x1C; //0x1C
} seqdrv_work;

void seqInit_MARIOSTORY(void);
void seqMain(void);
void seqSetSeq(s32 seq, s32 p0, s32 p1);
s32 seqGetSeq(void);
s32 seqGetPrevSeq(void);
s32 seqGetNextSeq(void);
BOOL seqCheckSeq(void);