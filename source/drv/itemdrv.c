#include "drv/itemdrv.h"

//.bss
static ItemWork work[2];

//TODO: inline function? I see nothing in the symbol map
#define itemGetWork() (gp->isBattleInit ? &work[1] : &work[0])

void itemInit(void) {

}

void itemMain(void) {

}