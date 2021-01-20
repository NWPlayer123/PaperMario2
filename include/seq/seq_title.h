#pragma once

#include <dolphin/types.h>
#include "drv/seqdrv.h"
#include "mgr/filemgr.h"

typedef struct seq_title_work {
	fileObj* handle; //0x0
	u8 field_0x4[0x30 - 0x4]; //0x4
	s32 debug_mode; //0x30
	u8 field_0x34[0x38 - 0x34]; //0x34
} seq_title_work;




void seq_titleInit(seqdrv_work* work);
void seq_titleMain(seqdrv_work* work);
void seq_titleExit(seqdrv_work* work);


char* DbgBtlSel_GetMsgDataPtr(void);
s32 getDebugMode(void);