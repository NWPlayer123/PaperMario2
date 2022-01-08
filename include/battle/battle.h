#pragma once

#include <dolphin/types.h>
#include "battle/battle_ac.h"
#include "battle/battle_audience.h"
#include "battle/battle_camera.h"
#include "battle/battle_database_common.h"
#include "battle/battle_icon.h"
#include "battle/battle_pad.h"
#include "battle/battle_stage_object.h"
#include "drv/dispdrv.h"
#include "drv/npcdrv.h"
#include "mario/mario_pouch.h"

typedef struct BattleWork BattleWork;
typedef struct BattleWorkUnit BattleWorkUnit;
typedef struct BattleWorkCommand BattleWorkCommand;
typedef struct BattleWorkAlliance BattleWorkAlliance;
typedef struct BattleWorkCommandAction BattleWorkCommandAction;
typedef struct BattleWorkCommandWeapon BattleWorkCommandWeapon;
typedef struct BattleWorkCommandOperation BattleWorkCommandOperation;
typedef struct BattleWorkCommandParty BattleWorkCommandParty;
typedef struct BattleWorkCommandMultiItem BattleWorkCommandMultiItem;
typedef struct BattleWorkCommandCursor BattleWorkCommandCursor;
typedef struct BattleWorkCommandWindow BattleWorkCommandWindow;

typedef struct FieldBattleInfo FieldBattleInfo;

//battle_status_effect
//should be an s8, typecasting for now
typedef enum StatusEffectType {
	kStatusAllergic, //0x0
	kStatusSleep, //0x1
	kStatusStop, //0x2
	kStatusDizzy, //0x3
	kStatusPoison, //0x4
	kStatusConfuse, //0x5
	kStatusElectric, //0x6
	kStatusDodgy, //0x7
	kStatusBurn, //0x8
	kStatusFreeze, //0x9
	kStatusHuge, //0xA
	kStatusTiny, //0xB
	kStatusAttackUp, //0xC
	kStatusAttackDown, //0xD
	kStatusDefenseUp, //0xE
	kStatusDefenseDown, //0xF
	kStatusCharge, //0x10
	kStatusFlipped, //0x11
	kStatusInvisible, //0x12
	kStatusFast, //0x13
	kStatusSlow, //0x14
	kStatusPayback, //0x15
	kStatusHoldFast, //0x16
	kStatusHpRegen, //0x17
	kStatusFpRegen, //0x18
	kStatusFright, //0x19
	kStatusGaleForce, //0x1A
	kStatusInstantKill, //0x1B
	kStatusInvalid //0x1C
} StatusEffectType;

//battle_unit
typedef struct BattleWorkUnitPart BattleWorkUnitPart;
typedef struct BattleUnitKindPart BattleUnitKindPart;


typedef enum BattleUnitType {
	kNullUnitKind, //0x0
	kUnitGoomba, //0x1
	kUnitParagoomba, //0x2
	kUnitSpikyGoomba, //0x3
	kUnitSpinia, //0x4
	kUnitSpania, //0x5
	kUnitLordCrumpPrologue, //0x6
	kUnitGus, //0x7
	kUnitBlooper, //0x8
	kUnitBlooperLeftTentacle, //0x9
	kUnitBlooperRightTentacle, //0xA
	kUnitKoopatrol, //0xB
	kUnitMagikoopa, //0xC
	kUnitMagikoopaClone, //0xD
	kUnitKoopaTroopa, //0xE
	kUnitParatroopa, //0xF
	kUnitFuzzy, //0x10
	kUnitDullBones, //0x11
	kUnitBaldCleft, //0x12
	kUnitBristle, //0x13
	kUnitGoldFuzzy, //0x14
	kUnitFuzzyHorde, //0x15
	kUnitRedBones, //0x16
	kUnitHooktail, //0x17
	kUnitDarkPuff, //0x18
	kUnitPalePiranha, //0x19
	kUnitCleft, //0x1A
	kUnitPider, //0x1B
	kUnitXNaut, //0x1C
	kUnitYux, //0x1D
	kUnitMiniYux, //0x1E
	kUnitBeldamChapter2, //0x1F
	kUnitMarilynChapter2, //0x20
	kUnitVivianChapter2, //0x21
	kUnitMagnus, //0x22
	kUnitXFist, //0x23
	kUnitGoombaGlitzville, //0x24
	kUnitKpKoopa, //0x25
	kUnitKpParatroopa, //0x26
	kUnitPokey, //0x27
	kUnitLakitu, //0x28
	kUnitSpiny, //0x29
	kUnitHyperBaldCleft, //0x2A
	kUnitBobOmb, //0x2B
	kUnitBandit, //0x2C
	kUnitBigBandit, //0x2D
	kUnitRedSpikyBuzzy, //0x2E
	kUnitShadyKoopa, //0x2F
	kUnitShadyParatroopa, //0x30
	kUnitRedMagikoopa, //0x31
	kUnitRedMagikoopaClone, //0x32
	kUnitWhiteMagikoopa, //0x33
	kUnitWhiteMagikoopaClone, //0x34
	kUnitGreenMagikoopa, //0x35
	kUnitGreenMagikoopaClone, //0x36
	kUnitDarkCraw, //0x37
	kUnitHammerBro, //0x38
	kUnitBoomerangBro, //0x39
	kUnitFireBro, //0x3A
	kUnitRedChomp, //0x3B
	kUnitDarkKoopatrol, //0x3C
	kUnitIronCleftRed, //0x3D
	kUnitIronCleftGreen, //0x3E
	kUnitBowserChapter3, //0x3F
	kUnitRawkHawk, //0x40
	kUnitMachoGrubba, //0x41
	kUnitHyperGoomba, //0x42
	kUnitHyperParagoomba, //0x43
	kUnitHyperSpikyGoomba, //0x44
	kUnitCrazeeDayzee, //0x45
	kUnitAmazyDayzee, //0x46
	kUnitHyperCleft, //0x47
	kUnitBuzzyBeetle, //0x48
	kUnitSpikeTop, //0x49
	kUnitSwooper, //0x4A
	kUnitBoo, //0x4B
	kUnitAtomicBoo, //0x4C
	kUnitDooplissChapter4Fight1, //0x4D
	kUnitDooplissChapter4Invincible, //0x4E
	kUnitDooplissChapter4Fight2, //0x4F
	kUnitGoombellaChapter4, //0x50
	kUnitKoopsChapter4, //0x51
	kUnitYoshiChapter4, //0x52
	kUnitFlurrieChapter4, //0x53
	kUnitEmber, //0x54
	kUnitLavaBubble, //0x55
	kUnitGreenFuzzy, //0x56
	kUnitFlowerFuzzy, //0x57
	kUnitPutridPiranha, //0x58
	kUnitParabuzzy, //0x59
	kUnitBillBlaster, //0x5A
	kUnitBulletBill, //0x5B
	kUnitBulkyBobOmb, //0x5C
	kUnitCortez, //0x5D
	kUnitCortezBonePile, //0x5E
	kUnitCortezSword, //0x5F
	kUnitCortezHook, //0x60
	kUnitCortezRapier, //0x61
	kUnitCortezSaber, //0x62
	kUnitLordCrumpChapter5, //0x63
	kUnitXNautsCrumpFormation1, //0x64
	kUnitXNautsCrumpFormation2, //0x65
	kUnitXNautsCrumpFormation3, //0x66
	kUnitRuffPuff, //0x67
	kUnitPoisonPokey, //0x68
	kUnitSpikyParabuzzy, //0x69
	kUnitDarkBoo, //0x6A
	kUnitSmorg, //0x6B
	kUnitSmorgMiasmaTentacleA, //0x6C
	kUnitSmorgMiasmaTentacleB, //0x6D
	kUnitSmorgMiasmaTentacleC, //0x6E
	kUnitSmorgMiasmaClaw, //0x6F
	kUnitIcePuff, //0x70
	kUnitFrostPiranha, //0x71
	kUnitMoonCleft, //0x72
	kUnitZYux, //0x73
	kUnitMiniZYux, //0x74
	kUnitXYux, //0x75
	kUnitMIniXYux, //0x76
	kUnitXNautPhD, //0x77
	kUnitEliteXNaut, //0x78
	kUnitMagnus20, //0x79
	kUnitXPunch, //0x7A
	kUnitSwoopula, //0x7B
	kUnitPhantomEmber, //0x7C
	kUnitBombshellBillBlaster, //0x7D
	kUnitBombshellBill, //0x7E
	kUnitChainChomp, //0x7F
	kUnitDarkWizzerd, //0x80
	kUnitDarkWizzerdClone, //0x81
	kUnitDryBones, //0x82
	kUnitDarkBones, //0x83
	kUnitGloomtail, //0x84
	kUnitBeldamChapter8, //0x85
	kUnitMarilynChapter8, //0x86
	kUnitDooplissChapter8, //0x87
	kUnitDooplissChapter8Mario, //0x88
	kUnitDooplissChapter8Goombella, //0x89
	kUnitDooplissChapter8Koops, //0x8A
	kUnitDooplissChapter8Yoshi, //0x8B
	kUnitDooplissChapter8Flurrie, //0x8C
	kUnitDooplissChapter8Vivian, //0x8D
	kUnitDooplissChapter8Bobbery, //0x8E
	kUnitDooplissChapter8MsMowz, //0x8F
	kUnitBowserChapter8, //0x90
	kUnitKammyKoopa, //0x91
	kUnitGrodus, //0x92
	kUnitGrodusX, //0x93
	kUnitShadowQueenPeach, //0x94
	kUnitShadowQueenInvincible, //0x95
	kUnitShadowQueenDemon, //0x96
	kUnitShadowQueenLeftRightHand, //0x97
	kUnitShadowQueenDeadHands, //0x98
	kUnitGloomba, //0x99
	kUnitParagloomba, //0x9A
	kUnitSpikyGloomba, //0x9B
	kUnitDarkKoopa, //0x9C
	kUnitDarkParatroopa, //0x9D
	kUnitBadgeBandit, //0x9E
	kUnitDarkLakitu, //0x9F
	kUnitSkyBlueSpiny, //0xA0
	kUnitWizzerd, //0xA1
	kUnitPiranhaPlant, //0xA2
	kUnitSpunia, //0xA3
	kUnitArantula, //0xA4
	kUnitDarkBristle, //0xA5
	kUnitPoisonPuff, //0xA6
	kUnitSwampire, //0xA7
	kUnitBobUlk, //0xA8
	kUnitEliteWizzerd, //0xA9
	kUnitEliteWizzerdClone, //0xAA
	kUnitBonetail, //0xAB
	kUnitUnusedRedBuzzy, //0xAC
	kUnitUnusedRedParabuzzy, //0xAD
	kUnitUnusedRedSpikyParabuzzy, //0xAE
	kUnitUnusedHyperBobOmb, //0xAF
	kUnitUnusedUltraBobOmb, //0xB0
	kUnitTutorialGoombella, //0xB1
	kUnitTutorialFranklyB2, //0xB2
	kUnitTutorialFranklyB3, //0xB3
	kUnitTutorialFranklyB4, //0xB4
	kUnitEpilogueDooplissMario, //0xB5
	kUnitEpilogueFlurrie, //0xB6
	kUnitEpilogueBoo, //0xB7
	kUnitEpilogueAtomicBoo, //0xB8
	kUnitEpilogueMaleToad, //0xB9
	kUnitEpilogueFemaleToad, //0xBA
	kUnitUnusedTest, //0xBB
	kUnitUnusedCrump2, //0xBC
	kUnitUnusedBeldam2, //0xBD
	kUnitUnusedMarilyn2, //0xBE
	kUnitUnusedVivian2, //0xBF
	kUnitUnusedBeldam3, //0xC0
	kUnitUnusedMarilyn3, //0xC1
	kUnitUnusedMechaKuri, //0xC2
	kUnitUnusedMechaKame, //0xC3
	kUnitUnusedOkorl, //0xC4
	kUnitUnusedYowarl, //0xC5
	kUnitUnusedTuyonarl, //0xC6
	kUnitUnusedWanawana, //0xC7
	kUnitUnusedMinaraiKamec, //0xC8
	kUnitUnusedShyGuy, //0xC9
	kUnitUnusedGrooveGuy, //0xCA
	kUnitUnusedPyroGuy, //0xCB
	kUnitUnusedSpyGuy, //0xCC
	kUnitUnusedAntiGuy, //0xCD
	kUnitUnusedBzzap, //0xCE
	kUnitUnusedMiniBzzap, //0xCF
	kUnitUnusedUfo, //0xD0
	kUnitUnusedPennington, //0xD1
	kUnitUnusedFighter, //0xD2
	kUnitUnusedZessT, //0xD3
	kUnitUnusedMaster, //0xD4
	kUnitUnusedReporter, //0xD5
	kUnitUnusedHotdogMaster, //0xD6
	kUnitUnusedFlavio, //0xD7
	kUnitUnusedTree, //0xD8
	kUnitUnusedSwitch, //0xD9
	kUnitUnusedTestnpc, //0xDA
	kUnitBombSquadBomb, //0xDB
	kUnitSystem, //0xDC
	kUnitPrologueGoombella, //0xDD
	kUnitMario, //0xDE
	kUnitShellShield, //0xDF
	kUnitGoombella, //0xE0
#define TYPE_PARTNER_MIN kUnitGoombella
	kUnitKoops, //0xE1
	kUnitYoshi, //0xE2
	kUnitFlurrie, //0xE3
	kUnitVivian, //0xE4
	kUnitBobbery, //0xE5
	kUnitMsMowz, //0xE6
#define TYPE_PARTNER_MAX kUnitMsMowz+1
	kUnitMax //0xE7
} BattleUnitType;

typedef enum BattleFlags {
	BATTLE_SEQ_END = 0x80000000
} BattleFlags;

typedef enum BattleSequence {
	SEQ_UNKNOWN = 0,
	SEQ_INIT = 1,
	SEQ_FIRST_ACT = 2,
	SEQ_TURN = 3,
	SEQ_PHASE = 4,
	SEQ_MOVE = 5,
	SEQ_ACT = 6,
	SEQ_END = 7
} BattleSequence;

struct FieldBattleInfo {
	u32 mode; //0x0, unused
	BattleUnitType party; //0x4
	u32 partyId; //0x8
	NpcBattleInfo* setup; //0xC
	u32 result; //0x10
	u8 field_0x14[0x1C - 0x14]; //0x14
};

struct BattleWorkCommandAction {
	u32 mType; //0x0
	BOOL mEnabled; //0x4
	char* mName; //0x8
	u16 mIcon; //0xC
	u8 pad_0xE[2]; //0xE
	u32 field_0x10; //0x10, mDisabled?
};

struct BattleWorkCommandWeapon {
	BattleWeapon* mWeaponParams; //0x0
	u32 field_0x4; //0x4
	char* mName; //0x8
	u16 mIcon; //0xC
	u8 pad_0xE[2]; //0xE
	u32 mIndex; //0x10
	ItemType mItem; //0x14
	u32 field_0x18; //0x18
};

struct BattleWorkCommandOperation {
	u32 mType; //0x0
	BOOL mEnabled; //0x4
	u32 field_0x8; //0x8, mDisabled?
	char* mName; //0xC
	u16 mIcon; //0x10
	u8 pad_0x12[2]; //0x12
	char* mDescription; //0x14
	u32 mWeaponCost; //0x18
};

struct BattleWorkCommandParty {
	BattleUnitType mPartyId; //0x0
	BOOL mEnabled; //0x4
	char* mName; //0x8
	u16 mIcon; //0xC
	u8 pad_0xE[2]; //0xE
	char* mDescription; //0x10
	u16 mCurrentHP; //0x14
	u16 mMaxHP; //0x16
};

struct BattleWorkCommandMultiItem {
	BOOL mEnabled; //0x0
	char* mName; //0x4
	char* mDescription; //0x8
	u16 mIcon; //0xC
	u8 pad_0xE[2]; //0xE
	u32 mWeaponCost; //0x10
	u32 field_0x14; //0x14, mDisabled?
};

struct BattleWorkCommandCursor {
	u32 mAbsolutePos; //0x0
	u32 mRelativePos; //0x4
	u32 mNumOptions; //0x8
};

struct BattleWorkCommandWindow {
	u8 field_0x0[0x124 - 0]; //0x0
};

typedef struct BattleWorkActRecord {
	u8 field_0x0[0x24 - 0x0]; //0x0
} BattleWorkActRecord;

//TODO: modularize for more actions/party members?
struct BattleWorkCommand {
	u32 mCurrentState; //0x0
	u32 mCurrentMenu; //0x4
	BattleWorkCommandAction mActionTable[6]; //0x8
	BattleWorkCommandWeapon mWeaponTable[21]; //0x80
	BattleWorkCommandOperation mOperationTable[7]; //0x2CC
	BattleWorkCommandParty mPartyTable[8]; //0x390
	BattleWorkCommandMultiItem mMultiItemTable[3]; //0x450
	BattleWorkCommandCursor mCursors[14]; //0x498
	u8 field_0x540[0x55C - 0x540]; //0x540
	BattleWorkCommandWindow* mWindowWork; //0x55C
	u8 field_0x560[0x574 - 0x560]; //0x560
};

struct BattleWorkAlliance {
	s16 mAllianceId; //0x0
	s8 mAttackDirection; //0x2
	u8 pad_3; //0x3, TODO remove?
	u32 mClearConditionMet; //0x4
};

struct BattleWork {
	u16 mTurnCount; //0x0
	u16 pad_0x2; //0x2
	u32 mSeq_Unknown; //0x4
	BattleWorkAlliance mAllianceInfo[3]; //0x8
	BattleWorkUnit* mUnits[64]; //0x20
	u8 field_0x120[0x424 - 0x120]; //0x120
	s32 field_0x424; //0x424
	u8 field_0x428[0xEF4 - 0x428]; //0x428
	BattleFlags flags; //0xEF4
	s32 mIconFlags; //0xEF8, TODO better name?
	u8 field_0xEFC[0xF0C - 0xEFC]; //0xEFC
	u32 mSeqInit; //0xF0C
	u32 mSeqFirstAct; //0xF10
	u32 mSeqTurn; //0xF14
	u32 mSeqPhase; //0xF18
	u32 mSeqMove; //0xF1C
	u32 mSeqAct; //0xF20
	u32 mSeqEnd; //0xF24
	void* mBattleEndSeqWork; //0xF28
	BattleWorkPad mPadWork[4]; //0xF2C
	BattleWorkCommand mCommandMenuWork; //0x171C
	BattleACManager actionCommands; //0x1C90
	FieldBattleInfo* mFieldBattleInfo; //0x2738
	u8 field_0x273C[0x2754 - 0x273C]; //0x273C
	BattleWorkCamera mCameraWork; //0x2754
	BattleAudience audience; //0x2858
	u8 field_0x1616C[0x163B8 - 0x1616C]; //0x1616C
	u32 mTattledUnitTypeFlags[8]; //0x163B8 JP, 0x163D4 US
	s32 mBadgeEquippedFlags; //0x163D8
	FileEntry* menuTex; //0x163DC JP, 0x163F8 US, battle_menu_disp
	u8 field_0x163DC[0x17140 - 0x163DC]; //0x163DC
	BattleWorkStageObject mStageObjectWork[32]; //0x17140, 0x1715C in US
	u8 field_0x180C0[0x182B0 - 0x180C0]; //0x180C0, 0x180DC in US
	BattleWorkIcon iconWork[16]; //0x182B0, TODO: rename to just "icon"?
	s16 field_0x18C70; //0x18C70 JP, 0x18C8C US, _disp Xpos something
	s16 field_0x18C82; //0x18C72 JP, 0x18C7E US, _disp Ypos something
	u8 field_0x18C84[0x18FE0 - 0x18C84]; //0x18C84
	s8 alertTick; //0x18FE0 JP, 0x18FF8 US
	u8 field_0x18FE1[3]; //0x18FE1, padding?
	StarPowerInfo mImpendingWeaponBonuses; //0x18FE4, 0x18FFC in US
	u8 field_0x18FF0[0x19050 - 0x18FF0]; //0x18FF0
	u32 mReserveItems[4]; //0x19050
	s32 field_0x19060; //0x19060
	u8 field_0x19064[0x19088 - 0x19064]; //0x19064
};
//u32 test = sizeof(BattleWorkCommand);

void battleMain(void);






void battle_exit(void);
BOOL battle_init(void);
void BattleAfterReactionQueueInit(void);






BattleWorkUnit* BattleGetPartnerPtr(BattleWork* work, BattleWorkUnit* unit);
BattleWorkUnit* BattleGetPartyPtr(BattleWork* work);
BattleWorkUnit* BattleGetMarioPtr(BattleWork* work);
BattleWorkUnit* BattleGetSystemPtr(BattleWork* work);
BattleWorkUnitPart* BattleGetUnitPartsPtr(s32 index, s32 partNum);
void BattleSetUnitPtr(BattleWork* work, s32 index, BattleWorkUnit* unit);
BattleWorkUnit* BattleGetUnitPtr(BattleWork* work, s32 index);
void BattleFree(void* ptr);
void* BattleAlloc(u32 size);
void BattleIncSeq(BattleWork* work, s32 seq);
u32 BattleGetSeq(BattleWork* work, BattleSequence seq);
void BattleSetSeq(BattleWork* work, BattleSequence seq, u32 num);
void BattleSetMarioParamToFieldBattle(BattleWork* work);
void Btl_UnitSetup(BattleWork* work);
void BattleEnd(void);
void BattleInit(FieldBattleInfo* info);
BOOL battleSeqEndCheck(void);



struct BattleWorkUnit* BattleChangeParty(BattleWork* wp);
s32 BattleTransPartyId(BattleUnitType kind);

BOOL battleDisableHResetCheck(void);