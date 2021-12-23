#include "drv/dispdrv.h"
#include "drv/camdrv.h"
#include "memory.h"
#include "system.h"
#include <string.h>

// Misc TODO: #define DISP_MAX_ENTRIES 0x400 ?

//.sdata
//TODO: figure out exact size minus alignment
f32 offset_tbl[18] = {
	400000.0f, 400000.0f, 600000.0f,
	100000.0f, 100000.0f, 300000.0f,
	900000.0f, 1100000.0f, 1000000.0f,
	500000.0f, 700000.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};

u8 i4tex = 0xFF; //TODO: align 32-byte

//.sbss
DispEntry* pDispWork; //TODO: re-type these
DispEntry** pSortWork;
u32 entry_n;
DispEntry* currentWorkPtr;

//local prototypes
static s32 _sort(const void* a, const void* b);

f32 dispCalcZ(Vec input) {
	CameraEntry* camera;
	Vec output;
	f32 calc;

	camera = camGetPtr(CAMERA_3D);
	MTXMultVec(camera->view, &input, &output);
	MTX44MultVec(camera->projection, &output, &output);
	calc = (5000.0f * output.z) + 5000.0f;
	if (calc < 0.0f) {
		calc = 0.0f;
	}
	else {
		if (calc > 10000.0f) {
			calc = 10000.0f;
		}
	}
	return calc * -1.0f;
}

void dispDraw(CameraId cameraId) {
	DispEntry* entry;
	int i;

	for (i = 0; i < entry_n; i++) {
		entry = pSortWork[i];
		if (entry->cameraId == cameraId) {
			GXSetColorUpdate(GX_TRUE);
			switch (cameraId) {
			case CAMERA_3D:
			case CAMERA_3D_EFFECTA:
			case CAMERA_3D_EFFECTB:
				GXSetAlphaUpdate(GX_FALSE);
				break;
			default:
				GXSetAlphaUpdate(GX_TRUE);
				break;
			}
			GXSetZScaleOffset(1.0f, 0.0f);
			switch (entry->renderMode) {
			case 0:
				GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_OR);
				GXSetZCompLoc(GX_TRUE);
				GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
				GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
				break;
			case 1:
				GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
				GXSetZCompLoc(GX_FALSE);
				GXSetAlphaCompare(GX_GEQUAL, 0x80u, GX_AOP_OR, GX_NEVER, 0);
				GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
				break;
			case 2:
			case 5:
			case 9:
			case 10:
				GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
				GXSetZCompLoc(GX_TRUE);
				GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
				GXSetZMode(GX_TRUE, GX_LEQUAL, GX_FALSE);
				break;
			case 3:
				GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
				GXSetZCompLoc(GX_TRUE);
				GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
				GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
				break;
			case 4:
				GXSetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_CLEAR);
				GXSetZCompLoc(GX_FALSE);
				GXSetAlphaCompare(GX_GEQUAL, 0x80u, GX_AOP_OR, GX_NEVER, 0);
				GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
				break;
			default:
				break;
			}
			currentWorkPtr = entry;
			entry->callback(cameraId, entry->param);
		}
	}
}

void dispSort(void) {
	qqsort(pSortWork, entry_n, 4, _sort);
}

//TODO: cleanup?
s32 _sort(const void* a, const void* b) {
	f32 value1, value2;

	value1 = *(f32*)(*(u32*)a + 4);
	value2 = *(f32*)(*(u32*)b + 4);
	if (value1 > value2) {
		return 1;
	}
	if (value1 >= value2) {
		return 0;
	}
	return -1;
}

void dispEntry(CameraId cameraId, u8 renderMode, DispCallback callback, void* param, f32 order) {
	DispEntry* entry;

	entry = &pDispWork[entry_n];
	entry->cameraId = cameraId;
	entry->renderMode = renderMode;
	entry->callback = callback;
	entry->param = param;

	switch (renderMode) {
	case 2:
	case 5:
	case 9:
	case 10:
		entry->order = offset_tbl[renderMode] + order;
		break;
	default:
		entry->order = offset_tbl[renderMode] - order;
		break;
	}
	pSortWork[entry_n++] = entry;
}

void dispReInit(void) {
	entry_n = 0;
}

void dispInit(void) {
	pDispWork = __memAlloc(HEAP_DEFAULT, sizeof(DispEntry) * 0x400);
	pSortWork = __memAlloc(HEAP_DEFAULT, sizeof(DispEntry*) * 0x400);
	memset(pDispWork, 0, sizeof(DispEntry) * 0x400);
	memset(pSortWork, 0, sizeof(DispEntry*) * 0x400);
	entry_n = 0;
}