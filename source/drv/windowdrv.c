/* TODO: macro "7" for numWindows or smth */
#include "drv/windowdrv.h"
#include "drv/arcdrv.h"
#include "drv/camdrv.h"
#include "memory.h"
#include "system.h"
#include "texPalette.h"
#include <string.h>

extern int sprintf(char* str, const char* format, ...);

//.sbss
static TPLHeader* tpldata; //new in retail
static BOOL tpl_loaded; //new in retail
f32 flt_80418800; //TODO: rename
GXTexObj* wakuTexObj;
WindowEntry* WinObjects;

//local prototypes
static void _callback(s32 error, DVDFileInfo* info);

void windowInit(void) {
	WinObjects = __memAlloc(HEAP_DEFAULT, sizeof(WindowEntry) * 7);
	WinObjects[0].flags = 0;
	WinObjects[0].field_0x28 = 0;
	WinObjects[1].flags = 0;
	WinObjects[1].field_0x28 = 0;
	WinObjects[2].flags = 0;
	WinObjects[2].field_0x28 = 0;
	WinObjects[3].flags = 0;
	WinObjects[3].field_0x28 = 0;
	WinObjects[4].flags = 0;
	WinObjects[4].field_0x28 = 0;
	WinObjects[5].flags = 0;
	WinObjects[5].field_0x28 = 0;
	WinObjects[6].flags = 0;
	WinObjects[6].field_0x28 = 0;
	tpldata = NULL;
	tpl_loaded = FALSE;
}

//new in retail
static void _callback(s32 error, DVDFileInfo* info) {
	u32 i;

	UnpackTexPalette(tpldata);
	DVDMgrClose(info->cb.userData);
	wakuTexObj = __memAlloc(HEAP_DEFAULT, sizeof(GXTexObj) * tpldata->imageCount);

	for (i = 0; i < tpldata->imageCount; i++) {
		TEXGetGXTexObjFromPalette(tpldata, &wakuTexObj[i], i);
	}
	tpl_loaded = TRUE;
}

void windowTexSetup(void) {
	DVDEntry* entry;
	u32 size;
	char path[128];
	u32 i;

	tpldata = NULL;
	tpl_loaded = FALSE;
	tpldata = arcOpen("font/msgWindow.tpl", NULL, NULL);
	if (tpldata) {
		UnpackTexPalette(tpldata);
		wakuTexObj = __memAlloc(HEAP_DEFAULT, sizeof(GXTexObj) * tpldata->imageCount);

		for (i = 0; i < tpldata->imageCount; i++) {
			TEXGetGXTexObjFromPalette(tpldata, &wakuTexObj[i], i);
		}
	}
	else {
		sprintf(path, "%s/f/msgWindow.tpl", getMarioStDvdRoot());
		entry = DVDMgrOpen(path, 2, 0);
		size = OSRoundUp32B(DVDMgrGetLength(entry));
		tpldata = __memAlloc(HEAP_DEFAULT, size);
		entry->info.cb.userData = entry;
		DVDMgrReadAsync(entry, tpldata, size, 0, _callback);
	}
}

void windowReInit(void) {
	WinObjects[0].flags = 0;
	WinObjects[0].field_0x28 = 0;
	WinObjects[1].flags = 0;
	WinObjects[1].field_0x28 = 0;
	WinObjects[2].flags = 0;
	WinObjects[2].field_0x28 = 0;
	WinObjects[3].flags = 0;
	WinObjects[3].field_0x28 = 0;
	WinObjects[4].flags = 0;
	WinObjects[4].field_0x28 = 0;
	WinObjects[5].flags = 0;
	WinObjects[5].field_0x28 = 0;
	WinObjects[6].flags = 0;
	WinObjects[6].field_0x28 = 0;
}

s32 windowEntry(u16 a1) {
	WindowEntry* entry;
	int i;

	for (i = 0; i < 7; i++) {
		entry = &WinObjects[i];
		if (!(entry->flags & 1)) {
			memset(entry, 0, sizeof(WindowEntry));
			entry->flags = 1;
			entry->field_0x4 = a1;
			return i;
		}
	}
	return -1;
}

BOOL windowDelete(WindowEntry* entry) {
	if (entry->delete) {
		entry->delete(entry);
	}
	entry->flags = 0;
	entry->field_0x28 = 0;
	return TRUE;
}

BOOL windowDeleteID(s32 id) {
	WindowEntry* entry = &WinObjects[id];
	if (!(entry->flags & 1)) {
		return FALSE;
	}
	if (entry->delete) {
		entry->delete(entry);
	}
	entry->flags = 0;
	entry->field_0x28 = 0;
	return TRUE;
}

void windowMain(void) {
	WindowEntry* entry;
	int i;

	for (i = 0; i < 7; i++) {
		entry = &WinObjects[i];
		if (entry->flags & 1) {
			if (entry->main) {
				entry->main(entry);
			}
		}
	}
}

void windowDispGX_Kanban(s32 type, u8 alpha, f32 x, f32 y, f32 height, f32 width) {
	cameraObj* camera;

	camera = camGetCurPtr();
	GXSetCullMode(GX_CULL_NONE);
	GXSetZCompLoc(GX_TRUE);
	GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_OR);
	GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
	GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, (GXColor){0xFF, 0xFF, 0xFF, 0xFF});
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
	GXSetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);




}

//more disp functions

s32 windowCheckID(s32 id) {
	return WinObjects[id].flags & 2;
}

WindowEntry* windowGetPointer(s32 id) {
	return &WinObjects[id];
}

GXTexObj* getWakuTexObj(s32 id) {
	return &wakuTexObj[id];
}