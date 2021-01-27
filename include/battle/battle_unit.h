#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"

struct BattleUnitKindPart {
	s32 mPartNum; //0x0
	u8 field_0x4[0x4C - 0x4]; //0x4
};

struct BattleWorkUnitPart {
	BattleWorkUnitPart* mNextPart; //0x0
	BattleUnitKindPart* mKindPartParams; //0x4
	u8 field_0x8[0x500 - 0x8]; //0x8
};

struct BattleWorkUnit {
	s32 mUnitId; //0x0
	BattleUnitType mInitialKind; //0x4
	BattleUnitType mCurrentKind; //0x8
	s8 mAlliance; //0xC
	u8 mLevel; //0xD
	u8 mGroupSetupIndex; //0xE
	u8 pad_0xF; //0xF
	u32 mKindParams; //0x10
	BattleWorkUnitPart* mParts; //0x14
	void* mDataTable; //0x18
	u32 field_0x1C; //0x1C
	u8 field_0x20[0x30D - 0x20]; //0x20
	u8 mMoveColorLv;  //0x30D, 0x311 in US, TODO find extra data
	u8 field_0x30E[0xB30 - 0x30E]; //0x30E
};

BOOL BtlUnit_Init(void);
BOOL BtlUnit_Delete(BattleWorkUnit* unit);
void BtlUnit_SetParamToPouch(BattleWorkUnit* unit);
BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit* unit, s32 partNum);