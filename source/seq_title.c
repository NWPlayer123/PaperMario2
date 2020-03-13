#include "seq_title.h"

//.bss
seq_title_work work2;
//.sdata
seq_title_work* wp2 = &work2;


s32 getDebugMode(void) {
	return wp2->debugMode;
}