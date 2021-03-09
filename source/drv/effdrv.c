#include "drv/effdrv.h"
#include "drv/arcdrv.h"
#include "eff/eff_n64.h"
#include "mgr/dvdmgr.h"
#include "mariost.h"
#include "memory.h"
#include "system.h"
#include <string.h>

extern int sprintf(char* str, const char* format, ...);

extern marioStruct* gp;

//.sbss
static effdrv_work work;

//.sdata
static effdrv_work* wp = &work;
char* prefix_tbl[2] = {"jp", "us"};

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
	wp->handle = NULL;
	wp->language = -1;
	wp->effTexture = NULL;
	wp->effTextureLoaded = FALSE;
	effInit64();
}

void effTexSetup(void) {
	DVDEntry* entry;
	char path[128];
	u32 length;

	wp->effTexture = NULL;
	wp->effTextureLoaded = FALSE;
	wp->effTexture = (TPLHeader*)arcOpen("effect.tpl", 0, 0);
	if (wp->effTexture) {
		UnpackTexPalette(wp->effTexture);
		wp->effTextureLoaded = TRUE;
	}
	else {
		sprintf(path, "%s/e/jp/effect.tpl", getMarioStDvdRoot());
		entry = DVDMgrOpen(path, 2, 0);
		length = OSRoundUp32B(DVDMgrGetLength(entry));
		wp->effTexture = (TPLHeader*)__memAlloc(HEAP_DEFAULT, length);
		entry->info.cb.userData = entry;
		DVDMgrReadAsync(entry, wp->effTexture, (s32)length, 0, _callback_tpl);
	}
	effTexSetupN64();
}

void effGetTexObj(u32 id, GXTexObj* obj) {
#ifdef __MWERKS__
#pragma explicit_zero_data on
	if (id < 146) { //number of images in effect.tpl
		if (!wp->effTextureLoaded) {
			static char dummy ATTRIBUTE_ALIGN(8) = '\0';
			GXInitTexObj(obj, &dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
		}
		else {
			TEXGetGXTexObjFromPalette(wp->effTexture, obj, id);
		}
	}
	else {
		static char dummy ATTRIBUTE_ALIGN(8) = '\0';
		if (!wp->handle) {
			GXInitTexObj(obj, &dummy, 1, 1, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE);
		}
		else {
			TEXGetGXTexObjFromPalette(*(TPLHeader**)wp->handle->mppFileData, obj, id - 146);
		}
	}
#pragma explicit_zero_data reset
#endif
}

void effAutoRelease(BOOL inBattle) {
	EffEntry* entry;
	int i;

	for (i = 0; i < wp->numEntries; i++) {
		entry = &wp->entries[i];
		if ((entry->flags & 1) && (entry->inBattle == inBattle) && (entry) && (entry->flags)) {
			__memFree(HEAP_EFFECT, entry->field_0xC);
			entry->flags = 0;
		}
	}
}

EffEntry* effEntry(void) {
	EffEntry* entry;
	int i;

	entry = &wp->entries[0];
	for (i = 0; i < wp->numEntries; i++) {
		if (entry->flags == 0) {
			break; //one we can use
		}
		entry++;
	}
	entry->flags = 1;
	entry->inBattle = gp->isBattleInit != 0;
	entry->field_0x14 = 0;
	entry->effCount = 0;
	entry->field_0xC = 0;
	entry->field_0x10 = 0;
	entry->name[0] = '\0';
	return entry;
}

void effSetName(EffEntry* effect, const char* name) {
	EffEntry* entry;
	int i;

	entry = wp->entries;
	for (i = 0; i < wp->numEntries; i++) {
		if (entry->flags == 0) {
			if (!strcmp(entry->name, name)) {
				break;
			}
		}
		entry++;
	}
	strcpy(effect->name, name);
}

void effMain(void) {
	char *prefix, *dvdroot;
	EffEntry* entry;
	int i;

	for (i = 0; i < wp->numEntries; i++) {
		entry = &wp->entries[i];
		if (gp->isBattleInit) {
			if (!entry->inBattle) {
				continue;
			}
		}
		else if (entry->inBattle) {
			continue;
		}
		if (entry->flags & 1) {
			if (entry->field_0x10) {
				entry->field_0x10(entry);
			}
		}
	}
	if (wp->language != gp->mLanguage) {
		if (wp->handle) {
			fileFree(wp->handle);
			wp->handle = NULL;
		}
		prefix = prefix_tbl[gp->mLanguage];
		dvdroot = getMarioStDvdRoot();
		if (fileAsyncf(4, 0, "%s/e/%s/effect_%s.tpl", dvdroot, prefix, prefix)) {
			wp->handle = fileAllocf(4, "%s/e/%s/effect_%s.tpl", dvdroot, prefix, prefix);
			wp->language = gp->mLanguage;
		}
 	}
}

void effDelete(EffEntry* effect) {
	if (effect && effect->flags) {
		__memFree(HEAP_EFFECT, effect->field_0xC);
		effect->flags = 0;
	}
}

void effSoftDelete(EffEntry* effect) {
	if (effect && effect->flags) {
		if (effect->flags & 2) {
			effect->flags |= 4; // "deleted"
		}
		else {
			effDelete(effect);
		}
	}
}












































