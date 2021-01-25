#include "mgr/winmgr.h"
#include "memory.h"
#include <string.h>

//.sbss
static WinMgrWork work;
//.sdata
static WinMgrWork* wp = &work;

void winMgrReInit(void) {
	memset(wp->mEntries, 0, wp->mNumEntries * sizeof(WinMgrEntry));
}

void winMgrInit(void) {
	wp->mNumEntries = 32;
	wp->mEntries = (WinMgrEntry*)__memAlloc(HEAP_DEFAULT, wp->mNumEntries * sizeof(WinMgrEntry));
	memset(wp->mEntries, 0, wp->mNumEntries * sizeof(WinMgrEntry));
}