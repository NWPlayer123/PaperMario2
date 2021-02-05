#include "mario_cam.h"
#include "mario.h"

s32 marioGetCamId(void) {
	return marioGetPtr()->camId;
}