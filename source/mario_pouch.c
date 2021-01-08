#include "mario_pouch.h"
#include "evt/evt_yuugijou.h"
#include "memory.h"
#include <string.h>

extern yuugijou_work* yuwp;

static PouchData* mpp;

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
		case kCoin:
			if (mpp->mCoins < 999) {
				mpp->mCoins++;
			}
			return TRUE;
		case kPianta:
			if (yuwp->mPianta < 99999) {
				yuwp->mPianta++;
			}
			return TRUE;
		/*case kHeartPickup:
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

ItemType1 pouchHaveBadge(s32 id) {
	return mpp->mBadges[id];
}

ItemType1 pouchKeepItem(s32 id) {
	return mpp->mStoredItems[id];
}

ItemType1 pouchHaveItem(s32 id) {
	return mpp->mHeldItems[id];
}

ItemType1 pouchKeyItem(s32 id) {
	return mpp->mKeyItems[id];
}

void pouchInit(void) {
	int i;

	mpp = __memAlloc(0, sizeof(PouchData));
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