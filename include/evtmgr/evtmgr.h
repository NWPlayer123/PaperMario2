#pragma once

#include <dolphin/types.h>
#include <string.h>
#include "drv/swdrv.h"
#include "mariost.h"

typedef struct evtmgr_entry {
	u32 field_0x0;
	u32 field_0x4[32];
	u32 field_0x84[3];
	void* field_0x90;
	u32 field_0x94;
	u32 field_0x98;
	u32 field_0x9C;
} evtmgr_entry;

evtmgr_entry* evtGetWork(void);