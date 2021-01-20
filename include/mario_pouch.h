#pragma once

#include <dolphin/types.h>

#define mNumKeyItems 121
//TODO: change name if it collides with backpack something
#define mNumHeldItems 20
#define mMinHeldItems 10
#define GetHeldItemCount pouchCheckItem(kItemStrangeSack) ? mNumHeldItems : mMinHeldItems
#define mNumStoredItems 32
#define mNumBadges 200

//4-byte wide, using u16 casting for mario_pouch
typedef enum ItemType {
	kNullItem, //0x0
// Key Items -------------------------------------------
	kItemStrangeSack, //0x1
#define KEY_ITEM_MIN kItemStrangeSack
	kItemPaperCurse, //0x2
	kItemTubeCurse, //0x3
	kItemPlaneCurse, //0x4
	kItemBoatCurse, //0x5
	kItemBoots, //0x6
	kItemSuperBoots, //0x7
	kItemUltraBoots, //0x8
	kItemHammer, //0x9
	kItemSuperHammer, //0xA
	kItemUltraHammer, //0xB
	kItemCastleKey_0x0c, //0xC
	kItemCastleKey_0x0d, //0xD
	kItemCastleKey_0x0e, //0xE
	kItemCastleKey_0x0f, //0xF
	kItemRedKey, //0x10
	kItemBlueKey, //0x11
	kItemStorageKey_0x12, //0x12
	kItemStorageKey_0x13, //0x13
	kItemGrottoKey, //0x14
	kItemShopKey, //0x15
	kItemSteepleKey_0x16, //0x16
	kItemSteepleKey_0x17, //0x17
	kItemSteepleKey_0x18, //0x18
	kItemSteepleKey_0x19, //0x19
	kItemElevatorKey_0x1a, //0x1A
	kItemElevatorKey_0x1b, //0x1B
	kItemElevatorKey_0x1c, //0x1C
	kItemCardKey_0x1d, //0x1D
	kItemCardKey_0x1e, //0x1E
	kItemCardKey_0x1f, //0x1F
	kItemCardKey_0x20, //0x20
	kItemBlackKey_0x21, //0x21
	kItemBlackKey_0x22, //0x22
	kItemBlackKey_0x23, //0x23
	kItemBlackKey_0x24, //0x24
	kItemStarKey, //0x25
	kItemPalaceKey_0x26, //0x26
	kItemPalaceKey_0x27, //0x27
	kItemPalaceKey_0x28, //0x28
	kItemPalaceKey_0x29, //0x29
	kItemPalaceKey_0x2a, //0x2A
	kItemPalaceKey_0x2b, //0x2B
	kItemPalaceKey_0x2c, //0x2C
	kItemPalaceKey_0x2d, //0x2D
	kItemPalaceKey_0x2e, //0x2E
	kItemPalaceKey_0x2f, //0x2F
	kItemPalaceKey_0x30, //0x30
	kItemHouseKey, //0x31
	kItemMagicalMap, //0x32
	kItemContactLens, //0x33
	kItemBlimpTicket, //0x34
	kItemTrainTicket, //0x35
	kItemMailboxSP, //0x36
	kItemSuperLuigi1, //0x37
	kItemSuperLuigi2, //0x38
	kItemSuperLuigi3, //0x39
	kItemSuperLuigi4, //0x3A
	kItemSuperLuigi5, //0x3B
	kItemCookbook, //0x3C
	kItemMoonStone, //0x3D
	kItemSunStone, //0x3E
	kItemNecklace, //0x3F
	kItemPuniOrb, //0x40
	kItemChampsBelt, //0x41
	kItemPoisonedCake, //0x42
	kItemSuperbombomb, //0x43
	kItemTheLetterP, //0x44
	kItemOldLetter, //0x45
	kItemChuckolaCola, //0x46
	kItemSkullGem, //0x47
	kItemGateHandle, //0x48
	kItemWeddingRing, //0x49
	kItemGalleyPot, //0x4A
	kItemGoldRing, //0x4B
	kItemShellEarrings, //0x4C
	kItemAutograph, //0x4D
	kItemRaggedDiary, //0x4E
	kItemBlanket, //0x4F
	kItemVitalPaper, //0x50
	kItemBriefcase, //0x51
	kItemGoldbobGuide, //0x52
	kItemUnk_Paper_0x53, //0x53
	kItemUnk_Paper_0x54, //0x54
	kItemCog, //0x55
	kItemDataDisk, //0x56
	kItemShineSprite, //0x57
	kItemUltraStone, //0x58
	kItemBowserBrosMeat, //0x59
	kItemMarioPoster, //0x5A
	kItemSpecialCard, //0x5B
	kItemPlatinumCard, //0x5C
	kItemGoldCard, //0x5D
	kItemSilverCard, //0x5E
	kItemBox, //0x5F
	kItemMagicalMap_Large, //0x60
	kItemDubiousPaper, //0x61
	kItemRoutingSlip, //0x62
	kItemWrestlingMag, //0x63
	kItemPresent, //0x64
	kItemBluePotion, //0x65
	kItemRedPotion, //0x66
	kItemOrangePotion, //0x67
	kItemGreenPotion, //0x68
	kItemUnk_Star_0x69, //0x69
	kItemLotteryPick, //0x6A
	kItemBattleTrunks, //0x6B
	kItemUpArrow, //0x6C
	kItemPackage, //0x6D
	kItemAttackFxBKeyItem, //0x6E
	kItemUnknown_0x6f, //0x6F
	kItemUnknown_0x70, //0x70
	kItemUnknown_0x71, //0x71
	kItemDiamondStar, //0x72
	kItemEmeraldStar, //0x73
	kItemGoldStar, //0x74
	kItemRubyStar, //0x75
	kItemSapphireStar, //0x76
	kItemGarnetStar, //0x77
	kItemCrystalStar, //0x78
#define KEY_ITEM_MAX kItemCrystalStar
// Normal Items ----------------------------------------
	kItemCoin, //0x79
#define ITEM_MIN kItemCoin
	kItemPianta, //0x7A
	kItemHeartPickup, //0x7B
	kItemFlowerPickup, //0x7C
	kItemStarPiece, //0x7D
	kItemGoldBar, //0x7E
	kItemGoldBarX3, //0x7F
	kItemThunderBolt, //0x80
	kItemThunderRage, //0x81
	kItemShootingStar, //0x82
	kItemIceStorm, //0x83
	kItemFireFlower, //0x84
	kItemEarthQuake, //0x85
	kItemBoosSheet, //0x86
	kItemVoltShroom, //0x87
	kItemRepelCape, //0x88
	kItemRuinPowder, //0x89
	kItemSleepySheep, //0x8A
	kItemPowBlock, //0x8B
	kItemStopwatch, //0x8C
	kItemDizzyDial, //0x8D
	kItemPowerPunch, //0x8E
	kItemCourageShell, //0x8F
	kItemHpDrainItem, //0x90
	kItemTradeOff, //0x91
	kItemMiniMrMini, //0x92
	kItemMrSoftener, //0x93
	kItemMushroom, //0x94
	kItemSuperShroom, //0x95
	kItemUltraShroom, //0x96
	kItemLifeShroom, //0x97
	kItemDriedShroom, //0x98
	kItemTastyTonic, //0x99
	kItemHoneySyrup, //0x9A
	kItemMapleSyrup, //0x9B
	kItemJamminJelly, //0x9C
	kItemSlowShroom, //0x9D
	kItemGradualSyrup, //0x9E
	kItemHotDog, //0x9F
	kItemCake, //0xA0
	kItemPointSwap, //0xA1
	kItemFrightMask, //0xA2
	kItemMystery, //0xA3
	kItemInnCoupon, //0xA4
	kItemWhackaBump, //0xA5
	kItemCoconut, //0xA6
	kItemDriedBouquet, //0xA7
	kItemMysticEgg, //0xA8
	kItemGoldenLeaf, //0xA9
	kItemKeelMango, //0xAA
	kItemFreshPasta, //0xAB
	kItemCakeMix, //0xAC
	kItemHotSauce, //0xAD
	kItemTurtleyLeaf, //0xAE
	kItemHorsetail, //0xAF
	kItemPeachyPeach, //0xB0
	kItemSpitePouch, //0xB1
	kItemKoopaCurse, //0xB2
	kItemShroomFry, //0xB3
	kItemShroomRoast, //0xB4
	kItemShroomSteak, //0xB5
	kItemMistake, //0xB6
	kItemHoneyShroom, //0xB7
	kItemMapleShroom, //0xB8
	kItemJellyShroom, //0xB9
	kItemHoneySuper, //0xBA
	kItemMapleSuper, //0xBB
	kItemJellySuper, //0xBC
	kItemHoneyUltra, //0xBD
	kItemMapleUltra, //0xBE
	kItemJellyUltra, //0xBF
	kItemSpicySoup, //0xC0
	kItemZessDinner, //0xC1
	kItemZessSpecial, //0xC2
	kItemZessDeluxe, //0xC3
	kItemZessDynamite, //0xC4
	kItemZessTea, //0xC5
	kItemSpaceFood, //0xC6
	kItemIciclePop, //0xC7
	kItemZessFrappe, //0xC8
	kItemSnowBunny, //0xC9
	kItemCoconutBomb, //0xCA
	kItemCourageMeal, //0xCB
	kItemShroomCake, //0xCC
	kItemShroomCrepe, //0xCD
	kItemMousseCake, //0xCE
	kItemFriedEgg, //0xCF
	kItemFruitParfait, //0xD0
	kItemEggBomb, //0xD1
	kItemInkPasta, //0xD2
	kItemSpaghetti, //0xD3
	kItemShroomBroth, //0xD4
	kItemPoisonShroom, //0xD5
	kItemChocoCake, //0xD6
	kItemMangoDelight, //0xD7
	kItemLovePudding, //0xD8
	kItemMeteorMeal, //0xD9
	kItemTrialStew, //0xDA
	kItemCouplesCake, //0xDB
	kItemInkySauce, //0xDC
	kItemOmeletteMeal, //0xDD
	kItemKoopaTea, //0xDE
	kItemKoopasta, //0xDF
	kItemSpicyPasta, //0xE0
	kItemHeartfulCake, //0xE1
	kItemPeachTart, //0xE2
	kItemElectroPop, //0xE3
	kItemFirePop, //0xE4
	kItemHoneyCandy, //0xE5
	kItemCocoCandy, //0xE6
	kItemJellyCandy, //0xE7
	kItemZessCookie, //0xE8
	kItemHealthySalad, //0xE9
	kItemKoopaBun, //0xEA
	kItemFreshJuice, //0xEB
#define ITEM_MAX kItemFreshJuice
// ??? -------------------------------------------------
	kItemAudienceCan, //0xEC
	kItemAudienceRock, //0xED
	kItemAudienceBone, //0xEE
	kItemAudienceHammer, //0xEF
// Badges ----------------------------------------------
	kItemPowerJump, //0xF0
#define BADGE_MIN kItemPowerJump
	kItemMultibounce, //0xF1
	kItemPowerBounce, //0xF2
	kItemTornadoJump, //0xF3
	kItemShrinkStomp, //0xF4
	kItemSleepyStomp, //0xF5
	kItemSoftStomp, //0xF6
	kItemPowerSmash, //0xF7
	kItemQuakeHammer, //0xF8
	kItemHammerThrow, //0xF9
	kItemPiercingBlow, //0xFA
	kItemHeadRattle, //0xFB
	kItemFireDrive, //0xFC
	kItemIceSmash, //0xFD
	kItemDoubleDip, //0xFE
	kItemDoubleDipP, //0xFF
	kItemCharge, //0x100
	kItemChargeP, //0x101
	kItemSuperAppeal, //0x102
	kItemSuperAppealP, //0x103
	kItemPowerPlus, //0x104
	kItemPowerPlusP, //0x105
	kItemPUpDDown, //0x106
	kItemPUpDDownP, //0x107
	kItemAllOrNothing, //0x108
	kItemAllOrNothingP, //0x109
	kItemMegaRush, //0x10A
	kItemMegaRushP, //0x10B
	kItemPowerRush, //0x10C
	kItemPowerRushP, //0x10D
	kItemPDownDUp, //0x10E
	kItemPDownDUpP, //0x10F
	kItemLastStand, //0x110
	kItemLastStandP, //0x111
	kItemDefendPlus, //0x112
	kItemDefendPlusP, //0x113
	kItemDamageDodge, //0x114
	kItemDamageDodgeP, //0x115
	kItemHpPlus, //0x116
	kItemHpPlusP, //0x117
	kItemFpPlus, //0x118
	kItemFlowerSaver, //0x119
	kItemFlowerSaverP, //0x11A
	kItemIcePower, //0x11B
	kItemSpikeShield, //0x11C
	kItemFeelingFine, //0x11D
	kItemFeelingFineP, //0x11E
	kItemZapTap, //0x11F
	kItemDoublePain, //0x120
	kItemJumpman, //0x121
	kItemHammerman, //0x122
	kItemReturnPostage, //0x123
	kItemHappyHeart, //0x124
	kItemHappyHeartP, //0x125
	kItemHappyFlower, //0x126
	kItemHpDrainBadge, //0x127
	kItemHpDrainP, //0x128
	kItemFpDrain, //0x129
	kItemFpDrainP, //0x12A
	kItemCloseCall, //0x12B
	kItemCloseCallP, //0x12C
	kItemPrettyLucky, //0x12D
	kItemPrettyLuckyP, //0x12E
	kItemLuckyDay, //0x12F
	kItemLuckyDayP, //0x130
	kItemRefund, //0x131
	kItemPityFlower, //0x132
	kItemPityFlowerP, //0x133
	kItemQuickChange, //0x134
	kItemPeekaboo, //0x135
	kItemTimingTutor, //0x136
	kItemHeartFinder, //0x137
	kItemFlowerFinder, //0x138
	kItemMoneyMoney, //0x139
	kItemItemHog, //0x13A
	kItemAttackFxR, //0x13B
	kItemAttackFxB, //0x13C
	kItemAttackFxG, //0x13D
	kItemAttackFxY, //0x13E
	kItemAttackFxP, //0x13F
	kItemChillOut, //0x140
	kItemFirstAttack, //0x141
	kItemBumpAttack, //0x142
	kItemSlowGo, //0x143
	kItemSimplifier, //0x144
	kItemUnsimplifier, //0x145
	kItemLuckyStart, //0x146
	kItemLEmblem, //0x147
	kItemWEmblem, //0x148
	kItemTripleDip, //0x149
	kItemLuckyStartP, //0x14A
	kItemAutoCommandBadge, //0x14B
	kItemMegaJump, //0x14C
	kItemMegaSmash, //0x14D
	kItemMegaQuake, //0x14E
	kItemUnusedDefendBadge, //0x14F
	kItemUnusedDefendBadgeP, //0x150
	kItemSuperCharge, //0x151
	kItemSuperChargeP, //0x152
#define BADGE_MAX kItemSuperChargeP
	kItemMax = 0xFFFFFFFF //force 4-byte
} ItemType;

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
	u16 mKeyItems[mNumKeyItems]; //0xA0
	u16 mHeldItems[mNumHeldItems]; //0x192
	u16 mStoredItems[mNumStoredItems]; //0x1BA
	u16 mBadges[mNumBadges]; //0x1FA
	u16 mEquippedBadges[mNumBadges]; //0x38A
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
u16 pouchKeyItem(s32 id);
u16 pouchHaveItem(s32 id);
u16 pouchKeepItem(s32 id);
u16 pouchHaveBadge(s32 id);
u32 pouchGetHaveItemCnt(void);
u32 pouchGetKeepItemCnt(void);
u32 pouchGetHaveBadgeCnt(void);
u32 pouchGetEquipBadgeCnt(void);
u32 pouchGetEmptyHaveItemCnt(void);
u32 pouchGetEmptyKeepItemCnt(void);


BOOL pouchGetItem(u32 itemId);
u32 pouchCheckItem(u32 itemId);