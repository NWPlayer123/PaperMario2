#pragma once

#include <dolphin/types.h>
#include "texPalette.h"

typedef struct eff_n64_work {
	TPLHeader* effTexture; //0x0
	BOOL effTextureLoaded; //0x4
} eff_n64_work;

typedef struct eff_set {
	s16 id; //0x0
	//will pad to 2 bytes
	char* name; //0x4
} eff_set;

void effInit64(void);
void effTexSetupN64(void);
void effGetTexObjN64(u32 id, GXTexObj* obj);
eff_set* effGetSetN64(const char* name);

void effSetVtxDescN64(void* array);