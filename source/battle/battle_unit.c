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