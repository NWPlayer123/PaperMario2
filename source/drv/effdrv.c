#include "drv/effdrv.h"
#include "eff/eff_n64.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include <string.h>

//.sbss
static effdrv_work work;

//.sdata
static effdrv_work* wp = &work;

//local prototypes
static void _callback_tpl(s32 error, DVDFileInfo* info);

static void _callback_tpl(s32 error, DVDFileInfo* info) {
	DVDEntry* entry;

	entry = (DVDEntry*)info->cb.userData;
	UnpackTexPalette(wp->effTexture);
	DVDMgrClose(entry);
	wp->effTextureLoaded = TRUE;
}

void effInit(void) {
	wp->numEntries = 0x100;
	wp->entries = (EffEntry*)__memAlloc(HEAP_DEFAULT, wp->numEntries * sizeof(EffEntry));
	memset(wp->entries, 0, wp->numEntries * sizeof(EffEntry));
	wp->field_0x10 = NULL;
	wp->field_0x14 = -1;
	wp->effTexture = NULL;
	wp->effTextureLoaded = FALSE;
	effInit64();
}