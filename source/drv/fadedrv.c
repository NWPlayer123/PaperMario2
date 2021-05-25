#include "drv/fadedrv.h"
#include "drv/arcdrv.h"
#include "drv/camdrv.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include "system.h"
#include <dolphin/dvd.h>
#include <string.h>

extern int sprintf(char* str, const char* format, ...);

static FadeWork* wp;

//local prototypes
void _callback(s32 error, DVDFileInfo* info);
void disp_texture(CameraId cameraId, void* param);
void z(void);
static void zFill(void);
void disp_maku(CameraId cameraId, void* param);
void disp_tec(CameraId cameraId, void* param);
void disp_dummy_black(CameraId cameraId, void* param);

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

		wp->entries[i].FadeSpotX = 0.0f;
		wp->entries[i].FadeSpotY = 0.0f;

		wp->entries[i].AnimOffsetX = 0.0f;
		wp->entries[i].AnimOffsetY = 0.0f;

		wp->entries[i].field_0x90 = 1.0f;

		wp->entries[i].field_0xA4 = 0;
	}

	wp->field_0x350 = -1;
	wp->tpl_header = NULL;
	wp->tpl_loaded = FALSE;
}

void fadeTexSetup(void) {
	DVDEntry* dvd;
	u32 size;
	char path[128];

	wp->tpl_header = NULL;
	wp->tpl_header = arcOpen("fade.tpl", NULL, NULL);
	if (wp->tpl_header) {
		UnpackTexPalette(wp->tpl_header);
		wp->tpl_loaded = TRUE;
	}
	else {
		sprintf(path, "%s/fade.tpl", getMarioStDvdRoot());
		dvd = DVDMgrOpen(path, 2, 0);
		size = OSRoundUp32B(DVDMgrGetLength(dvd));
		wp->tpl_header = __memAlloc(HEAP_DEFAULT, size);
		dvd->info.cb.userData = dvd;
		DVDMgrReadAsync(dvd, wp->tpl_header, size, 0, _callback);
	}
}

//TODO: probably lots of switch statements
void fadeEntry(s32 type, s32 duration, GXColor* color) {
	if (type) {
		if (type == 16) {
			type = 9;
			*color = (GXColor){ 0xF0, 0xF0, 0xF0, 0xFF };
		}
		if (type == 17) {
			type = 10;
			*color = (GXColor){ 0xF0, 0xF0, 0xF0, 0xFF };
		}
		if (type == 18) {
			type = 11;
			*color = (GXColor){ 0xF0, 0xF0, 0xF0, 0xFF };
		}
		if (type == 19) {
			type = 12;
			*color = (GXColor){ 0xF0, 0xF0, 0xF0, 0xFF };
		}
		if (type == 20) {
			type = 14;
			*color = (GXColor){ 0xF0, 0xF0, 0xF0, 0xFF };
		}
		if (type == 21) {
			type = 15;
			*color = (GXColor){ 0xF0, 0xF0, 0xF0, 0xFF };
		}
		if ((type == 1 || type == 2) && duration < 1000) {
			duration = 1000;
		}
		if (type == 7) {
			type = 3;
		}
		else if (type == 8) {
			type = 4;
		}
		else if ((type - 3 <= 2 || type == 6) && duration < 1000) {
			duration = 1000;
		}
	}
}

void fadeTecOn(void) {
	wp->flags |= 0x1000;
}

void fadeTecOff(void) {
	wp->flags &= ~0x1000;
}

void fadeTecSoftOn(void) {
	wp->flags |= 0x1010;
	wp->field_0x354 = 0.0f;
}

void fadeTecSoftOff(void) {
	wp->flags |= 0x1020;
	wp->field_0x354 = 1.0f;
}

void fadeSoftFocusOn(void) {
	wp->flags |= 0x2000;
}

void fadeSoftFocusOff(void) {
	wp->flags &= ~0x2000;
}

void fadeWhiteOutOn(void) {
	wp->flags |= 0x4000;
}

void fadeWhiteOutOff(void) {
	wp->flags &= ~0x4000;
}

void fadeSetSpotPos(f32 x, f32 y) {
	wp->entries[0].FadeSpotX = x;
	wp->entries[0].FadeSpotY = y;
}

void fadeSetAnimOfsPos(f32 x, f32 y) {
	wp->entries[1].AnimOffsetX = x;
	wp->entries[1].AnimOffsetY = y;
}

void fadeSetAnimVirtualPos(f32 x, f32 y, f32 z) {
	FadeEntry* entry = &wp->entries[1];
	entry->VirtualPos = (Vec){x, y, z};
	entry->flags |= 4;
}

void fadeSetOffscreenCallback(void (*callback)(void*, void*), void* param) {
	wp->entries[3].OffscreenCallback = callback;
	wp->entries[3].OffscreenParam = param;
}

void fadeMain(void) {

}

void disp_texture(CameraId cameraId, void* param) {
	//passed wp, unused
}

void z(void) {

}

static void zFill(void) {

}

void disp_maku(CameraId cameraId, void* param) {
	FadeEntry* entry = param;

}

void disp_tec(CameraId cameraId, void* param) {
	//passed wp, unused

}

void disp_dummy_black(CameraId cameraId, void* param) {
	//passed wp, unused

}

BOOL fadeIsFinish(void) {
	return TRUE;
}

void fadeReset(s32 id) { //TODO: better param name?
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
	entry->FadeSpotX = 0.0f;
	entry->FadeSpotY = 0.0f;
	entry->AnimOffsetX = 0.0f;
	entry->AnimOffsetY = 0.0f;
	entry->field_0x90 = 1.0f;
}

TPLHeader* fadeGetTpl(void) {
	return wp->tpl_header;
}

void fadeSetNarrowFast(void) {
	wp->entries[1].flags |= 8;
}
