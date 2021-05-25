#pragma once

#include <dolphin/os.h>
#include "drv/npcdrv.h"
#include "evt/evt_lottery.h"
#include "countdown.h"

typedef struct marioStruct {
	u32 mFlags; //0x0
	s32 mFPS; //0x04, TODO double check s32 cuz double conversion
	u8 field_0x8[0x10 - 0x8]; //0x8
	BOOL mDVDError; //0x10
	u32 isBattleInit; //0x14
	s32 mSystemLevelFlags; //0x18
	u32 field_0x1C; //0x1C
	OSTime mLastFrameRetraceLocalTime; //0x20
	OSTime mLastFrameRetraceDeltaTime; //0x28
	OSTime mLastFrameRetraceTime; //0x30
	OSTime mAnimationTimeInclBattle; //0x38
	OSTime mAnimationTimeNoBattle; //0x40
	OSTime field_0x48; //0x48
	OSTime field_0x50; //0x50
	OSTime field_0x58; //0x58
	OSTime field_0x60; //0x60
	CountdownWork countdown; //0x68
	lottery_info mLotteryInfo; //0xA8
	u8 field_0xE8[0xF8 - 0xE8]; //0xE8
	s32 mNextMapChangeFadeOutType; //0xF8
	s32 mNextMapChangeFadeOutDuration; //0xFC
	s32 mNextMapChangeFadeInType; //0x100
	s32 mNextMapChangeFadeInDuration; //0x104
	s32 mNextAreaChangeFadeOutType; //0x108
	s32 mNextAreaChangeFadeOutDuration; //0x10C
	s32 mNextAreaChangeFadeInType; //0x110
	s32 mNextAreaChangeFadeInDuration; //0x114
	BOOL mbAreaChanged; //0x118
	u8 field_0x11C[0x12C - 0x11C]; //0x11C
	char mCurrentMapName[0x10]; //0x12C
	char mCurrentAreaName[0x20]; //0x13C
	OSModuleHeader* mpRelFileBase; //0x15C
	OSModuleInfo* mpMapAlloc; //0x160
	u32 field_0x164; //0x164
	FieldBattleData* mpFieldBattleData; //0x168
	s32 mLanguage; //0x16C
	u16 fbWidth; //0x170
	u16 efbHeight; //0x172
	u32 mGSW0; //0x174, first entry is a u32
	u32 mGSFW[0x100]; //0x178
	u8 mGSW[0x800]; //0x578
	u32 mLSWF[0x10]; //0xD78
	u8 mLSW[0x400]; //0xDB8
	u8 field_0x11B8[0x1274 - 0x11B8]; //0x11B8
	u32 field_0x1274; //0x1274
	u32 field_0x1278; //0x1278
	u8 field_0x127C[0x1294 - 0x127C]; //0x127C
	u32 field_0x1294; //0x1294
	u8 field_0x1298[0x12E8 - 0x1298]; //0x1298
	u8 field_0x12E8[4]; //0x12E8
	u8 field_0x12EC[4]; //0x12EC
	OSTime field_0x12F0[4];
	//u8 field_0x1300[0x1310 - 0x1300]; //0x1300
	u8 field_0x1310[4]; //0x1310
	OSTick mDeltaGame; //0x1314
	OSTick mDeltaRender; //0x1318
	u8 field_0x131C[0x1324 - 0x131C]; //0x131C
// system.c "key" controller data, TODO sub-struct?
	u32 field_0x1324; //0x1324
	u32 mButton[4]; //0x1328
	u32 mButtonTrg[4]; //0x1338
	u32 mButtonRep[4]; //0x1348
	u32 mButtonRepeatDelay[4]; //0x1358,
	u32 mButtonUp[4]; //0x1368
	u32 mDir[4]; //0x1378, TODO mDirNrm?
	u32 mDirTrg[4]; //0x1388
	u32 mDirRep[4]; //0x1398
	u32 mDirRepeatDelay[4]; //0x13A8
	s8 mStickX[4]; //0x13B8
	s8 mStickY[4]; //0x13BC
	s8 mSubStickX[4]; //0x13C0
	s8 mSubStickY[4]; //0x13C4
	u8 mTriggerL[4]; //0x13C8
	u8 mTriggerR[4]; //0x13CC
	u8 mRumbleStatus[4]; //0x13D0
	u8 field_0x13D4[4]; //0x13D4
} marioStruct;

//u32 test = sizeof(marioSt); // 0x13D8/5080
void marioStInit(void);
void marioStMain(void);
void marioStDisp(void);
void marioStSystemLevel(s32 level);
s32 marioStGetSystemLevel(void);
