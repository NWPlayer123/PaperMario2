#include "mario_pouch.h"
#include "evt/evt_badgeshop.h"
#include "evt/evt_yuugijou.h"
#include "item_data.h"
#include "memory.h"
#include <string.h>

extern yuugijou_work* yuwp;
extern badgeshopw* bdsw;
extern ItemData itemDataTable[];

static PouchData* mpp;


//.data
s16 _party_max_hp_table[8][4] = {
	{0, 0, 0, 0},
	{10, 20, 30, 200},
	{10, 15, 25, 200},
	{20, 30, 40, 200},
	{10, 20, 30, 200},
	{15, 25, 35, 200},
	{15, 20, 30, 200},
	{15, 20, 25, 200}
};


char* pouchGetYoshiName(void) {
	if (strlen(mpp->mPartnerYoshiName)) {
		return mpp->mPartnerYoshiName;
	}
	else {
		return NULL; // return msgSearch("name_party3");
	}
}

void pouchSetYoshiName(char* name) {
	strcpy(mpp->mPartnerYoshiName, name);
}

void pouchSetPartyColor(MarioPartner partnerId, u16 color) {
	mpp->mPartyData[partnerId].mFlags = (u16)((mpp->mPartyData[partnerId].mFlags & 0x1FFF) | (color << 13));
}

s32 pouchGetPartyColor(MarioPartner partnerId) {
	return mpp->mPartyData[partnerId].mFlags >> 13;
}

u32 pouchCheckMail(s32 mailId) {
	s32 mask = (1 << (mailId % 32));
	if (mpp->mEmailReadFlags[mailId / 32] & mask) {
		return 2;
	}
	else {
		return (mpp->mEmailReceivedFlags[mailId / 32] & mask) != 0;
	}
}

void pouchOpenMail(s32 mailId) {
	mpp->mEmailReadFlags[mailId / 32] |= (1 << (mailId % 32));
}

void pouchReceiveMail(s32 mailId) {
	s32 index, shift, count;
	int i;

	shift = mailId % 32;
	index = mailId / 32;

	//increment through known IDs to add to mEmailIds
	for (i = 0, count = 0; i < 99; i++) {
		if (mpp->mEmailReceivedFlags[i / 32] & (1 << (i % 32))) {
			count++;
		}
	}
	mpp->mEmailReceivedFlags[index] |= (1 << shift);
	mpp->mEmailIds[count] = (u8)mailId;
}

s32 pouchReceiveMailCount(void) {
	s32 i, count;

	for (i = 0, count = 0; i < 99; i++) {
		if (mpp->mEmailReceivedFlags[i / 32] & (1 << (i % 32))) {
			count++;
		}
	}
	return count;
}

void pouchGetStarStone(u32 id) {
	if (!mpp->mStarPowersObtained) {
		mpp->mLastAudienceCount = 2.0f;
	}
	mpp->mStarPowersObtained |= 1 << id;
	mpp->mMaxSP = (s16)(100 * (id + 1));
	if (mpp->mMaxSP < 0) {
		mpp->mMaxSP = 0;
	}
	if (mpp->mMaxSP > 800) {
		mpp->mMaxSP = 800;
	}
}

void pouchAddKpaScore(u32 score) {
	mpp->mSuperBowserScore += score;
	if (mpp->mSuperBowserScore > 999999)
	{
		mpp->mSuperBowserScore = 999999;
	}
}

u32 pouchGetKpaScore(void) {
	return mpp->mSuperBowserScore;
}

void pouchAddKpaCoin(u8 coins) {
	mpp->mSuperBowserCoins += coins;
	if (mpp->mSuperBowserCoins >= 100) {
		mpp->mSuperBowserCoins -= 100;
	}
}

u8 pouchGetKpaCoin(void) {
	return mpp->mSuperBowserCoins;
}

void pouchMajinaiInit(u32 curseCount) {
	if (mpp->mMerleeCurseUsesRemaining < curseCount) {
		mpp->mMerleeCurseUsesRemaining = (u8)curseCount;
		mpp->mTurnsUntilMerleeActivation = -1;
		mpp->mMerleeNextCurseType = 0;
	}
}

s32 pouchArriveBadge(s16 id) {
	return badgeShop_add(bdsw->special_table, id, 1);
}

BOOL pouchUnEquipBadgeID(s32 badgeId) {
	int i;

	for (i = 0; i < 200; i++) {
		if (mpp->mEquippedBadges[i] == badgeId) {
			if (mpp->mEquippedBadges[i]) {
				mpp->mEquippedBadges[i] = 0;
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL pouchEquipBadgeID(s32 badgeId) {
	int i;

	for (i = 0; i < 200; i++) {
		if (mpp->mBadges[i] == badgeId) {
			if (mpp->mBadges[i]) {
				mpp->mEquippedBadges[i] = mpp->mBadges[i];
			}
			return TRUE;
		}
	}
	return FALSE;
}

BOOL pouchEquipCheckBadgeIndex(s32 index) {
	return mpp->mEquippedBadges[index] != 0;
}

s32 pouchEquipCheckBadge(s32 badgeId) {
	int i, count;

	count = 0;
	for (i = 0; i < 200; i++) {
		if (badgeId == mpp->mEquippedBadges[i]) {
			count++;
		}
	}
	return count;
}

BOOL pouchUnEquipBadgeIndex(s32 index) {
	if (mpp->mEquippedBadges[index] == 0) {
		return FALSE;
	}
	mpp->mEquippedBadges[index] = 0;
	return TRUE;
}

BOOL pouchEquipBadgeIndex(s32 index) {
	if (mpp->mBadges[index] == 0) {
		return FALSE;
	}
	mpp->mEquippedBadges[index] = mpp->mBadges[index];
	return TRUE;
}

u16 pouchGetStarPoint(void) {
	return mpp->mStarPoints;
}

void pouchRevisePartyParam(void) {
	PouchPartyData* party;
	int i, count;

	for (i = 0, count = 0; i < 200; i++) {
		if (mpp->mEquippedBadges[i] == kItemHpPlusP) {
			count++;
		}
	}

	for (i = 0; i < 8; i++) { //TODO: NUM_PARTNERS macro
		if (i != PARTNER_NONE) {
			party = &mpp->mPartyData[i];
			party->mBaseMaxHP = _party_max_hp_table[i][party->mHPLevel];
			party->mMaxHP = (s16)(party->mBaseMaxHP + (count * 5));
			if (party->mCurrentHP > party->mMaxHP) {
				party->mCurrentHP = party->mMaxHP;
			}
		}
	}
}

void pouchReviseMarioParam(void) {
	int i;

	mpp->mMaxHP = mpp->mBaseMaxHP;
	mpp->mMaxFP = mpp->mBaseMaxFP;

	for (i = 0; i < 200; i++) {
		switch (mpp->mEquippedBadges[i]) {
			case kItemHpPlus:
				mpp->mMaxHP += 5;
				break;
			case kItemFpPlus:
				mpp->mMaxFP += 5;
				break;
		}
	}
	if (mpp->mCurrentHP > mpp->mMaxHP) {
		mpp->mCurrentHP = mpp->mMaxHP;
	}
	if (mpp->mCurrentFP > mpp->mMaxFP) {
		mpp->mCurrentFP = mpp->mMaxFP;
	}
	mpp->mAvailableBP = mpp->mTotalBP;

	for (i = 0; i < 200; i++) {
		mpp->mAvailableBP -= itemDataTable[mpp->mEquippedBadges[i]].bp_cost;
	}
}

BOOL pouchRemoveKeepItem(s32 id, s32 index) {
	int i, j;

	if (mpp->mStoredItems[index] != id) {
		return FALSE;
	}
	if (index < 32) {
		for (i = (mNumStoredItems - index); i < 32; i++) {
			if (mpp->mStoredItems[i] == id) {
				mpp->mStoredItems[i] = 0;
				if (index < 31) {
					for (j = 31; j < 31 - index; j--) {
						mpp->mStoredItems[j] = mpp->mStoredItems[j + 1];
						mpp->mStoredItems[j + 1] = 0;
					}
				}
				return TRUE;
			}
			index++;
		}
	}
	return FALSE;
}

//TODO: this function
BOOL pouchAddKeepItem(s16 id) {
	return FALSE;
}

s16 pouchGetPartyAttackLv(MarioPartner partnerId) {
	return mpp->mPartyData[partnerId].mAttackLevel;
}

s32 pouchGetHammerLv(void) {
	s8 level = 0;

	if (pouchCheckItem(kItemUltraHammer)) {
		level = 3;
	}
	else if (pouchCheckItem(kItemSuperHammer)) {
		level = 2;
	}
	else if (pouchCheckItem(kItemHammer)) {
		level = 1;
	}
	mpp->mHammerLevel = level;
	return mpp->mHammerLevel;
}

s32 pouchGetJumpLv(void) {
	s8 level = 0;

	if (pouchCheckItem(kItemUltraBoots)) {
		level = 3;
	}
	else if (pouchCheckItem(kItemSuperBoots)) {
		level = 2;
	}
	else if (pouchCheckItem(kItemBoots)) {
		level = 1;
	}
	mpp->mJumpLevel = level;
	return mpp->mJumpLevel;
}

void pouchSetAudienceNum(f32 num) {
	mpp->mLastAudienceCount = num;
}

f32 pouchGetAudienceNum(void) {
	return mpp->mLastAudienceCount;
}

s16 pouchGetMaxAP(void) {
	return mpp->mMaxSP;
}

void pouchSetAP(s16 points) {
	mpp->mCurrentSP = points;
	if (mpp->mCurrentSP < 0) {
		mpp->mCurrentSP = 0;
	}
	if (mpp->mCurrentSP > mpp->mMaxSP) {
		mpp->mCurrentSP = mpp->mMaxSP;
	}
}

void pouchAddAP(s16 points) {
	mpp->mCurrentSP += points;
	if (mpp->mCurrentSP < 0) {
		mpp->mCurrentSP = 0;
	}
	if (mpp->mCurrentSP > mpp->mMaxSP) {
		mpp->mCurrentSP = mpp->mMaxSP;
	}
}

s16 pouchGetAP(void) {
	return mpp->mCurrentSP;
}

void pouchSetMaxFP(s16 points) {
	mpp->mMaxFP = points;
	mpp->mBaseMaxFP = mpp->mMaxFP;
}

void pouchSetFP(s16 points) {
	mpp->mCurrentFP = points;
	if (mpp->mCurrentFP > mpp->mMaxFP) {
		mpp->mCurrentFP = mpp->mMaxFP;
	}
}

s16 pouchGetMaxFP(void) {
	return mpp->mMaxFP;
}

s16 pouchGetFP(void) {
	return mpp->mCurrentFP;
}

void pouchSetPartyHP(MarioPartner partnerId, s16 points) {
	PouchPartyData* party;

	mpp->mPartyData[partnerId].mCurrentHP = points;
	party = &mpp->mPartyData[partnerId];
	if (party->mCurrentHP > party->mMaxHP) {
		party->mCurrentHP = party->mMaxHP;
	}
}

s16 pouchGetPartyHP(MarioPartner partnerId) {
	return mpp->mPartyData[partnerId].mCurrentHP;
}

void pouchSetMaxHP(s16 points) {
	mpp->mMaxHP = points;
	mpp->mBaseMaxHP = mpp->mMaxHP;
}

void pouchSetHP(s16 points) {
	mpp->mCurrentHP = points;
	if (mpp->mCurrentHP > mpp->mMaxHP) {
		mpp->mCurrentHP = mpp->mMaxHP;
	}
}

s16 pouchGetMaxHP(void) {
	return mpp->mMaxHP;
}

s16 pouchGetHP(void) {
	return mpp->mCurrentHP;
}

s16 pouchAddHP(s16 points) {
	mpp->mCurrentHP += points;
	if (mpp->mCurrentHP < 0) {
		mpp->mCurrentHP = 0;
	}
	if (mpp->mCurrentHP > mpp->mMaxHP) {
		mpp->mCurrentHP = mpp->mMaxHP;
	}
	return mpp->mCurrentHP;
}

s16 pouchAddStarPiece(s16 count) {
	mpp->mStarPieceCount += count;
	return mpp->mStarPieceCount;
}

s16 pouchGetStarPiece(void) {
	return mpp->mStarPieceCount;
}

s16 pouchSetSuperCoin(s16 count) {
	mpp->mShineSpriteCount = count;
	return mpp->mShineSpriteCount;
}

s16 pouchGetSuperCoin(void) {
	return mpp->mShineSpriteCount;
}

s16 pouchSetCoin(s16 coins) {
	mpp->mCoins = coins;
	if (mpp->mCoins < 0) {
		mpp->mCoins = 0;
	}
	if (mpp->mCoins > 999) {
		mpp->mCoins = 999;
	}
	return mpp->mCoins;
}

s16 pouchAddCoin(s16 coins) {
	mpp->mCoins += coins;
	if (mpp->mCoins < 0) {
		mpp->mCoins = 0;
	}
	if (mpp->mCoins > 999) {
		mpp->mCoins = 999;
	}
	return mpp->mCoins;
}

s16 pouchGetCoin(void) {
	return mpp->mCoins;
}





void pouchRemoveItem(u32 itemId) {

}








u32 pouchCheckItem(u32 itemId) {
	u32 itemCnt;
	int i;

	if (KEY_ITEM_MIN <= itemId <= KEY_ITEM_MAX) {
		for (i = 0, itemCnt = 0; i < mNumKeyItems; i++) {
			if (mpp->mKeyItems[i] == itemId) {
				itemCnt++;
			}
		}
		return itemCnt;
	}
	else if (ITEM_MIN <= itemId <= ITEM_MAX) {
		for (i = 0, itemCnt = 0; i < mNumHeldItems; i++) {
			if (mpp->mHeldItems[i] == itemId) {
				itemCnt++;
			}
		}
		return itemCnt;
	}
	else if (BADGE_MIN <= itemId <= BADGE_MAX) {
		for (i = 0, itemCnt = 0; i < mNumBadges; i++) {
			if (mpp->mBadges[i] == itemId) {
				itemCnt++;
			}
		}
		return itemCnt;
	}
	else {
		return 0;
	}
}

//we have obtained an item, update the appropriate data
BOOL pouchGetItem(u32 itemId) {
	switch (itemId) {
		case kItemCoin:
			if (mpp->mCoins < 999) {
				mpp->mCoins++;
			}
			return TRUE;
		case kItemPianta:
			if (yuwp->mPianta < 99999) {
				yuwp->mPianta++;
			}
			return TRUE;
		/*case kItemHeartPickup:
			if (mpp->mCurrentHP < mpp->mMaxHP) {
				mpp->mCurrentHP++;
			}
			return TRUE;*/
	}
	return FALSE;
}

u32 pouchGetEmptyKeepItemCnt(void) {
	u32 itemCnt, i;

	for (i = 0, itemCnt = 0; i < mNumStoredItems; i++) {
		if (mpp->mStoredItems[i] == 0) {
			itemCnt++;
		}
	}
	return itemCnt;
}

u32 pouchGetEmptyHaveItemCnt(void) {
	u32 itemCnt, i;

	for (i = 0, itemCnt = 0; i < GetHeldItemCount; i++) {
		if (mpp->mHeldItems[i] == 0) {
			itemCnt++;
		}
	}
	return itemCnt;
}

u32 pouchGetEquipBadgeCnt(void) {
	u32 itemCnt, i;

	for (i = 0, itemCnt = 0; i < mNumBadges; i++) {
		if (mpp->mEquippedBadges[i] != 0) {
			itemCnt++;
		}
	}
	return itemCnt;
}

u32 pouchGetHaveBadgeCnt(void) {
	u32 itemCnt, i;

	for (i = 0, itemCnt = 0; i < mNumBadges; i++) {
		if (mpp->mBadges[i] != 0) {
			itemCnt++;
		}
	}
	return itemCnt;
}

u32 pouchGetKeepItemCnt(void) {
	u32 itemCnt, i;

	for (i = 0, itemCnt = 0; i < mNumStoredItems; i++) {
		if (mpp->mStoredItems[i] != 0) {
			itemCnt++;
		}
	}
	return itemCnt;
}

u32 pouchGetHaveItemCnt(void) {
	u32 itemCnt, i;
	
	for (i = 0, itemCnt = 0; i < mNumHeldItems; i++) {
		if (mpp->mHeldItems[i] != 0) {
			itemCnt++;
		}
	}
	return itemCnt;
}

s16 pouchHaveBadge(s32 id) {
	return mpp->mBadges[id];
}

s16 pouchKeepItem(s32 id) {
	return mpp->mStoredItems[id];
}

s16 pouchHaveItem(s32 id) {
	return mpp->mHeldItems[id];
}

s16 pouchKeyItem(s32 id) {
	return mpp->mKeyItems[id];
}

void pouchInit(void) {
	int i;

	mpp = __memAlloc(HEAP_DEFAULT, sizeof(PouchData));
	memset(mpp, 0, sizeof(PouchData));
	for (i = 0; i < mNumKeyItems; i++) {
		mpp->mKeyItems[i] = kNullItem;
	}
	for (i = 0; i < mNumHeldItems; i++) {
		mpp->mHeldItems[i] = kNullItem;
	}
	for (i = 0; i < mNumStoredItems; i++) {
		mpp->mStoredItems[i] = kNullItem;
	}
	for (i = 0; i < mNumBadges; i++) {
		mpp->mBadges[i] = kNullItem;
	}
	for (i = 0; i < mNumBadges; i++) {
		mpp->mEquippedBadges[i] = kNullItem;
	}
	mpp->mMaxHP = 10;
	mpp->mBaseMaxHP = 10;
	mpp->mMaxFP = 5;
	mpp->mBaseMaxFP = 5;
	mpp->mAvailableBP = 3;
	mpp->mTotalBP = 3;
	mpp->mCurrentHP = mpp->mMaxHP;
	mpp->mCurrentFP = mpp->mMaxFP;
	mpp->mCurrentSP = 0;
	mpp->field_0x7E = 0;
	mpp->mMaxSP = 0;
	mpp->field_0x80 = 0;
	mpp->mLastAudienceCount = 0.0f;
	mpp->mRank = 0;
	mpp->mLevel = 1;
	mpp->mJumpLevel = 0;
	mpp->mHammerLevel = 0;
	for (i = 0; i < 8; i++) {
		mpp->mPartyData[i].mFlags = 0;
		mpp->mPartyData[i].mBaseMaxHP = 10;
		mpp->mPartyData[i].mMaxHP = mpp->mPartyData[i].mBaseMaxHP;
		mpp->mPartyData[i].mCurrentHP = mpp->mPartyData[i].mBaseMaxHP;
		mpp->mPartyData[i].mAttackLevel = 0;
		mpp->mPartyData[i].mTechLevel = 0;
	}
	strcpy(mpp->mPartnerYoshiName, "チビヨッシー"); //TODO: US
}

PouchData* pouchGetPtr(void) {
	return mpp;
}