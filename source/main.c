#include "main.h"
#include "system.h"
#include "texPalette.h"

extern marioStruct* gp;

void main(void) {
	OSTick base_tick;

	marioStInit();
	marioStMain();
	while (1) {
		base_tick = OSGetTick();
		//DEMOBeforeRender();
		marioStDisp();
		marioStMain();
		gp->deltaGame = OSGetTick() - base_tick;
		//DEMODoneRender();
		gp->deltaRender = OSGetTick() - base_tick;
	}
}