#include "win/win_main.h"
#include "data/item_data.h"
#include "drv/seqdrv.h"
#include "evt/evt_cmd.h"
#include "mgr/evtmgr.h"
#include "mario/mario.h"
#include "memory.h"
#include "party.h"
#include "system.h"
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include <string.h>

extern ItemData* itemDataTable;
extern int sprintf(char* str, const char* format, ...);

//.sbss
static WindowWork* wp;
void* winTexTpl; //TODO: re-type (TPL prob)
static s32 cnt;

//local prototypes
void winDisp(CameraId cameraId, void* param);
void winDispKoopa(CameraId cameraId, void* param);

WindowWork* winGetPtr(void) {
	return wp;
}

void winInit(void) {
	wp = __memAlloc(HEAP_DEFAULT, sizeof(WindowWork));
	memset(wp, 0, sizeof(WindowWork));
}

void winReInit(void) {
	memset(wp, 0, sizeof(WindowWork));
}

BOOL winCheck(void) {
	s32 state;

	if (wp == NULL) {
		return FALSE;
	}
	if (wp->flags & 0x1000) {
		return FALSE;
	}
	state = wp->menuState;
	if (state && state < 200) {
		return TRUE;
	}
	return FALSE;
}

void winOpenEnable(void) {
	if (wp) {
		wp->flags &= ~1;
	}
}

void winOpenDisable(void) {
	if (wp) {
		wp->flags |= 1;
	}
}

void winMain(void) {
	MarioWork* mario;

	mario = marioGetPtr();
	if (!(wp->flags & 1) && seqGetSeq() == 2 && !seqCheckSeq()) {
		wp->buttonTrg = keyGetButtonTrg(0);
		wp->buttonRep = keyGetButtonRep(0);
		wp->dirTrg = keyGetDirTrg(0);
		wp->dirRep = keyGetDirRep(0);

		if (keyGetButtonTrg(0) & PAD_BUTTON_LEFT) {
			wp->dirTrg |= DEMO_STICK_LEFT;
		}
		if (keyGetButtonTrg(0) & PAD_BUTTON_RIGHT) {
			wp->dirTrg |= DEMO_STICK_RIGHT;
		}
		if (keyGetButtonTrg(0) & PAD_BUTTON_UP) {
			wp->dirTrg |= DEMO_STICK_UP;
		}
		if (keyGetButtonTrg(0) & PAD_BUTTON_DOWN) {
			wp->dirTrg |= DEMO_STICK_DOWN;
		}

		if (keyGetButtonRep(0) & PAD_BUTTON_LEFT) {
			wp->dirRep |= DEMO_STICK_LEFT;
		}
		if (keyGetButtonRep(0) & PAD_BUTTON_RIGHT) {
			wp->dirRep |= DEMO_STICK_RIGHT;
		}
		if (keyGetButtonRep(0) & PAD_BUTTON_UP) {
			wp->dirRep |= DEMO_STICK_UP;
		}
		if (keyGetButtonRep(0) & PAD_BUTTON_DOWN) {
			wp->dirRep |= DEMO_STICK_DOWN;
		}
	}
}

void winDisp(CameraId cameraId, void* param) {

}

void winDispKoopa(CameraId cameraId, void* param) {

}

USERFUNC_DEF(itemUseFunc) {
	PartyEntry* party;
	MarioWork* mario;
	ItemData* item;

	item = &itemDataTable[evt->lwData[10]];
	if (isFirstCall) {
		cnt = 0;
	}
	/*switch (cnt) {
		
	}*/
}





