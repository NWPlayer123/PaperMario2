#include "countdown.h"
#include "mariost.h"
#include <string.h>

extern marioStruct* gp;

//.sbss
static CountdownWork* wp;

void countDownInit(void) {
	wp = &gp->countdown;
	memset(&gp->countdown, 0, sizeof(CountdownWork));
}

void countDownSaveReStart(void) {

}

void countDownMain(void) {

}