#include "battle/battle_pad.h"
#include "battle/battle.h"

extern BattleWork* _battleWorkPointer;

u32 BattlePadMultiCheckRepeat(s32 chan, s32 mask) {
	return _battleWorkPointer->mPadWork[chan].mButtonsHeldHistory[0] & mask;
}

u32 BattlePadCheckRepeat(s32 mask) {
	return BattlePadMultiCheckRepeat(0, mask);
}

u32 BattlePadMultiCheckUp(s32 chan, s32 mask) {
	return _battleWorkPointer->mPadWork[chan].mButtonsReleasedHistory[0] & mask;
}

u32 BattlePadCheckUp(s32 mask) {
	return BattlePadMultiCheckUp(0, mask);
}

u32 BattlePadMultiCheckNow(s32 chan, s32 mask) {
	return _battleWorkPointer->mPadWork[chan].mButtonsHistory[0] & mask;
}

u32 BattlePadCheckNow(s32 mask) {
	return BattlePadMultiCheckNow(0, mask);
}

u32 BattlePadMultiCheckRecordTrigger(s32 chan, s32 frame, s32 mask) {
	return _battleWorkPointer->mPadWork[chan].mButtonsPressedHistory[frame] & mask;
}

u32 BattlePadCheckRecordTrigger(s32 frame, s32 mask) {
	return BattlePadMultiCheckRecordTrigger(0, frame, mask);
}

u32  BattlePadMultiCheckTrigger(s32 chan, s32 mask) {
	return _battleWorkPointer->mPadWork[chan].mButtonsPressedHistory[0] & mask;
}

u32 BattlePadCheckTrigger(s32 mask) {
	return BattlePadMultiCheckTrigger(0, mask);
}

u32 BattlePadGetNow(void) {
	return _battleWorkPointer->mPadWork[0].mButtonsHistory[0];
}

u32 BattlePadGetTrigger(void) {
	return _battleWorkPointer->mPadWork[0].mButtonsPressedHistory[0];
}

void BtlPad_WorkUpdate(BattleWorkPad* pad, s32 chan) {
	int i, repeat;

	//TODO: inline memcpy?
	*(u32*)((u32)&pad->mPadData[1] + 0) = *(u32*)((u32)&pad->mPadData[0] + 0);
	*(u32*)((u32)&pad->mPadData[1] + 4) = *(u32*)((u32)&pad->mPadData[0] + 4);
	*(u32*)((u32)&pad->mPadData[1] + 8) = *(u32*)((u32)&pad->mPadData[0] + 8);

	for (i = 0; i < 29; i++) {
		pad->mButtonsPressedHistory[30 - i - 1] = pad->mButtonsPressedHistory[30 - i];
		pad->mButtonsHeldHistory[30 - i - 1] = pad->mButtonsHeldHistory[30 - i];
		pad->mButtonsHistory[30 - i - 1] = pad->mButtonsHistory[30 - i];
	}

	if (pad->mPadData[0].stickY >= 30) {
		pad->mButtonsHistory[0] |= (1 << 16);
	}
	if (pad->mPadData[0].stickY <= -30) {
		pad->mButtonsHistory[0] |= (1 << 17);
	}

	if (pad->mPadData[0].stickX <= -30) {
		pad->mButtonsHistory[0] |= (1 << 18);
	}
	if (pad->mPadData[0].stickX >= 30) {
		pad->mButtonsHistory[0] |= (1 << 19);
	}

	if (pad->mPadData[0].substickY >= 30) {
		pad->mButtonsHistory[0] |= (1 << 20);
	}
	if (pad->mPadData[0].substickY <= -30) {
		pad->mButtonsHistory[0] |= (1 << 21);
	}

	if (pad->mPadData[0].substickX <= -30) {
		pad->mButtonsHistory[0] |= (1 << 22);
	}
	if (pad->mPadData[0].substickX >= 30) {
		pad->mButtonsHistory[0] |= (1 << 23);
	}

	pad->mButtonsPressedHistory[0] |= pad->mButtonsHistory[0] & ~pad->mButtonsHistory[1] & 0xFFFF0000;
	
	if (pad->mButtonsHistory[0]) {
		if (pad->mButtonsHistory[0] == pad->mButtonsHistory[1]) {
			repeat = 8;
			pad->mHoldRepeatFrameCounter++;
			if (pad->mHoldRepeatState == 1) {
				repeat = 16;
			}

			if (pad->mHoldRepeatFrameCounter >= repeat) {
				pad->mButtonsHeldHistory[0] |= pad->mButtonsHistory[0] & 0xFFFF0000;
				pad->mHoldRepeatState = 2;
				pad->mHoldRepeatFrameCounter = 0;
			}
		}
		else {
			pad->mButtonsHeldHistory[0] |= pad->mButtonsHistory[0] & 0xFFFF0000;
			pad->mButtonsHeldHistory[0] &= pad->mButtonsPressedHistory[0] & 0xFFFF0000 | 0xFFFF;
			pad->mHoldRepeatState = 1;
			pad->mHoldRepeatFrameCounter = 0;
		}
	}
	else {
		pad->mButtonsHeldHistory[0] = 0;
		pad->mHoldRepeatState = 0;
		pad->mHoldRepeatFrameCounter = 0;
	}
	pad->mButtonsReleasedHistory[0] |= pad->mButtonsHistory[1] & (pad->mButtonsHistory[1] ^ pad->mButtonsHistory[0]) & 0xFFFF0000;
}

void BtlPad_WorkInit(BattleWorkPad* pad) {
	int i;

	for (i = 0; i < 2; i++) {
		pad->mPadData[i].button = 0;
		pad->mPadData[i].stickX = 0;
		pad->mPadData[i].stickY = 0;
		pad->mPadData[i].substickX = 0;
		pad->mPadData[i].substickY = 0;
		pad->mPadData[i].triggerLeft = 0;
		pad->mPadData[i].triggerRight = 0;
		pad->mPadData[i].analogA = 0;
		pad->mPadData[i].analogB = 0;
		pad->mPadData[i].err = 0;
	}

	for (i = 0; i < 30; i++) {
		pad->mButtonsPressedHistory[i] = 0;
		pad->mButtonsHeldHistory[i] = 0;
		pad->mButtonsHistory[i] = 0;
	}

	pad->mHoldRepeatState = 0;
	pad->mHoldRepeatFrameCounter = 0;
}

void BattlePadManager(void) {
	int i;

	for (i = 0; i < 4; i++) {
		BtlPad_WorkUpdate(&_battleWorkPointer->mPadWork[i], i);
	}
}

void BattlePadInit(void) {
	int i;

	for (i = 0; i < 4; i++) {
		BtlPad_WorkInit(&_battleWorkPointer->mPadWork[i]);
	}
}