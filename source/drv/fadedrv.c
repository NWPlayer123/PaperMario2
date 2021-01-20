#include "drv/fadedrv.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include <dolphin/dvd.h>
#include <string.h>

static fadedrv_work* wp;

void _callback(s32 error, DVDFileInfo* info);

void fadeReset(u32 id) { //TODO: better param name?
	fadedrv_somestruct* entry;

	entry = &wp->field_0x8[id];
	if (entry->field_0x6C >= 0) {
		//animPoseRelease(entry->field_0x6C);
	}
	if (entry->field_0x70 >= 0) {
		//animPaperPoseRelease(entry->field_0x70);
	}
	if (entry->field_0x8C >= 0) {
		//imgRelease(entry->field_0x8C, entry->field_0x68);
	}
	memset(entry, 0, sizeof(fadedrv_somestruct));
	entry->field_0x6C = -1;
	entry->field_0x70 = -1;
	entry->field_0x1C = 0.0f;
	entry->field_0x20 = 0.0f;
	entry->field_0x78 = 0.0f;
	entry->field_0x7C = 0.0f;
	entry->field_0x90 = 1.0f;
}

//TODO: probably lots of case statements
void fadeEntry(s32 type, s32 duration, GXColor color) {
	if (!type) return; //TODO: invert for entire function?
}


BOOL fadeIsFinish(void) {
	return TRUE;
}

void fadeInit(void) {
	int i;

	wp = __memAlloc(0, sizeof(fadedrv_work));
	memset(wp, 0, sizeof(fadedrv_work));

	for (i = 0; i < 5; i++) {
		wp->field_0x8[i].field_0x6C = -1;
		wp->field_0x8[i].field_0x70 = -1;

		wp->field_0x8[i].field_0x1C = 0.0f;
		wp->field_0x8[i].field_0x20 = 0.0f;
		wp->field_0x8[i].field_0x78 = 0.0f;
		wp->field_0x8[i].field_0x7C = 0.0f;
		wp->field_0x8[i].field_0x90 = 1.0f;

		wp->field_0x8[i].field_0xA4 = 0;
	}

	wp->field_0x350 = -1;
	wp->tpl_header = NULL;
	wp->tpl_loaded = FALSE;
}

void _callback(s32 error, DVDFileInfo* info) {
	UnpackTexPalette(wp->tpl_header);
	DVDMgrClose(info->cb.userData);
	wp->tpl_loaded = TRUE;
}