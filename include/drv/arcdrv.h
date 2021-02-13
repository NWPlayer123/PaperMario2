#pragma once

#include <dolphin/types.h>
#include "sdk/arc.h"

typedef struct arcObj {
	u16 flags; //0x0
	//will pad 2 bytes
	ARCHandle handle; //0x4
	u32 field_0x20; //0x20
	u32 field_0x24; //0x24
} arcObj;


void arcInit(void);
void* arcOpen(char* filename, void** addr, u32* length);