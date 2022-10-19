#pragma once

#include <dolphin/mtx.h>
#include "drv/mapdrv.h"

typedef BOOL (*HitFilterCallback)(struct HitCheckQuery* query, struct HitEntry* entry);

typedef struct HitVector {
    Vec unk0; //0x0
    Vec unkC; //0xC
    Vec unk18; //0x18
    Vec unk24; //0x24
    Vec unk30; //0x30
    Vec unk3C; //0x3C
    Vec unk48; //0x48
} HitVector;

typedef struct HitEntry {
	u16 flags; //0x0
    u8 unk2[0x4 - 0x2]; //0x2
    s32 unk4; //0x4
    MapJoint* joint; //0x8
    u8 unkC[0xA8 - 0xC]; //0xC
    s16 unkA8; //0xA8
    u8 unkAA[0xAC - 0xAA]; //0xAA
    HitVector* unkAC; //0xAC
    u8 unkB0[0xC0 - 0xB0]; //0xB0
    Vec unkC0; //0xC0
    f32 unkCC; //0xCC
    MapObject* mapObj; //0xD0
    u8 unkD4[0xE0 - 0xD4]; //0xD4
    struct HitEntry* next; //0xE0
} HitEntry;

typedef struct HitCheckQuery {
    BOOL singleSided; //0x0
    s32 user0; //0x4
    s32 user1; //0x8
    Vec targetPos; //0xC
    Vec targetDir; //0x18
    Vec hitPos; //0x24
    Vec hitNormal; //0x30
    f32 targetDistance; //0x3C
} HitCheckQuery;

void hitInit(void);
void hitMain(void);





HitEntry* hitNameToPtr(const char* name);

HitEntry* hitCheckVecFilter(HitCheckQuery* query, HitFilterCallback callback);
HitEntry* hitCheckFilter(HitFilterCallback callback, f32* hitX, f32* hitY, f32* hitZ, f32* distance,
						f32* hitNX, f32* hitNY, f32* hitNZ, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz);
BOOL hitCheckVecHitObjXZ(HitCheckQuery* query, HitEntry* entry);
HitEntry* hitCheckAttr(s32 user0, f32* hitX, f32* hitY, f32* hitZ, f32* distance, f32* hitNX,
					  f32* hitNY, f32* hitNZ, f32 x, f32 y, f32 z, f32 nx, f32 ny, f32 nz);

