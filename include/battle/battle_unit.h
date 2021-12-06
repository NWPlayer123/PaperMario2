#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"
#include "battle/battle_damage.h"
#include "battle/battle_status_effect.h"
#include "battle/battle_status_icon.h"
#include "drv/effdrv.h"

#define UNIT_ALLIANCE_ENEMY (unit->mAlliance == 0)
#define UNIT_ALLIANCE_PARTY (unit->mAlliance == 1)

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

typedef enum PartsCounterAttribute_Flags { //32-bit size
	kCounterTopSpiky = (1 << 0), //0x1
	kCounterPreemptiveFrontSpiky = (1 << 1), //0x2
	kCounterFrontSpiky = (1 << 2), //0x4
	kCounterFiery = (1 << 4), //0x10
	kCounterFieryStatus = (1 << 5), //0x20
	kCounterIcy = (1 << 6), //0x40
	kCounterIcyStatus = (1 << 7), //0x80
	kCounterPoison = (1 << 8), //0x100
	kCounterPoisonStatus = (1 << 9), //0x200
	kCounterElectric = (1 << 10), //0x400
	kCounterElectricStatus = (1 << 11), //0x800
	kCounterExplosive = (1 << 12), //0x1000
	kCounterVolatileExplosive = (1 << 13) //0x2000
} PartsCounterAttribute_Flags;

typedef struct BattleDataEntry {
	s32 tag; //0x0, TODO: rename?
	void* data; //0x4, TODO: rename?
} BattleDataEntry;

//TODO: move/rename?
typedef struct BattleUnitDefense {
	u8 defenses[5]; //0x0
} BattleUnitDefense;

//TODO: move/rename?
typedef struct BattleUnitDefenseAttr {
	u8 defenseAttrs[5]; //0x0
} BattleUnitDefenseAttr;

struct BattleUnitKindPart {
	s32 partNum; //0x0
	const char* partName; //0x4
	const char* modelName; //0x8
	Vec positionOffset; //0xC, TODO: shorter name?
	Vec hitBaseOffset; //0x18, TODO: better name?
	u8 field_0x24[0x38 - 0x24]; //0x24
	BattleUnitDefense* defense; //0x38
	BattleUnitDefenseAttr* defenseAttr; //0x3C
	PartsAttribute_Flags attributes; //0x40
	PartsCounterAttribute_Flags counterAttributes; //0x44
	void* poseTable; //0x48
};

typedef struct BattleWorkUnitPartBlurEntry {
	s32 flags; //0x0
	Mtx orientation; //0x4
	f32 rotation; //0x34
	GXColor base; //0x38
	s8 blurColor[8]; //0x3C, TODO: blur?
} BattleWorkUnitPartBlurEntry;

typedef struct BattleWorkUnitPartBlur {
	s32 flags; //0x0
	s8 color[8]; //0x4
	BattleWorkUnitPartBlurEntry work[10]; //0xC
} BattleWorkUnitPartBlur;

struct BattleWorkUnitPart {
	BattleWorkUnitPart* mNextPart; //0x0
	BattleUnitKindPart* mKindPartParams; //0x4
	const char* partName; //0x8
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
	u8 field_0xAC[0xBC - 0xAC]; //0xAC
	s8 moveDirection; //0xBC
	s8 faceDirection; //0xBD
	u8 field_0xBE[0x130 - 0xBE]; //0xBE
	s32 work[16]; //0x130
	Vec mHitBaseOffset; //0x170, guess
	Vec mHitOffset; //0x17C
	Vec mHitCursorBaseOffset; //0x188, guess
	Vec mHitCursorOffset; //0x194
	s16 field_0x1A0; //0x1A0
	u8 field_0x1A2[0x1AC - 0x1A2]; //0x1A2
	PartsAttribute_Flags attributes; //0x1AC
	PartsCounterAttribute_Flags counterAttributes; //0x1B0
	BattleUnitDefense* defense; //0x1B4
	BattleUnitDefenseAttr* defenseAttr; //0x1B8
	void* poseTable; //0x1BC
	u8 field_0x1C0[0x21C - 0x1C0]; //0x1C0
	BattleWorkUnitPartBlur blurWork; //0x21C
	u8 field_0x4D0[0x4EC - 0x4D0]; //0x4D0
	BattleWorkUnit* mOwner; //0x4EC
	GXColor color; //0x4F0
	u8 field_0x4F4[0x500 - 0x4F4]; //0x4F4
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
	BattleDataEntry* dataTable; //0xC0
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

typedef struct BattleWorkHpGauge { //TODO: diff header file?
	u8 field_0x0[2]; //0x0
	u16 field_0x2; //0x2, TODO: double check u16
	s16 field_0x4; //0x4
	s16 field_0x6; //0x6
	s16 field_0x8; //0x8
	s16 field_0xA; //0xA
	s32 field_0xC; //0xC
	s32 field_0x10; //0x10
	f32 field_0x14; //0x14
	f32 field_0x18; //0x18
} BattleWorkHpGauge;

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
	BattleDataEntry* dataTable; //0x18
	s32 mFlags; //0x1C
	s8 moveState; //0x20
	s8 maxMoveCount; //0x21
	s8 movesRemaining; //0x22
	s8 maxMovesThisTurn; //0x23
	s8 activeTurns; //0x24
	s8 swallowRate; //0x25
	s8 swallowAttribute; //0x26
	u8 field_0x27[0x30 - 0x27]; //0x27
	Vec mHomePosition; //0x30
	Vec mPosition; //0x3C
	Vec mPositionOffset; //0x48
	Vec mDisplayOffset; //0x54
	Vec mBaseRotation; //0x60
	Vec mRotation; //0x6C
	Vec mRotationOffset; //0x78
	Vec offsetCenter; //0x84
	Vec mScale; //0x90
	Vec mBaseScale; //0x9C
	Vec togeOffset; //0xA8
	u8 field_0xB4[0xC0 - 0xB4]; //0xB4
	Vec heldItemOffset; //0xC0
	s16 mWidth; //0xCC
	s16 mHeight; //0xCE
	s16 mStatusIconOffset[2]; //0xD0
	s16 mHpGaugeOffset[2]; //0xD4
	Vec cutBaseOffset; //0xD8
	f32 cutWidth; //0xE4
	f32 cutHeight; //0xE8
	Vec bintaHitOffset; //0xEC, Love Slap
	Vec kissHitOffset; //0xF8, Lip Lock
	BattleUnitAttribute_Flags mAttributeFlags; //0x104
	s16 maxHp; //0x108
	s16 mBaseMaxHp; //0x10A
	s16 currentHp; //0x10C
	s16 mMaxFp; //0x10E
	s16 mBaseMaxFp; //0x110
	s16 currentFp; //0x112
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
	s8 flippedTurns; //0x12B, turn count left until flipping back
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
	//additional 4 bytes here in US
	s32 protectId; //0x13C
	BattleVulnerableStatus* vulnStatus; //0x140
	Vec mMoveStartPos; //0x144
	Vec mMoveCurrentPos; //0x150
	Vec mMoveTargetPos; //0x15C
	u8 field_0x168[0x16C - 0x168]; //0x168
	f32 mMoveSpeedXZ; //0x16C
	f32 mFallAccel; //0x170
	f32 mMoveSpeedY; //0x174
	u8 field_0x178[0x184 - 0x178]; //0x178
	s8 moveDirection; //0x184
	s8 faceDirection; //0x185
	u8 field_0x186[0x1F8 - 0x186]; //0x186
	BattleWorkHpGauge hpGauge; //0x1F8
	s32 work[16]; //0x214
	BattleWorkUnitPart* currentTarget; //0x254
	u8 field_0x258[0x25C - 0x258]; //0x258
	s16 hpDamageCount; //0x25C, number of damaging moves
	u8 field_0x25E[0x260 - 0x25E]; //0x25E
	s32 totalHpDamageTaken; //0x260
	u8 field_0x264[0x26C - 0x264]; //0x264
	s8 hpDamageTaken; //0x26C
	s8 fpDamageTaken; //0x26D
	u8 field_0x26E[0x278 - 0x26E]; //0x26E
	s32 damagePattern; //0x270, TODO: rename?
	s32 damageCode; //0x274
	BattleUnitToken_Flags mTokenFlags; //0x278
	u8 field_0x27C[0x284 - 0x27C]; //0x27C
	s32 initEventId; //0x284
	void* waitEventCode; //0x288
	s32 waitEventId; //0x28C
	void* unisonPhaseEventCode; //0x290
	void* phaseEventCode; //0x294
	s32 phaseEventId; //0x298
	u8 field_0x29C[0x2A4 - 0x29C]; //0x29C
	s32 attackEventId; //0x2A4
	u8 field_0x2A8[0x2B0 - 0x2A8]; //0x2A8
	void* damageEventCode; //0x2AC
	s32 damageEventId; //0x2B0
	u8 field_0x2B4[0x2DC - 0x2B4]; //0x2B4
	BattleWorkUnitBadgesEquipped mBadgesEquipped; //0x2DC
	ItemType heldItem; //0x304, 4 bytes
	u8 field_0x308[0x30D - 0x308]; //0x308
	u8 mMoveColorLv;  //0x30D
	u8 field_0x30E[0x334 - 0x30E]; //0x30E
	s16 mBurnDamageFlameState; //0x334
	s16 mbBurnDamageFlameActive; //0x336
	s32 mBurnDamageFlameAnimTimer; //0x338
	f32 mBurnDamageFlameScale; //0x33C
	EffEntry* mBurnDamageFlameEff; //0x340
	BattleWorkStatusIcon mStatusIconWork; //0x344
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









void BtlUnit_CheckPinchStatus(BattleWorkUnit* unit, BOOL a2);




void BtlUnit_SetParamToPouch(BattleWorkUnit* unit);





void BtlUnit_ResetMoveStatus(BattleWorkUnit* unit);



s32 BtlUnit_GetFp(BattleWorkUnit* unit);
void BtlUnit_SetFp(BattleWorkUnit* unit, s32 value);
s32 BtlUnit_GetMaxFp(BattleWorkUnit* unit);
void BtlUnit_SetMaxFp(BattleWorkUnit* unit, s32 value);
void BtlUnit_RecoverHp(BattleWorkUnit* unit, s32 value);
void BtlUnit_RecoverFp(BattleWorkUnit* unit, s32 value);
s32 BtlUnit_GetHitDamage(BattleWorkUnit* unit);
s32 BtlUnit_GetTotalHitDamage(BattleWorkUnit* unit);
void* BtlUnit_GetData(BattleWorkUnit* unit, s32 tag);
BOOL BtlUnit_CheckData(BattleWorkUnit* unit, s32 tag);
BOOL BtlUnit_GetEnemyBelong(BattleWorkUnit* unit);

