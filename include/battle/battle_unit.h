#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"

//TODO: better name
typedef enum BattleUnitToken_Flags {
	kChargeExpended = (1 << 3), //0x8
	kConfusedAction = (1 << 4), //0x10
	kCurrentlyActing = (1 << 12), //0x1000
	kHasRallyWinkStatus = (1 << 13) //0x2000
} BattleUnitToken_Flags;

struct BattleUnitKindPart {
	s32 mPartNum; //0x0
	u8 field_0x4[0x4C - 0x4]; //0x4
};

struct BattleWorkUnitPart {
	BattleWorkUnitPart* mNextPart; //0x0
	BattleUnitKindPart* mKindPartParams; //0x4
	u8 field_0x8[0x18 - 0x8]; //0x8
	Vec mPosition; //0x18
	u8 field_0x24[0x500 - 0x24]; //0x24
};

typedef struct BattleWorkUnitBadgesEquipped {
	u8 field_0x0[0x27 - 0x0]; //0x0
	u8 mAutoCommandBadge; //0x27
} BattleWorkUnitBadgesEquipped;

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
	u8 field_0x20[0x3C - 0x20]; //0x20
	Vec mPosition; //0x3C
	u8 field_0x48[0xCC - 0x48]; //0x48
	s16 mWidth; //0xCC
	s16 mHeight; //0xCE
	u8 field_0xD0[0x278 - 0xD0]; //0xD0
	BattleUnitToken_Flags mTokenFlags; //0x278, 0x27C in US
	u8 field_0x27C[0x2DC - 0x27C]; //0x27C
	BattleWorkUnitBadgesEquipped mBadgesEquipped; //0x2DC
	u8 field_0x304[0x30D - 0x304]; //0x304
	u8 mMoveColorLv;  //0x30D, 0x311 in US, TODO find extra data
	u8 field_0x30E[0xB30 - 0x30E]; //0x30E
};

BOOL BtlUnit_Init(void);
BOOL BtlUnit_Delete(BattleWorkUnit* unit);
void BtlUnit_SetParamToPouch(BattleWorkUnit* unit);
BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit* unit, s32 partNum);









void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);


s16 BtlUnit_GetWidth(BattleWorkUnit* unit);
s16 BtlUnit_GetHeight(BattleWorkUnit* unit);





