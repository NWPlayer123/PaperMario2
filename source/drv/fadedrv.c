#include "drv/fadedrv.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include <dolphin/dvd.h>
#include <string.h>

static FadeWork* wp;

//local prototypes
void _callback(s32 error, DVDFileInfo* info);

void _callback(s32 error, DVDFileInfo* info) {
	UnpackTexPalette(wp->tpl_header);
	DVDMgrClose(info->cb.userData);
	wp->tpl_loaded = TRUE;
}

void fadeInit(void) {
	int i;

	wp = __memAlloc(HEAP_DEFAULT, sizeof(FadeWork));
	memset(wp, 0, sizeof(FadeWork));

	for (i = 0; i < 5; i++) {
		wp->entries[i].field_0x6C = -1;
		wp->entries[i].field_0x70 = -1;

		wp->entries[i].field_0x1C = 0.0f;
		wp->entries[i].field_0x20 = 0.0f;

		wp->entries[i].field_0x78 = 0.0f;
		wp->entries[i].field_0x7C = 0.0f;

		wp->entries[i].field_0x90 = 1.0f;

		wp->entries[i].field_0xA4 = 0;
	}

	wp->field_0x350 = -1;
	wp->tpl_header = NULL;
	wp->tpl_loaded = FALSE;
}









void fadeReset(u32 id) { //TODO: better param name?
	FadeEntry* entry;

	entry = &wp->entries[id];
	if (entry->field_0x6C >= 0) {
		//animPoseRelease(entry->field_0x6C);
	}
	if (entry->field_0x70 >= 0) {
		//animPaperPoseRelease(entry->field_0x70);
	}
	if (entry->field_0x8C >= 0) {
		//imgRelease(entry->field_0x8C, entry->field_0x68);
	}
	memset(entry, 0, sizeof(FadeEntry));
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

void fadeMain(void) {

}