#pragma once

#include <dolphin/types.h>

#define mNumKeyItems 121
//TODO: change name if it collides with backpack something
#define mNumHeldItems 20
#define mMinHeldItems 10
#define GetHeldItemCount pouchCheckItem(kStrangeSack) ? mNumHeldItems : mMinHeldItems
#define mNumStoredItems 32
#define mNumBadges 200

#pragma enumsalwaysint off
//2-byte wide
typedef enum ItemType1 {
	kNullItem, //0x0
// Key Items -------------------------------------------
	kStrangeSack, //0x1
#define KEY_ITEM_MIN kStrangeSack
	kPaperCurse, //0x2
	kTubeCurse, //0x3
	kPlaneCurse, //0x4
	kBoatCurse, //0x5
	kBoots, //0x6
	kSuperBoots, //0x7
	kUltraBoots, //0x8
	kHammer, //0x9
	kSuperHammer, //0xA
	kUltraHammer, //0xB
	kCastleKey_0x0c, //0xC
	kCastleKey_0x0d, //0xD
	kCastleKey_0x0e, //0xE
	kCastleKey_0x0f, //0xF
	kRedKey, //0x10
	kBlueKey, //0x11
	kStorageKey_0x12, //0x12
	kStorageKey_0x13, //0x13
	kGrottoKey, //0x14
	kShopKey, //0x15
	kSteepleKey_0x16, //0x16
	kSteepleKey_0x17, //0x17
	kSteepleKey_0x18, //0x18
	kSteepleKey_0x19, //0x19
	kElevatorKey_0x1a, //0x1A
	kElevatorKey_0x1b, //0x1B
	kElevatorKey_0x1c, //0x1C
	kCardKey_0x1d, //0x1D
	kCardKey_0x1e, //0x1E
	kCardKey_0x1f, //0x1F
	kCardKey_0x20, //0x20
	kBlackKey_0x21, //0x21
	kBlackKey_0x22, //0x22
	kBlackKey_0x23, //0x23
	kBlackKey_0x24, //0x24
	kStarKey, //0x25
	kPalaceKey_0x26, //0x26
	kPalaceKey_0x27, //0x27
	kPalaceKey_0x28, //0x28
	kPalaceKey_0x29, //0x29
	kPalaceKey_0x2a, //0x2A
	kPalaceKey_0x2b, //0x2B
	kPalaceKey_0x2c, //0x2C
	kPalaceKey_0x2d, //0x2D
	kPalaceKey_0x2e, //0x2E
	kPalaceKey_0x2f, //0x2F
	kPalaceKey_0x30, //0x30
	kHouseKey, //0x31
	kMagicalMap, //0x32
	kContactLens, //0x33
	kBlimpTicket, //0x34
	kTrainTicket, //0x35
	kMailboxSP, //0x36
	kSuperLuigi1, //0x37
	kSuperLuigi2, //0x38
	kSuperLuigi3, //0x39
	kSuperLuigi4, //0x3A
	kSuperLuigi5, //0x3B
	kCookbook, //0x3C
	kMoonStone, //0x3D
	kSunStone, //0x3E
	kNecklace, //0x3F
	kPuniOrb, //0x40
	kChampsBelt, //0x41
	kPoisonedCake, //0x42
	kSuperbombomb, //0x43
	kTheLetterP, //0x44
	kOldLetter, //0x45
	kChuckolaCola, //0x46
	kSkullGem, //0x47
	kGateHandle, //0x48
	kWeddingRing, //0x49
	kGalleyPot, //0x4A
	kGoldRing, //0x4B
	kShellEarrings, //0x4C
	kAutograph, //0x4D
	kRaggedDiary, //0x4E
	kBlanket, //0x4F
	kVitalPaper, //0x50
	kBriefcase, //0x51
	kGoldbobGuide, //0x52
	kUnk_Paper_0x53, //0x53
	kUnk_Paper_0x54, //0x54
	kCog, //0x55
	kDataDisk, //0x56
	kShineSprite, //0x57
	kUltraStone, //0x58
	kBowserBrosMeat, //0x59
	kMarioPoster, //0x5A
	kSpecialCard, //0x5B
	kPlatinumCard, //0x5C
	kGoldCard, //0x5D
	kSilverCard, //0x5E
	kBox, //0x5F
	kMagicalMap_Large, //0x60
	kDubiousPaper, //0x61
	kRoutingSlip, //0x62
	kWrestlingMag, //0x63
	kPresent, //0x64
	kBluePotion, //0x65
	kRedPotion, //0x66
	kOrangePotion, //0x67
	kGreenPotion, //0x68
	kUnk_Star_0x69, //0x69
	kLotteryPick, //0x6A
	kBattleTrunks, //0x6B
	kUpArrow, //0x6C
	kPackage, //0x6D
	kAttackFxBKeyItem, //0x6E
	kUnknown_0x6f, //0x6F
	kUnknown_0x70, //0x70
	kUnknown_0x71, //0x71
	kDiamondStar, //0x72
	kEmeraldStar, //0x73
	kGoldStar, //0x74
	kRubyStar, //0x75
	kSapphireStar, //0x76
	kGarnetStar, //0x77
	kCrystalStar, //0x78
#define KEY_ITEM_MAX kCrystalStar
// Normal Items ----------------------------------------
	kCoin, //0x79
#define ITEM_MIN kCoin
	kPianta, //0x7A
	kHeartPickup, //0x7B
	kFlowerPickup, //0x7C
	kStarPiece, //0x7D
	kGoldBar, //0x7E
	kGoldBarX3, //0x7F
	kThunderBolt, //0x80
	kThunderRage, //0x81
	kShootingStar, //0x82
	kIceStorm, //0x83
	kFireFlower, //0x84
	kEarthQuake, //0x85
	kBoosSheet, //0x86
	kVoltShroom, //0x87
	kRepelCape, //0x88
	kRuinPowder, //0x89
	kSleepySheep, //0x8A
	kPowBlock, //0x8B
	kStopwatch, //0x8C
	kDizzyDial, //0x8D
	kPowerPunch, //0x8E
	kCourageShell, //0x8F
	kHpDrainItem, //0x90
	kTradeOff, //0x91
	kMiniMrMini, //0x92
	kMrSoftener, //0x93
	kMushroom, //0x94
	kSuperShroom, //0x95
	kUltraShroom, //0x96
	kLifeShroom, //0x97
	kDriedShroom, //0x98
	kTastyTonic, //0x99
	kHoneySyrup, //0x9A
	kMapleSyrup, //0x9B
	kJamminJelly, //0x9C
	kSlowShroom, //0x9D
	kGradualSyrup, //0x9E
	kHotDog, //0x9F
	kCake, //0xA0
	kPointSwap, //0xA1
	kFrightMask, //0xA2
	kMystery, //0xA3
	kInnCoupon, //0xA4
	kWhackaBump, //0xA5
	kCoconut, //0xA6
	kDriedBouquet, //0xA7
	kMysticEgg, //0xA8
	kGoldenLeaf, //0xA9
	kKeelMango, //0xAA
	kFreshPasta, //0xAB
	kCakeMix, //0xAC
	kHotSauce, //0xAD
	kTurtleyLeaf, //0xAE
	kHorsetail, //0xAF
	kPeachyPeach, //0xB0
	kSpitePouch, //0xB1
	kKoopaCurse, //0xB2
	kShroomFry, //0xB3
	kShroomRoast, //0xB4
	kShroomSteak, //0xB5
	kMistake, //0xB6
	kHoneyShroom, //0xB7
	kMapleShroom, //0xB8
	kJellyShroom, //0xB9
	kHoneySuper, //0xBA
	kMapleSuper, //0xBB
	kJellySuper, //0xBC
	kHoneyUltra, //0xBD
	kMapleUltra, //0xBE
	kJellyUltra, //0xBF
	kSpicySoup, //0xC0
	kZessDinner, //0xC1
	kZessSpecial, //0xC2
	kZessDeluxe, //0xC3
	kZessDynamite, //0xC4
	kZessTea, //0xC5
	kSpaceFood, //0xC6
	kIciclePop, //0xC7
	kZessFrappe, //0xC8
	kSnowBunny, //0xC9
	kCoconutBomb, //0xCA
	kCourageMeal, //0xCB
	kShroomCake, //0xCC
	kShroomCrepe, //0xCD
	kMousseCake, //0xCE
	kFriedEgg, //0xCF
	kFruitParfait, //0xD0
	kEggBomb, //0xD1
	kInkPasta, //0xD2
	kSpaghetti, //0xD3
	kShroomBroth, //0xD4
	kPoisonShroom, //0xD5
	kChocoCake, //0xD6
	kMangoDelight, //0xD7
	kLovePudding, //0xD8
	kMeteorMeal, //0xD9
	kTrialStew, //0xDA
	kCouplesCake, //0xDB
	kInkySauce, //0xDC
	kOmeletteMeal, //0xDD
	kKoopaTea, //0xDE
	kKoopasta, //0xDF
	kSpicyPasta, //0xE0
	kHeartfulCake, //0xE1
	kPeachTart, //0xE2
	kElectroPop, //0xE3
	kFirePop, //0xE4
	kHoneyCandy, //0xE5
	kCocoCandy, //0xE6
	kJellyCandy, //0xE7
	kZessCookie, //0xE8
	kHealthySalad, //0xE9
	kKoopaBun, //0xEA
	kFreshJuice, //0xEB
#define ITEM_MAX kFreshJuice
// ??? -------------------------------------------------
	kAudienceCan, //0xEC
	kAudienceRock, //0xED
	kAudienceBone, //0xEE
	kAudienceHammer, //0xEF
// Badges ----------------------------------------------
	kPowerJump, //0xF0
#define BADGE_MIN kPowerJump
	kMultibounce, //0xF1
	kPowerBounce, //0xF2
	kTornadoJump, //0xF3
	kShrinkStomp, //0xF4
	kSleepyStomp, //0xF5
	kSoftStomp, //0xF6
	kPowerSmash, //0xF7
	kQuakeHammer, //0xF8
	kHammerThrow, //0xF9
	kPiercingBlow, //0xFA
	kHeadRattle, //0xFB
	kFireDrive, //0xFC
	kIceSmash, //0xFD
	kDoubleDip, //0xFE
	kDoubleDipP, //0xFF
	kCharge, //0x100
	kChargeP, //0x101
	kSuperAppeal, //0x102
	kSuperAppealP, //0x103
	kPowerPlus, //0x104
	kPowerPlusP, //0x105
	kPUpDDown, //0x106
	kPUpDDownP, //0x107
	kAllOrNothing, //0x108
	kAllOrNothingP, //0x109
	kMegaRush, //0x10A
	kMegaRushP, //0x10B
	kPowerRush, //0x10C
	kPowerRushP, //0x10D
	kPDownDUp, //0x10E
	kPDownDUpP, //0x10F
	kLastStand, //0x110
	kLastStandP, //0x111
	kDefendPlus, //0x112
	kDefendPlusP, //0x113
	kDamageDodge, //0x114
	kDamageDodgeP, //0x115
	kHpPlus, //0x116
	kHpPlusP, //0x117
	kFpPlus, //0x118
	kFlowerSaver, //0x119
	kFlowerSaverP, //0x11A
	kIcePower, //0x11B
	kSpikeShield, //0x11C
	kFeelingFine, //0x11D
	kFeelingFineP, //0x11E
	kZapTap, //0x11F
	kDoublePain, //0x120
	kJumpman, //0x121
	kHammerman, //0x122
	kReturnPostage, //0x123
	kHappyHeart, //0x124
	kHappyHeartP, //0x125
	kHappyFlower, //0x126
	kHpDrainBadge, //0x127
	kHpDrainP, //0x128
	kFpDrain, //0x129
	kFpDrainP, //0x12A
	kCloseCall, //0x12B
	kCloseCallP, //0x12C
	kPrettyLucky, //0x12D
	kPrettyLuckyP, //0x12E
	kLuckyDay, //0x12F
	kLuckyDayP, //0x130
	kRefund, //0x131
	kPityFlower, //0x132
	kPityFlowerP, //0x133
	kQuickChange, //0x134
	kPeekaboo, //0x135
	kTimingTutor, //0x136
	kHeartFinder, //0x137
	kFlowerFinder, //0x138
	kMoneyMoney, //0x139
	kItemHog, //0x13A
	kAttackFxR, //0x13B
	kAttackFxB, //0x13C
	kAttackFxG, //0x13D
	kAttackFxY, //0x13E
	kAttackFxP, //0x13F
	kChillOut, //0x140
	kFirstAttack, //0x141
	kBumpAttack, //0x142
	kSlowGo, //0x143
	kSimplifier, //0x144
	kUnsimplifier, //0x145
	kLuckyStart, //0x146
	kLEmblem, //0x147
	kWEmblem, //0x148
	kTripleDip, //0x149
	kLuckyStartP, //0x14A
	kAutoCommandBadge, //0x14B
	kMegaJump, //0x14C
	kMegaSmash, //0x14D
	kMegaQuake, //0x14E
	kUnusedDefendBadge, //0x14F
	kUnusedDefendBadgeP, //0x150
	kSuperCharge, //0x151
	kSuperChargeP, //0x152
#define BADGE_MAX kSuperChargeP
	kItemMax
} ItemType1;
#pragma enumsalwaysint on

typedef enum MarioPartners {
	PARTNER_NONE,
	PARTNER_GOOMBELLA,
	PARTNER_KOOPS,
	PARTNER_BOBBERY,
	PARTNER_YOSHI,
	PARTNER_FLURRIE,
	PARTNER_VIVIAN,
	PARTNER_MSMOWZ
} MarioPartners;

typedef struct PouchPartyData {
	u16 mFlags; //0x0
	u16 mMaxHP; //0x2
	u16 mBaseMaxHP; //0x4
	u16 mCurrentHP; //0x6
	u16 mHPLevel; //0x8
	u16 mAttackLevel; //0xA
	u16 mTechLevel; //0xC
} PouchPartyData;

typedef struct PouchData {
	PouchPartyData mPartyData[8]; //0x0
	u16 mCurrentHP; //0x70
	u16 mMaxHP; //0x72
	u16 mCurrentFP; //0x74
	u16 mMaxFP; //0x76
	u16 mCoins; //0x78
	u16 mCurrentSP; //0x7A
	u16 mMaxSP; //0x7C
	u16 field_0x7E; //0x7E
	u16 field_0x80; //0x80
	u16 field_0x82; //0x82
	f32 mLastAudienceCount; //0x84
	u16 mRank; //0x88
	u16 mLevel; //0x8A
	u16 mStarPowersObtained; //0x8C
	u16 mBaseMaxHP; //0x8E
	u16 mBaseMaxFP; //0x90
	u16 mAvailableBP; //0x92
	u16 mTotalBP; //0x94
	u16 mStarPoints; //0x96
	u8 mJumpLevel; //0x98
	u8 mHammerLevel; //0x99
	u16 mStarPieceCount; //0x9A
	u16 mShineSpriteCount; //0x9C
	u16 mPowerBounceRecord; //0x9E, Pit of 100 Trials
	ItemType1 mKeyItems[mNumKeyItems]; //0xA0
	ItemType1 mHeldItems[mNumHeldItems]; //0x192
	ItemType1 mStoredItems[mNumStoredItems]; //0x1BA
	ItemType1 mBadges[mNumBadges]; //0x1FA
	ItemType1 mEquippedBadges[mNumBadges]; //0x38A
	u8 mEmailIds[100]; //0x51A
	u8 field_0x57E[2]; //0x57E, alignment?
	u32 mEmailReceivedFlags[4]; //0x580
	u32 mEmailReadFlags[4]; //0x590
	u8 field_0x5A0[0x10]; //0x5A0
	u16 mShopPoints; //0x5B0
	u8 field_0x5B2[2]; //0x5B2
	u32 field_0x5B4; //0x5B4
	u8 mMerleeCurseUsesRemaining; //0x5B8
	u8 mTurnsUntilMerleeActivation; //0x5B9
	u8 mMerleeNextCurseType; //0x5BA
	u8 mSuperBowserCoins; //0x5BB
	u32 mSuperBowserScore; //0x5BC
	char mPartnerYoshiName[16]; //0x5C0
	u8 field_0x5D0[4]; //0x5D0
} PouchData;

void pouchInit(void);
PouchData* pouchGetPtr(void);
ItemType1 pouchKeyItem(s32 id);
ItemType1 pouchHaveItem(s32 id);
ItemType1 pouchKeepItem(s32 id);
ItemType1 pouchHaveBadge(s32 id);
u32 pouchGetHaveItemCnt(void);
u32 pouchGetKeepItemCnt(void);
u32 pouchGetHaveBadgeCnt(void);
u32 pouchGetEquipBadgeCnt(void);
u32 pouchGetEmptyHaveItemCnt(void);
u32 pouchGetEmptyKeepItemCnt(void);


BOOL pouchGetItem(u32 itemId);
u32 pouchCheckItem(u32 itemId);