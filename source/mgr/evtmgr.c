#include "mgr/evtmgr.h"

//.bss
s32 priIDTbl[0x100];
s32 priTbl[0x100];
evtWork work[2];

//.sbss
BOOL runMainF;
u32 priTblNum; //TODO: type
u32 evtMax; //TODO: sort these once they're all added

extern marioStruct* gp;

evtWork* evtGetWork(void) {
	return &work[gp->isBattleInit];
}