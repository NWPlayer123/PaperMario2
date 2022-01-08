#include "seq/seq_game.h"
#include "drv/fadedrv.h"
#include "mario/mariost.h"

extern GlobalWork* gp;

static OSTime none_key;

void seq_gameInit(SequenceWork* work) {
	none_key = OSGetTime();
	gp->systemLevelFlags &= ~0x10;
}

void seq_gameMain(SequenceWork* work) {
	switch (work->state) {
		case 0:
			if (gp->flags & 0x10) {
				fadeEntry(18, 900, (GXColor){0, 0, 0, 0xFF});
			}
	}
}