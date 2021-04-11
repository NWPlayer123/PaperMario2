#include "battle/battle_unit.h"

BOOL BtlUnit_Init(void) {
    return TRUE;
}

BOOL BtlUnit_Delete(BattleWorkUnit* unit) {
    return TRUE;
}


void BtlUnit_SetParamToPouch(BattleWorkUnit* unit) {

}

BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit* unit, s32 partNum) {
    BattleWorkUnitPart* part; // r3

    part = unit->mParts;
    if (partNum >= 0 && part->mNextPart) {
        while (part && part->mKindPartParams->mPartNum != partNum) {
            part = part->mNextPart;
        }
    }
    return part;
}




void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z) {
    *x = unit->mPosition.x;
    *y = unit->mPosition.y;
    *z = unit->mPosition.z;
}

void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    unit->mPosition.x = x;
    unit->mPosition.y = y;
    unit->mPosition.z = z;
}

void BtlUnit_AddPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z) {
    unit->mPosition.x += x;
    unit->mPosition.y += y;
    unit->mPosition.z += z;
}

void BtlUnit_GetPartsPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z) {
    *x = part->mPosition.x;
    *y = part->mPosition.y;
    *z = part->mPosition.z;
}

void BtlUnit_SetPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->mPosition.x = x;
    part->mPosition.y = y;
    part->mPosition.z = z;
}

void BtlUnit_AddPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z) {
    part->mPosition.x += x;
    part->mPosition.y += y;
    part->mPosition.z += z;
}








s16 BtlUnit_GetWidth(BattleWorkUnit* unit) {
    return unit->mWidth;
}

s16 BtlUnit_GetHeight(BattleWorkUnit* unit) {
    return unit->mHeight;
}









