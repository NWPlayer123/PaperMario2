#include "drv/mobjdrv.h"
#include "drv/animdrv.h"
#include "mario/mariost.h"
#include "memory.h"
#include <string.h>

extern GlobalWork* gp;

//.sbss
BOOL koopaRunFlag;

//.bss
static MapObjectWork work[3];

#define mobjGetWork2(flag) (flag ? &work[1] : &work[0])

void mobjKoopaOn(void) {
	if (koopaRunFlag == FALSE) {
		koopaRunFlag = TRUE;
		work[2].count = 128;
		work[2].entries = _mapAlloc(sizeof(MapObjectEntry) * work[2].count);
		memset(work[2].entries, 0, sizeof(MapObjectEntry) * work[2].count);
	}
}

inline MapObjectWork* mobjGetWork(void) {
	if (koopaRunFlag)
		return &work[2];
	else
		return gp->isBattleInit ? &work[1] : &work[0];
}

inline void calcMtx(MapObjectEntry* entry, Mtx dest, Vec position) { // guessing, always inlined
	Mtx trans, scale, xrot, yrot, zrot;

	MTXTrans(trans, position.x, position.y, position.z);
	MTXScale(scale, entry->scale2.x, entry->scale2.y, entry->scale2.z);
	MTXRotRad(xrot, 'x', MTXDegToRad(entry->rotation.x));
	MTXRotRad(yrot, 'y', MTXDegToRad(entry->rotation.y));
	MTXRotRad(zrot, 'z', MTXDegToRad(entry->rotation.z));
	MTXConcat(trans, zrot, trans);
	MTXConcat(trans, yrot, trans);
	MTXConcat(trans, xrot, trans);
	MTXConcat(trans, scale, dest);
}

//DispCallback mobjDispXLU
//DispCallback mObjDisp
//DispCallback mobjDisp_OffscreenXLU

void mobjInit(void) {
	work[0].count = 16;
	work[0].entries = __memAlloc(HEAP_DEFAULT, sizeof(MapObjectEntry) * work[0].count);
	memset(work[0].entries, 0, sizeof(MapObjectEntry) * work[0].count);

	work[1].count = 8;
	work[1].entries = __memAlloc(HEAP_DEFAULT, sizeof(MapObjectEntry) * work[1].count);
	memset(work[1].entries, 0, sizeof(MapObjectEntry) * work[1].count);

	koopaRunFlag = FALSE;
}

void mobjReset(BOOL inBattle) {
	MapObjectWork* wp = mobjGetWork2(inBattle);
	memset(wp->entries, 0, sizeof(MapObjectEntry) * wp->count);
	koopaRunFlag = FALSE;
}

void mobjHitEntry(MapObjectEntry* entry, s32 type) {
	AnimPoseData* pose;
	Vec bboxMin, bboxMax;

	pose = animPoseGetAnimBaseDataPtr(entry->poseId);
	bboxMin = pose->bboxMin;
	bboxMax = pose->bboxMax;




}






void mobjMain(void) {

}