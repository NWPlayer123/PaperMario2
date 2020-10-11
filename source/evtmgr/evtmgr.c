#include "evtmgr/evtmgr.h"

//.sbss
BOOL runMainF;

extern marioSt_gp* gp;

evtmgr_entry work[2];

evtmgr_entry* evtGetWork(void) {
	if (gp->field_0x14 == 0)
		return &work[0];
	return &work[1];
}

void evtmgrMain(void) {
	evtmgr_entry* entry = evtGetWork(); //inline
	runMainF = TRUE;
}