#include "drv/extdrv.h"
#include "drv/dispdrv.h"
#include "mario/mariost.h"

extern GlobalWork* gp;

static ExtWork work[2];

#define extGetWork() (gp->isBattleInit ? &work[1] : &work[0])

void extInit(void) {
	ExtWork* wp = extGetWork();
	wp->poseNum = 0;
	wp->field_0x8 = 0;
}

void extMain(void) {

}





void extMakeTexture(CameraId cameraId, void* param) {

}