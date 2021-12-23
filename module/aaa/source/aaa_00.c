#include "aaa_00.h"
#include "drv/dispdrv.h"
#include "evt/evt_cmd.h"

typedef struct MarioHouseWork {
	u8 field_0x0[0x8 - 0x0]; //0x0
} MarioHouseWork;

EVT_BEGIN(aaa_00_init_evt)
SET(LW(0), STRING("dokan_1"))



EVT_END()

static void draw(CameraId cameraId, void* param) {
	MarioHouseWork* work = param;

	//stack data
	//some big struct
	f32 viewport[GX_VIEWPORT_SZ];
	f32 projection[GX_PROJECTION_SZ];

	GXGetViewportv(viewport);
	GXGetProjectionv(projection);


	GXSetViewportv(viewport);
	GXSetProjectionv(projection);
}

void mapdelete(void) {

}