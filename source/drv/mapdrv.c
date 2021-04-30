#include "drv/mapdrv.h"

//.bss
MapError error_data[32];

//.sbss
MapWork* mapWork;
s32 activeGroup;
BOOL mapClipOffFlag;
MapObj* current_mp;
BOOL error_flag;
s32 error_count;
BOOL mapDispFlag; //unknown variable name

BOOL __mapdrv_make_dl;

void mapInit(void) {

}

void mapMain(void) {

}

void mapDisp(void) {

}