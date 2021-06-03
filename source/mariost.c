#include "mariost.h"

#include "battle/battle.h"

#include "drv/animdrv.h"
#include "drv/arcdrv.h"
#include "drv/bgdrv.h"
#include "drv/camdrv.h"
#include "drv/casedrv.h"
#include "drv/dispdrv.h"
#include "drv/effdrv.h"
#include "drv/envdrv.h"
#include "drv/extdrv.h"
#include "drv/fadedrv.h"
#include "drv/hitdrv.h"
#include "drv/icondrv.h"
#include "drv/imgdrv.h"
#include "drv/itemdrv.h"
#include "drv/lightdrv.h"
#include "drv/mapdrv.h"
#include "drv/mobjdrv.h"
#include "drv/msgdrv.h"
#include "drv/npcdrv.h"
#include "drv/offscreendrv.h"
#include "drv/seqdrv.h"
#include "drv/shadowdrv.h"
#include "drv/swdrv.h"
#include "drv/windowdrv.h"

#include "evt/evt_badgeshop.h"
#include "evt/evt_johoya.h"
#include "evt/evt_yuugijou.h"

#include "mgr/arammgr.h"
#include "mgr/cardmgr.h"
#include "mgr/dvdmgr.h"
#include "mgr/evtmgr.h"
#include "mgr/filemgr.h"
#include "mgr/fontmgr.h"
#include "mgr/winmgr.h"

#include "sdk/DEMOInit.h"

#include "win/win_main.h"

#include "countdown.h"
#include "error_handler.h"
#include "mario.h"
#include "memory.h"
#include "nameent.h"
#include "pmario_sound.h"
#include "romfont.h"
#include "sound.h"
#include "statuswindow.h"
#include "system.h"

#include <dolphin/vi.h>
#include <string.h>

extern void DEMOPadInit(void);
extern BOOL g_bFirstSmartAlloc;

//.bss
static u8 stack[0x1000];
OSThread DvdCheckThread;
GlobalWork marioSt;

//.sdata
GlobalWork* gp = &marioSt;
BOOL aoff_trg[4];
OSTime aoff_time[4];

//.sbss
BOOL DvdCheckThreadOn; //DvdCheckTreadOn
s32 _mariostSystemLevel;
static OSTime none_key;

//.data
GXRenderModeObj sRMObjHReso = {
	VI_TVMODE_NTSC_INT, //viTVmode
	0x260, //fbWidth
	0x1C0, //efbHeight
	0x1C0, //xfbHeight
	0x1B, //viXOrigin
	0x10, //viYOrigin
	0x29A, //viWidth
	0x1C0, //viHeight
	VI_XFBMODE_DF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj sRMObjHReso_prog = {
	VI_TVMODE_NTSC_PROG, //viTVmode
	0x260, //fbWidth
	0x1C0, //efbHeight
	0x1C0, //xfbHeight
	0x1B, //viXOrigin
	0x10, //viYOrigin
	0x29A, //viWidth
	0x1C0, //viHeight
	VI_XFBMODE_SF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj GXNtsc480IntDfMarioSt = {
	VI_TVMODE_NTSC_INT, //viTVmode
	0x260, //fbWidth
	0x1E0, //efbHeight
	0x1E0, //xfbHeight
	0x1E, //viXOrigin
	0x0, //viYOrigin
	0x294, //viWidth
	0x1E0, //viHeight
	VI_XFBMODE_DF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj GXNtsc480ProgMarioSt = {
	VI_TVMODE_NTSC_PROG, //viTVmode
	0x260, //fbWidth
	0x1E0, //efbHeight
	0x1E0, //xfbHeight
	0x1E, //viXOrigin
	0x0, //viYOrigin
	0x294, //viWidth
	0x1E0, //viHeight
	VI_XFBMODE_SF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

//local prototypes
void viPostCallback(u32 retraceCount);
void* gcDvdCheckThread(void* param);
void gcRumbleCheck(void);
void gcResetCheck(void);

void marioStInit(void) {
	if (!OSIsRestart() || (OSGetResetCode() & OS_RESET_SHUTDOWN)) {
		DEMOInit(&GXNtsc480IntDfMarioSt);
	}
	else {
		switch (OSGetResetCode() & OS_RESET_HOTRESET) {
		case 0:
			DEMOInit(&GXNtsc480IntDfMarioSt);
			break;
		case 1:
			DEMOInit(&GXNtsc480ProgMarioSt);
			break;
		}
	}
	DEMOEnableGPHangWorkaround(5);
	setupErrorHandler();
	DEMOPadInit();
	GXSetCopyClear((GXColor) { 0, 0, 0, 0 }, 0xFFFFFF);
	__GXSetIndirectMask(0);
	memset(gp, 0, sizeof(GlobalWork));
	gp->fbWidth = DEMOGetRenderModeObj()->fbWidth;
	gp->efbHeight = DEMOGetRenderModeObj()->efbHeight;
	gp->field_0x1294 = 1;
	gp->field_0x1274 = 0;

	gp->mLastFrameRetraceLocalTime = 0;
	gp->mLastFrameRetraceDeltaTime = 0;

	gp->mLastFrameRetraceTime = OSGetTime();

	gp->mAnimationTimeNoBattle = 0;
	gp->mAnimationTimeInclBattle = 0;

	gp->field_0x60 = 0;
	gp->field_0x58 = 0;
	gp->field_0x50 = 0;
	gp->field_0x48 = 0;

	gp->mNextMapChangeFadeOutType = 9;
	gp->mNextMapChangeFadeOutDuration = 300;
	gp->mNextMapChangeFadeInType = 10;
	gp->mNextMapChangeFadeInDuration = 300;
	gp->mNextAreaChangeFadeOutType = 9;
	gp->mNextAreaChangeFadeOutDuration = 300;
	gp->mNextAreaChangeFadeInType = 10;
	gp->mNextAreaChangeFadeInDuration = 300;
	if ((OSGetFontEncode() == OS_FONT_ENCODE_SJIS) && !(gp->mFlags & 0x1000))
		gp->mLanguage = 0;
	else
		gp->mLanguage = 1;
	gp->mFPS = 60;
	gp->mSystemLevelFlags = 0;
	badgeShop_init(); //Badge Shop
	yuugijou_init(); //Pianta Parlor
	johoya_init(); //?????
	gp->mpMapAlloc = __memAlloc(HEAP_DEFAULT, 660 * 1024);
	VISetPostRetraceCallback(viPostCallback);
	romFontInit();
	OSCreateThread(&DvdCheckThread, gcDvdCheckThread, NULL, (void*)((u32)&stack + sizeof(stack)), sizeof(stack), 16, OS_THREAD_ATTR_DETACH);
	DvdCheckThreadOn = TRUE;
	OSResumeThread(&DvdCheckThread);
	DVDMgrInit();
	psndInit();
	aramMgrInit();
	fileInit();
	smartInit();
	dispInit();
	camInit();
	fontmgrInit();
	windowInit();
	mapInit();
	hitInit();
	itemInit();
	iconInit();
	fadeInit();
	bgInit();
	shadowInit();
	evtmgrInit();
	animInit();
	msgInit();
	npcInit();
	mobjInit();
	effInit();
	marioInit();
	caseInit();
	imgInit();
	lightInit();
	offscreenInit();
	arcInit();
	extInit();
	swInit();
	cardInit();
	winInit();
	statusWinInit();
	envInit();
	countDownInit();
	nameEntInit();
	winMgrInit();
	seqInit_MARIOSTORY();
	seqSetSeq(0, NULL, NULL);
	gp->field_0x1274 = 0;
	if (OSGetSoundMode() == OS_SOUND_MODE_MONO) {
		SoundSetOutputMode(SND_OUTPUTMODE_MONO);
	}
	else {
		SoundSetOutputMode(SND_OUTPUTMODE_STEREO);
	}
}

void marioStMain(void) {
	int i;

	while (gp->mDVDError) {
		OSYieldThread();
	}
	makeKey();
	seqMain();
	cardMain();
	bgMain();
	shadowMain();
	animMain();
	battleMain();
	camMain();
	windowMain();
	mapMain();
	hitMain();
	evtmgrMain();
	mapDisp();
	marioMain();
	mobjMain();
	npcMain();
	fadeMain();
	imgMain();
	lightMain();
	offscreenMain();
	itemMain();
	caseMain();
	iconMain();
	extMain();
	psndMain();
	winMain();
	statusWinMain();
	envMain();
	countDownMain();
	nameEntMain();
	winMgrMain();
	effMain();
	gcResetCheck();
	if (gp->mFlags & 0x1000) {
		for (i = 0; i < 4; i++) {
			if (gp->mDir[i]) break;
			if (gp->mButton[i]) break;
			if (gp->mStickX[i]) break;
			if (gp->mStickY[i]) break;
			if (gp->mSubStickX[i]) break;
			if (gp->mSubStickY[i]) break;
			if (gp->mTriggerL[i]) break;
			if (gp->mTriggerR[i]) break;
		}
	}
	if (i >= 4) {
		if (OSTicksToSeconds(OSGetTime() - none_key) > 120 && (seqGetSeq() == 2 || seqGetSeq() == 4)) {
			gp->mFlags |= 0x2000u;
		}
	}
	else {
		none_key = OSGetTime();
	}
}

void marioStDisp(void) {
	camDraw();
	g_bFirstSmartAlloc = 0;
}

void marioStSystemLevel(s32 level) {
	switch (level) {
		case 0:
			_mariostSystemLevel = level;
			gp->mSystemLevelFlags &= ~0xF;
			evtStartAll(0xEF);
			break;
		case 1:
			_mariostSystemLevel = level;
			gp->mSystemLevelFlags &= ~0xE;
			gp->mSystemLevelFlags |= 1;
			evtStopAll(1);
			break;
		case 2:
			_mariostSystemLevel = level;
			gp->mSystemLevelFlags &= ~0xC;
			gp->mSystemLevelFlags |= 3;
			evtStopAll(2);
			break;
		case 3:
			_mariostSystemLevel = level;
			gp->mSystemLevelFlags &= ~8;
			gp->mSystemLevelFlags |= 7;
			evtStopAll(0x10);
			break;
		case 4:
			_mariostSystemLevel = level;
			gp->mSystemLevelFlags |= 0xF;
			evtStopAll(0xEF);
			break;
	}
}

s32 marioStGetSystemLevel(void) {
	return gp->mSystemLevelFlags;
}

void viPostCallback(u32 retraceCount) {

}

//TODO: fuck up float conversions for 1:1 cuz this is cleaner
void* gcDvdCheckThread(void* param) {
	const char* message;
	s32 msgId;
	f32 width;
	Mtx44 mtx;

	while (1) {
		msgId = 0;
		switch (DVDGetDriveStatus()) {
			case DVD_STATE_COVER_OPEN:
				msgId = 5;
				gp->mDVDError = TRUE;
				break;
			case DVD_STATE_NO_DISK:
				msgId = 6;
				gp->mDVDError = TRUE;
				break;
			case DVD_STATE_WRONG_DISK:
				msgId = 6;
				gp->mDVDError = TRUE;
				break;
			case DVD_STATE_RETRY:
				msgId = 7;
				gp->mDVDError = TRUE;
				break;
			case DVD_RESULT_FATAL_ERROR:
				msgId = 8;
				gp->mDVDError = TRUE;
				break;
			default:
				gp->mDVDError = FALSE;
				break;
			case DVD_STATE_WAITING:
				break;
		}
		if (gp->mDVDError) {
			SoundOpenCover();
		}
		else {
			SoundCloseCover();
		}
		if (gp->mDVDError) {
			if (cardIsExec()) {
				gp->mDVDError = 0;
			}
		}
		if (gp->mDVDError) {
			DEMOBeforeRender();
			if (msgId) {
				message = romFontGetMessage(msgId);
				width = (f32)romFontGetWidth(message);
				MTXPerspective(mtx, 25.0f, 1.2666667f, 1.0f, 10000.0f);
				GXSetProjection(mtx, GX_PERSPECTIVE);
				romFontPrintGX(-width * 0.5f, 60.0f, 1.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF}, message);
			}
			DEMODoneRender();
			makeKey();
			gcResetCheck();
		}
		OSYieldThread();
	}
}

void gcRumbleCheck(void) {
	/*OSTime start;
	volatile BOOL array[4] = { 0 };
	int i;

	start = OSGetTime();
	for (i = 0; i < 4; i++) {
		if (gp->field_0x12E8[i]) {
			if (OSTicksToMilliseconds(start - gp->field_0x12F0[i]) > 30000) {
				gp->field_0x12EC[i] = 0;
				gp->field_0x12E8[i] = 0;
			}
			array[i] = TRUE;
		}
	}*/
}

void gcResetCheck(void) {
	u32 code;

	if (gp->field_0x1278) {
		VISetBlack(TRUE);
		VIFlush();
		VIWaitForRetrace();
		VIWaitForRetrace();
		VIWaitForRetrace();
		psndExit();
		DVDMgrDelete();
		if ((OSGetCurrentThread() != &DvdCheckThread) && DvdCheckThreadOn) {
			OSCancelThread(&DvdCheckThread);
			DvdCheckThreadOn = FALSE;
		}
		while (cardIsExec()) {
			cardMain();
			VIWaitForRetrace();
		}
		if (gp->field_0x1278 != 2) {
			if (!(gp->mFlags & 4)) {
				code = OS_RESET_SHUTDOWN;
			}
			else {
				if (!(gp->mFlags & 8)) {
					code = OS_RESET_RESTART;
				}
				else {
					code = OS_RESET_HOTRESET;
				}
			}
			OSResetSystem(OS_RESET_RESTART, code, FALSE);
		}
		else {
			OSResetSystem(OS_RESET_HOTRESET, 0, TRUE);
		}
		while (1) {}
	}
}