#include "seq/seq_game.h"
#include "drv/fadedrv.h"
#include "mariost.h"

extern GlobalWork* gp;

static OSTime none_key;

void seq_gameInit(seqdrv_work* work) {
	none_key = OSGetTime();
	gp->mSystemLevelFlags &= ~0x10;
}

void seq_gameMain(seqdrv_work* work) {
	switch (work->field_0x4) {
		case 0:
			if (gp->mFlags & 0x10) {
				fadeEntry(18, 900, (GXColor){0, 0, 0, 0xFF});
			}
	}
}