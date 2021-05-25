#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_status_icon.h"
#include "drv/effdrv.h"

//TODO: better name
typedef enum BattleUnitToken_Flags {
	kChargeExpended = (1 << 3), //0x8
	kConfusedAction = (1 << 4), //0x10
	kCurrentlyActing = (1 << 12), //0x1000
	kHasRallyWinkStatus = (1 << 13) //0x2000
} BattleUnitToken_Flags;

typedef enum PartsAttribute_Flags { //32-bit size
	kMainPart = (1 << 0), //0x1
	kImportantPart = (1 << 1), //0x2
	kSemiImportantPart = (1 << 2), //0x4
	kUnknown_0x8 = (1 << 3), //0x8
	kUnknown_0x10 = (1 << 4), //0x10
	kUnknown_0x40 = (1 << 6), //0x40
	kWeakToAttackFxR = (1 << 7), //0x80
	kWeakToIcePower = (1 << 8), //0x100
	kIsWinged = (1 << 11), //0x800
	kIsShelled = (1 << 12), //0x1000
	kIsBombFlippable = (1 << 13), //0x2000
	kUnknown_0x4000 = (1 << 14), //0x4000
	kNotTargetable = (1 << 16), //0x10000
	kUnknown_0x40000 = (1 << 18), //0x40000
	kUntattleable = (1 << 19), //0x80000
	kJumplikeCannotTarget = (1 << 20), //0x100000
	kHammerlikeCannotTarget = (1 << 21), //0x200000
	kShellTosslikeCannotTarget = (1 << 22), //0x400000
	kUnknown_0x800000 = (1 << 23), //0x800000
	kUnk_ImmuneToDamageOrStatus = (1 << 29), //0x20000000
	kUnk_ImmageToOHKO = (1 << 30), //0x40000000
	kUnk_ImmuneToStatus = (1 << 31) //0x80000000
} PartsAttribute_Flags;

typedef enum BattleUnitAttribute_Flags { //32-bit size
	kUnknown_0x1 = (1 << 0), //0x1
	kCeiling = (1 << 1), //0x2
	kFloating = (1 << 2), //0x4
	kUnknownUnit_0x8 = (1 << 3), //0x8
	kVeiled = (1 << 4), //0x10
	kShellShielded = (1 << 5), //0x20
	kUnknownUnit_0x1000 = (1 << 12), //0x1000
	kUnknownUnit_0x2000 = (1 << 13), //0x2000
	kInactive = (1 << 17), //0x20000
	kUnknownUnit_0x2000000 = (1 << 25) //0x2000000
} BattleUnitAttribute_Flags;

struct BattleUnitKindPart {
	s32 mPartNum; //0x0
	u8 field_0x4[0x4C - 0x4]; //0x4
};

struct BattleWorkUnitPart {
	BattleWorkUnitPart* mNextPart; //0x0
	BattleUnitKindPart* mKindPartParams; //0x4
	char* mPartName; //0x8
	Vec mHomePosition; //0xC
	Vec mPosition; //0x18
	Vec mPositionOffset; //0x24
	Vec mDisplayOffset; //0x30
	Vec mBaseRotation; //0x3C
	Vec mRotation; //0x48
	Vec mRotationOffset; //0x54
	Vec mScale; //0x60
	Vec mBaseScale; //0x68
	f32 field_0x78; //0x78
	Vec mMoveStartPos; //0x7C
	Vec mMoveCurrentPos; //0x88
	Vec mMoveTargetPos; //0x94
	u8 field_0xA0[0xA4 - 0xA0]; //0xA0
	f32 mMoveSpeedXZ; //0xA4
	f32 mFallAccel; //0xA8
	u8 field_0xAC[0x170 - 0xAC]; //0xAC
	Vec mHitBaseOffset; //0x170, guess
	Vec mHitOffset; //0x17C
	Vec mHitCursorBaseOffset; //0x188, guess
	Vec mHitCursorOffset; //0x194
	u8 field_0x1A0[0x1AC - 0x1A0]; //0x1A0
	PartsAttribute_Flags mAttributes; //0x1AC
	u8 field_0x1B0[0x1BC - 0x1B0]; //0x1B0
	void* mPoseTable; //0x1BC
	u8 field_0x1C0[0x4EC - 0x1C0]; //0x1C0
	BattleWorkUnit* mOwner; //0x4EC
	u8 field_0x4F0[0x500 - 0x4F0]; //0x4F0
};

typedef struct BattleUnitKind {
	BattleUnitType mUnitType; //0x0
	u8 field_0x4[0xC - 0x4]; //0x4
	u8 mDangerHP; //0xC
	u8 mPerilHP; //0xD
	u8 mLevel; //0xE
	u8 mBonusExp; //0xF
	u8 field_0x10[0xB8 - 0x10]; //0x10
	BattleWorkUnitPart* mParts; //0xB8
	u8 field_0xBC[4]; //0xBC
	void* mDataTable; //0xC0
} BattleUnitKind;

typedef struct BattleUnitSetup {
	BattleUnitKind* mUnitKindParams; //0x0
	u8 field_0x4[0x30 - 0x4]; //0x4
} BattleUnitSetup;

typedef struct BattleWorkUnitBadgesEquipped {
	u8 field_0x0[0x17 - 0x0]; //0x0
	u8 mZapTap; //0x17
	u8 field_0x18[0x25 - 0x18]; //0x18
	u8 mSimplifier; //0x25
	u8 mUnsimplifier; //0x26
	u8 mAutoCommandBadge; //0x27
} BattleWorkUnitBadgesEquipped;

//note to self: +4 is near mUnk_HpGauge at 0x1FC
struct BattleWorkUnit {
	s32 mUnitId; //0x0
	BattleUnitType mInitialKind; //0x4
	BattleUnitType mCurrentKind; //0x8
	s8 mAlliance; //0xC
	u8 mLevel; //0xD
	u8 mGroupSetupIndex; //0xE
	u8 pad_0xF; //0xF
	BattleUnitKind* mKindParams; //0x10
	BattleWorkUnitPart* mParts; //0x14
	void* mDataTable; //0x18
	s32 mFlags; //0x1C
	u8 field_0x20[0x22 - 0x20]; //0x20
	s8 mMovesRemaining; //0x22
	s8 mMaxMovesThisTurn; //0x23
	u8 field_0x24[0x30 - 0x24]; //0x24
	Vec mHomePosition; //0x30
	Vec mPosition; //0x3C
	Vec mPositionOffset; //0x48
	Vec mDisplayOffset; //0x54
	Vec mBaseRotation; //0x60
	Vec mRotation; //0x6C
	Vec mRotationOffset; //0x78
	u8 field_0x84[0x90 - 0x84]; //0x84
	Vec mScale; //0x90
	Vec mBaseScale; //0x9C
	u8 field_0xA8[0xCC - 0xA8]; //0xA8
	s16 mWidth; //0xCC
	s16 mHeight; //0xCE
	s16 mStatusIconOffset[2]; //0xD0
	u8 field_0xD4[0x104 - 0xD4]; //0xD4
	BattleUnitAttribute_Flags mAttributeFlags; //0x104
	s16 mMaxHp; //0x108
	s16 mBaseMaxHp; //0x10A
	s16 mCurrentHp; //0x10C
	s16 mMaxFp; //0x10E
	s16 mBaseMaxFp; //0x110
	s16 mCurrentFp; //0x112
	f32 mSizeMultiplier; //0x114
	// Status Data ------------------------------------
	s8 mSleepTurns; //0x118
	s8 mSleepStrength; //0x119
	s8 mStopTurns; //0x11A
	s8 mDizzyTurns; //0x11B
	s8 mPoisonTurns; //0x11C
	s8 mPoisonStrength; //0x11D
	s8 mConfusionTurns; //0x11E
	s8 mElectricTurns; //0x11F
	s8 mDodgyTurns; //0x120
	s8 mBurnTurns; //0x121
	s8 mFreezeTurns; //0x122
	s8 mSizeChangeTurns; //0x123
	s8 mSizeChangeStrength; //0x124
	s8 mAtkChangeTurns; //0x125
	s8 mAtkChangeStrength; //0x126
	s8 mDefChangeTurns; //0x127
	s8 mDefChangeStrength; //0x128
	s8 mChargeStrength; //0x129
	s8 mAllergicTurns; //0x12A
	s8 mFlippedTurns; //0x12B (for shelled enemies)
	s8 mInvisibleTurns; //0x12C
	s8 mPaybackTurns; //0x12D
	s8 mHoldFastTurns; //0x12E
	s8 mFastTurns; //0x12F
	s8 mSlowTurns; //0x130
	s8 mHpRegenTurns; //0x131
	s8 mHpRegenStrength; //0x132
	s8 mFpRegenTurns; //0x133
	s8 mFpRegenStrength; //0x134
	s8 mInstantKillStrength; //0x135
	u8 pad_0x136[2]; //0x136, TODO remove?
	// ------------------------------------------------
	s32 mStatusFlags; //0x138
	u8 field_0x13C[0x144 - 0x13C]; //0x13C
	Vec mMoveStartPos; //0x144 JP, 0x148 US
	Vec mMoveCurrentPos; //0x150 JP, 0x154 US
	Vec mMoveTargetPos; //0x15C JP, 0x160 US
	u8 field_0x168[0x16C - 0x168]; //0x168
	f32 mMoveSpeedXZ; //0x16C JP, 0x170 US
	f32 mFallAccel; //0x170 JP, 0x174 US
	f32 mMoveSpeedY; //0x174 JP, 0x178 US
	u8 field_0x178[0x278 - 0x178]; //0x178
	BattleUnitToken_Flags mTokenFlags; //0x278, 0x27C in US
	u8 field_0x27C[0x2DC - 0x27C]; //0x27C
	BattleWorkUnitBadgesEquipped mBadgesEquipped; //0x2DC
	u8 field_0x304[0x30D - 0x304]; //0x304
	u8 mMoveColorLv;  //0x30D, 0x311 in US, TODO find extra data
	u8 field_0x30E[0x334 - 0x30E]; //0x30E
	s16 mBurnDamageFlameState; //0x334 JP, 0x338 US
	s16 mbBurnDamageFlameActive; //0x336 JP, 0x33A US
	s32 mBurnDamageFlameAnimTimer; //0x338 JP, 0x33C US
	f32 mBurnDamageFlameScale; //0x33C JP, 0x340 US
	EffEntry* mBurnDamageFlameEff; //0x340 JP, 0x344 US
	BattleWorkStatusIcon mStatusIconWork; //0x344 JP, 0x348 US
	u8 field_0xAE4[0xB30 - 0xAE4]; //0xAE4
};

BOOL BtlUnit_Init(void);
BattleWorkUnit* BtlUnit_Entry(BattleUnitSetup* setup);
BOOL BtlUnit_Delete(BattleWorkUnit* unit);
BattleWorkUnit* BtlUnit_Spawn(BattleUnitSetup* setup, s32 flags);
s32 BtlUnit_GetUnitId(BattleWorkUnit* unit);
BattleWorkUnitPart* BtlUnit_GetPartsPtr(BattleWorkUnit* unit, s32 partNum);
s32 BtlUnit_GetBodyPartsId(BattleWorkUnit* unit);
void BtlUnit_GetPos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsWorldPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_GetHitPos(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetHitOffset(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetHitCursorOffset(BattleWorkUnit* unit, BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetHomePos(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetHomePos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddHomePos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsHomePos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetRotate(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_AddRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsRotate(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_AddPartsRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetBaseRotate(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetBaseRotate(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsBaseRotate(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsBaseRotate(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetRotateOffset(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsRotateOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsRotateOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetBaseScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetScale(BattleWorkUnit* unit, f32* x, f32* y, f32* z);
void BtlUnit_SetScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_AddScale(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsBaseScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsScale(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
s16 BtlUnit_GetWidth(BattleWorkUnit* unit);
s16 BtlUnit_GetHeight(BattleWorkUnit* unit);
void BtlUnit_SetHeight(BattleWorkUnit* unit, s16 height);
void BtlUnit_SetOffsetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_GetPartsOffsetPos(BattleWorkUnitPart* part, f32* x, f32* y, f32* z);
void BtlUnit_SetPartsOffsetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsOffsetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetDispOffset(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsDispOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_AddPartsDispOffset(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetMoveStartPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsMoveStartPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetMoveCurrentPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsMoveCurrentPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetMoveTargetPos(BattleWorkUnit* unit, f32 x, f32 y, f32 z);
void BtlUnit_SetPartsMoveTargetPos(BattleWorkUnitPart* part, f32 x, f32 y, f32 z);
void BtlUnit_SetFallAccel(BattleWorkUnit* unit, f32 fallAccel);
void BtlUnit_SetPartsFallAccel(BattleWorkUnitPart* part, f32 fallAccel);
void BtlUnit_SetMoveSpeed(BattleWorkUnit* unit, f32 moveSpeedXZ);
void BtlUnit_SetPartsMoveSpeed(BattleWorkUnitPart* part, f32 moveSpeedXZ);
void BtlUnit_SetJumpSpeed(BattleWorkUnit* unit, f32 moveSpeedY);
s8 BtlUnit_GetBelong(BattleWorkUnit* unit);
void BtlUnit_GetStatus(BattleWorkUnit* unit, StatusEffectType type, s8* turns, s8* strength);
BOOL BtlUnit_SetStatus(BattleWorkUnit* unit, StatusEffectType type, s8 turns, s8 strength);
void BtlUnit_ClearStatus(BattleWorkUnit* unit);
BOOL BtlUnit_CheckRecoveryStatus(BattleWorkUnit* unit, StatusEffectType type);
BOOL BtlUnit_CheckStatus(BattleWorkUnit* unit, StatusEffectType type);
s32 BtlUnit_CheckStatusFlag(BattleWorkUnit* unit, s32 flags);
void BtlUnit_OnStatusFlag(BattleWorkUnit* unit, s32 flags);
void BtlUnit_OffStatusFlag(BattleWorkUnit* unit, s32 flags);
s32 BtlUnit_CheckUnitFlag(BattleWorkUnit* unit, s32 flags);
void BtlUnit_OnUnitFlag(BattleWorkUnit* unit, s32 flags);
void BtlUnit_OffUnitFlag(BattleWorkUnit* unit, s32 flags);
char* BtlUnit_GetPoseNameFromType(BattleWorkUnitPart* part, s32 type);














void BtlUnit_SetParamToPouch(BattleWorkUnit* unit);





void BtlUnit_ResetMoveStatus(BattleWorkUnit* unit);













