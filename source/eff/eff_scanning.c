#include "eff/eff_scanning.h"
#include "drv/animdrv.h"
#include "drv/dispdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <dolphin/mtx.h>

extern GlobalWork* gp;

typedef struct effScanningData {
	s32 field_0x0; //0x0
	Vec position; //0x4
	f32 field_0x10; //0x10
	s32 animId; //0x14
	s32 field_0x18; //0x18
	s32 field_0x1C; //0x1C
	s32 field_0x20; //0x20
} effScanningData;

void effScanningMain(EffEntry* effect);
void effScanningDisp(CameraId cameraId, void* param);

EffEntry* effScanningEntry(s32 int2, f32 float3, f32 float4, f32 float5) {
	EffEntry* entry;
	effScanningData* data;
	
	
	entry = effEntry();
	entry->field_0x14 = "Scanning";
	entry->effCount = 1;
	data = (effScanningData*)__memAlloc(HEAP_EFFECT, sizeof(effScanningData) * entry->effCount);
	entry->userdata = data;
	entry->callback = effScanningMain;
	entry->flags |= 2;

	data->field_0x0 = int2;
	data->position.x = float3;
	data->position.y = float4;
	data->position.z = float5;
	data->field_0x10 = 1.0f;
	data->animId = -1;
	data->field_0x18 = 0;
	data->field_0x1C = 0;
	data->field_0x20 = 255;

	return entry;
}

void effScanningMain(EffEntry* effect) {
	BOOL inBattle;
	Vec disp = { 0 }; //loads Vec from .rodata before being overwritten
	effScanningData* data = effect->userdata;
	disp.x = data->position.x;
	disp.y = data->position.y;
	disp.z = data->position.z;

	inBattle = gp->isBattleInit != 0;

	if (effect->flags & 4) { //soft delete
		effect->flags &= ~4;
		if (data->animId != -1) {
			animPoseRelease(data->animId);
		}
		effDelete(effect);
	}
	else {
		switch (data->field_0x18) {
		case 0:
			/*if (!animGroupBaseAsync("EFF_Scan", inBattle, 0)) {
				return;
			}*/
			data->animId = animPoseEntry("EFF_Scan", (u32)inBattle);
			animPoseSetAnim(data->animId, "Z_1", 1);
			data->field_0x18++;
			break;
		case 1:
			if (animPoseGetLoopTimes(data->animId) > 1.0f) {
				animPoseRelease(data->animId);
				effDelete(effect);
				return;
			}
			break;
		}
		dispEntry(kCam3d, 2, effScanningDisp, effect, dispCalcZ(disp));
	}
}

void effScanningDisp(CameraId cameraId, void* param) {

}