#include "battle/battle_ac.h"
#include "battle/battle_unit.h"
#include "battle/ac/ac_table.h"
#include "drv/dispdrv.h"

extern ActionCommandEntry ActionCommandList[];
extern BattleWork* _battleWorkPointer;

//local prototypes
u32 BattleACPadCheckRecordTrigger(s32 chan, s32 mask);


void BattleActionCommandManagerInit(BattleWork* work) {
	BtlPad_WorkInit(&work->mAcManagerWork.mPadWork);
	work->mAcManagerWork.mMainFunction = NULL;
	work->mAcManagerWork.mResultFunction = NULL;
	work->mAcManagerWork.mDispFunction = NULL;
	work->mAcManagerWork.mDeleteFunction = NULL;
	work->mAcManagerWork.field_0x288 = -1;
}

void BattleActionCommandManager(BattleWork* work) {
	BtlPad_WorkUpdate(&work->mAcManagerWork.mPadWork, 0);
	if (work->mAcManagerWork.mMainFunction) {
		if (!work->mAcManagerWork.mMainFunction(work)) {
			work->mAcManagerWork.mMainFunction = NULL;
		}
		if (work->mAcManagerWork.mDispFunction) {
			dispEntry(kCam2d, 1, work->mAcManagerWork.mDispFunction, work, 900.0f);
		}
	}
}

s32 BattleActionCommandResult(BattleWork* work) {
	if (work->mAcManagerWork.mResultFunction) {
		return work->mAcManagerWork.mResultFunction(work);
	}
	else {
		return work->mAcManagerWork.mAcResult & 2;
	}
}

void BattleActionCommandDeclareACResult(BattleWork* work, BattleWeapon* weapon, s32 result) {
	if (result == -1) {
		//BattleAudience_Case_ActionCommandBad(weapon);
	}
	else {
		work->mImpendingWeaponBonuses.mWeapon = weapon;
		work->mImpendingWeaponBonuses.mSpAcSuccessMultiplier = ((f32)result * 0.25f) + 0.5f;
		work->mImpendingWeaponBonuses.field_0x9 = weapon->field_0x19;
		work->mImpendingWeaponBonuses.mBingoSlotChance = weapon->mBingoSlotChance;
		//BattleAudience_Case_ActionCommandGood();
	}
}

void BattleActionCommandSetup(BattleWork* work, s32 id, BattleWorkUnit* unit, s32 a4, s32 a5) {
	ActionCommandEntry* entry; // r6

	work->mAcManagerWork.mAcUnit = unit;
	work->mAcManagerWork.field_0x4 = a4;
	work->mAcManagerWork.mAcState = 0;
	work->mAcManagerWork.mResultCount = 0;
	work->mAcManagerWork.mAcResult = 1;
	work->mAcManagerWork.mMainFunction = 0;
	work->mAcManagerWork.mDispFunction = 0;
	work->mAcManagerWork.mResultFunction = 0;
	work->mAcManagerWork.mDeleteFunction = 0;
	work->mAcManagerWork.field_0x8 = a5;
	work->mAcManagerWork.field_0x30 = 0;
	entry = ActionCommandList;
	while (entry++->id) {
		if (entry->id == id) {
			work->mAcManagerWork.mMainFunction = entry->main;
			if (!(work->mAcManagerWork.mAcUnit->mTokenFlags & 0x10)){
				work->mAcManagerWork.mDispFunction = entry->disp;
			}
			work->mAcManagerWork.mResultFunction = entry->result;
			work->mAcManagerWork.mDeleteFunction = entry->delete;
			return;
		}
	}
}

void BattleActionCommandStart(BattleWork* work) {
	work->mAcManagerWork.mAcState = 100;
}

void BattleActionCommandStop(BattleWork* work) {
	if (work->mAcManagerWork.mDeleteFunction) {
		work->mAcManagerWork.mDeleteFunction(work);
	}
}

//TODO: a2 is some pointer
s32 BattleActionCommandCheckDefence(BattleWorkUnit* unit, int a2) {
	return -2;
}

u32 BattleACPadCheckRecordTrigger(s32 chan, s32 mask) {
	return _battleWorkPointer->mAcManagerWork.mPadWork.mButtonsPressedHistory[chan] & mask;
}

s32 BattleActionCommandGetDefenceResult(void) {
	return _battleWorkPointer->mAcManagerWork.mDefenseResult;
}

void BattleActionCommandResetDefenceResult(void) {
	_battleWorkPointer->mAcManagerWork.mDefenseResult = 0;
	BtlPad_WorkInit(&_battleWorkPointer->mAcManagerWork.mPadWork);
}

s8 BattleActionCommandGetDifficulty(BattleWork* work) {
	return work->mAcManagerWork.mAcDifficulty;
}

void BattleActionCommandSetDifficulty(BattleWork* work, BattleWorkUnit* unit, s32 difficulty) {
	s32 adjusted;

	work->mAcManagerWork.mBaseAcDifficulty = difficulty;
	adjusted = difficulty - unit->mBadgesEquipped.mSimplifier + unit->mBadgesEquipped.mUnsimplifier;
	if (adjusted < 0) {
		adjusted = 0;
	}
	if (adjusted > 6) {
		adjusted = 6;
	}
	work->mAcManagerWork.mAcDifficulty = adjusted;
}





