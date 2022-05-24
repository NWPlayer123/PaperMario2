#include "aaa_00.h"
#include "drv/dispdrv.h"
#include "evt/evt_bero.h"
#include "evt/evt_npc.h"
#include "mgr/filemgr.h"
#include "win/win_main.h"
#include "system.h"
#include <string.h>

typedef struct MarioHouseWork {
	FileEntry* texture; //0x0
	s32 alpha; //0x4
} MarioHouseWork;

NpcSetupInfo npcEnt[] = {
	{"パレッタ", 0x40000600, 0, 0, 0, 0, 0, 0, 0, 0, 0, NPC_TERRITORY_TYPE_NOTHING, {0}, {0}, 0.0f, 0.0f, 0.0f, 0.0f, 0}, //Parakarry
	{"ルイージ", 0x40000600, 0, 0, 0, 0, 0, 0, 0, 0, 0, NPC_TERRITORY_TYPE_NOTHING, {0}, {0}, 0.0f, 0.0f, 0.0f, 0.0f, 0}, //Luigi
	{0}
};

BeroEntry bero_entry_data = {
	"dokan_1",
	2, 0, 9,
	0x186A0, 0, 0,
	-1, NULL, 6, NULL,
	0, "dokan_1",
	1, 1, 0, 0
};

EVT_BEGIN(aaa_00_init_evt)
SET(LW(0), PTR(&bero_entry_data))
USER_FUNC(evt_bero_get_info)
RUN_SCRIPT(evt_bero_info_run)
USER_FUNC(evt_npc_setup, PTR(npcEnt))
EVT_END()

//.bss
MarioHouseWork* wp;


static void draw(CameraId cameraId, void* param) {
	MarioHouseWork* work = param;

	//stack data
	CameraEntry camera;
	f32 viewport[GX_VIEWPORT_SZ];
	f32 projection[GX_PROJECTION_SZ];
	Vec translate, scale;
	GXColor color;

	GXGetViewportv(viewport);
	GXGetProjectionv(projection);
	GXSetProjection(camGetPtr(CAMERA_2D)->projection, camGetPtr(CAMERA_2D)->type);
	camera = *camGetCurPtr();
	*camGetCurPtr() = *camGetPtr(CAMERA_2D);
	winTexInit(*wp->texture->data);
	color = (GXColor){0xFF, 0xFF, 0xFF, (u8)wp->alpha};
	scale = (Vec){1.0f, 1.0f, 1.0f};
	translate = (Vec){0.0f, 0.0f, 0.0f};
	winTexSet(3, translate, scale, color);
	*camGetCurPtr() = camera;
	GXSetViewportv(viewport);
	GXSetProjectionv(projection);
}

USERFUNC_DEF(mapdraw) {
	if (isFirstCall) {
		wp = _mapAlloc(8);
		memset(wp, 0, sizeof(MarioHouseWork));
		wp->texture = fileAllocf(4, "%s/mariost.tpl", getMarioStDvdRoot());
		wp->alpha = 0;
	}
	wp->alpha += 2;
	if (wp->alpha > 255) {
		wp->alpha = 255;
		evtSetValue(evt, GF(0), 1); //done rolling in
	}
	dispEntry(CAMERA_3D, 7, draw, wp, 1000.0f);
	return EVT_RETURN_BLOCK;
}

void mapdelete(void) {
	if (wp && wp->texture) {
		fileFree(wp->texture);
	}
}
