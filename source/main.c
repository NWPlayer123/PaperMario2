#include "seq_title.h"
#include "mariost.h"
#include <dolphin/os.h>
extern seq_title_work* wp2;
extern marioSt_gp* gp;

//void teststuff(s32 value);

void main(void) {
	OSTick base_tick;

	marioStInit();
	marioStMain();
	while (1) {
		base_tick = OSGetTick();
		DEMOBeforeRender();
		marioStDisp();
		marioStMain();
		gp->deltaGame = OSGetTick() - base_tick;
		DEMODoneRender();
		gp->deltaRender = OSGetTick() - base_tick;
	}
}


/*void main(void) {
	//wp2->debugMode = -1;
	marioStInit();
	asm{
		ba 0
	}
	fileInit();
	fileAllocf(4, "%s/monthry.tpl", ".");
	while (1) {

	}
}

void teststuff(s32 value) {
	s32 test;
	//test = value / 32;
	value %= 32;
}*/