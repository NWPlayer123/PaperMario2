#include "evt/evt_lottery.h"
#include "mgr/evtmgr.h"
#include "mgr/evtmgr_cmd.h"
#include <dolphin/os.h>

extern marioStruct* gp;

//.sbss
BOOL dbg_lotteryinfo;

//local prototypes
EvtStatus evt_lottery(EvtEntry* evt, BOOL firstCall);

lottery_info* lotteryGetPtr(void) {
	return &gp->mLotteryInfo;
}

EvtStatus evt_lottery(EvtEntry* evt, BOOL firstCall) {
	lottery_info* LotteryInfo;

	s32* args = evt->currCmdArgs;
	s32 arg1 = evtGetValue(evt, args[0]); //r25
	OSTime start = OSGetTime();
	dbg_lotteryinfo = TRUE;

	LotteryInfo = &gp->mLotteryInfo;

	LotteryInfo->mFlags &= ~0x1000;
	LotteryInfo->mFlags &= ~0x2000;
	LotteryInfo->mFlags &= ~0x4000;
	LotteryInfo->mFlags &= ~0x8000;

	if (LotteryInfo->mFlags & 8) {

	}
















	return EVT_RETURN_DONE2;
}
