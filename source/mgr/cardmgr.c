#include "mgr/cardmgr.h"
#include "memory.h"
#include <string.h>

//.sbss
static CardWork* wp;

void cardInit(void) {
	wp = __memAlloc(HEAP_DEFAULT, sizeof(CardWork));
	memset(wp, 0, sizeof(CardWork));

}







BOOL cardIsExec(void) {
	return TRUE;
}


void cardMain(void) {

}