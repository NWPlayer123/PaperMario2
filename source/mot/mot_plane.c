#include "mot/mot_plane.h"
#include "mario/mario.h"
#include "memory.h"
#include <string.h>

void mot_plane(void) {
	MarioWork* mario;
	MotionPlaneData* data;

	mario = marioGetPtr();
	if (mario->trigFlags & 1) {
		mario->trigFlags &= ~1;
		mario->motStruct = __memAlloc(HEAP_DEFAULT, sizeof(MotionPlaneData));
		memset(mario->motStruct, 0, sizeof(MotionPlaneData));
		data = marioGetPtr()->motStruct;
		data->field_0x28 = -1;
		mario->currSubMotionId = 0;
	}
	switch (mario->currSubMotionId) {
		case 0:
			//inline start?
			mario = marioGetPtr();

			//inline end?
			//marioPreJump();
			break;
	}
}

void flyMain(void) {
	MarioWork* mario;

	mario = marioGetPtr();
	switch (mario->currSubMotionId) {
		case 0xC:
			if (mario->wCamVal1 >= mario->position.y) {
				mario->currSubMotionId = 14;
			}
			//inlined?
			mario = marioGetPtr();
	}
}