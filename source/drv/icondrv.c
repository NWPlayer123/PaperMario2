#include "drv/icondrv.h"
#include "mgr/dvdmgr.h"
#include "mariost.h"
#include "memory.h"
#include "system.h"
#include "texPalette.h"
#include <string.h>
#include <stdio.h>

extern marioStruct* gp;

//.sbss
TPLHeader* icon_tpl;
void* icon_bin; //TODO: type
BOOL icon_tpl_ok;
BOOL icon_bin_ok;
static IconWork work[2];

//local prototypes
void _callback_tpl(s32 error, DVDFileInfo* info);
void _callback_bin(s32 error, DVDFileInfo* info);

//TODO: inline function? I see nothing in the symbol map
#define iconGetWork() (gp->isBattleInit ? &work[1] : &work[0])

void _callback_tpl(s32 error, DVDFileInfo* info) {
	UnpackTexPalette(icon_tpl);
	DVDMgrClose(info->cb.userData);
	icon_tpl_ok = TRUE;
}

void _callback_bin(s32 error, DVDFileInfo* info) {
	DVDMgrClose(info->cb.userData);
	icon_bin_ok = TRUE;
}

void iconInit(void) {
	char path[128];
	DVDEntry* entry;
	u32 size;

	work[0].count = 32;
	work[0].entries = (IconEntry*)__memAlloc(HEAP_DEFAULT, sizeof(IconEntry) * work[0].count);
	memset(work[0].entries, 0, sizeof(IconEntry) * work[0].count);
	
	work[1].count = 32;
	work[1].entries = (IconEntry*)__memAlloc(HEAP_DEFAULT, sizeof(IconEntry) * work[1].count);
	memset(work[1].entries, 0, sizeof(IconEntry) * work[1].count);
	icon_tpl = NULL;
	icon_bin = NULL;
	icon_tpl_ok = FALSE;
	icon_bin_ok = FALSE;

	sprintf(path, "%s/icon.tpl", getMarioStDvdRoot());
	entry = DVDMgrOpen(path, 2, 0);
	if (entry) {
		size = OSRoundUp32B(DVDMgrGetLength(entry));
		icon_tpl = (TPLHeader*)__memAlloc(HEAP_DEFAULT, size);
		entry->info.cb.userData = entry; //this file's userData is a pointer to the entry
		DVDMgrReadAsync(entry, icon_tpl, size, 0, _callback_tpl);
	}

	sprintf(path, "%s/icon.bin", getMarioStDvdRoot());
	entry = DVDMgrOpen(path, 2, 0);
	if (entry) {
		size = OSRoundUp32B(DVDMgrGetLength(entry));
		icon_bin = (TPLHeader*)__memAlloc(HEAP_DEFAULT, size);
		entry->info.cb.userData = entry; //this file's userData is a pointer to the entry
		DVDMgrReadAsync(entry, icon_bin, size, 0, _callback_bin);
	}
}

void iconTexSetup(void) {
	; //stubbed in retail
}

void iconReInit(void) {
	IconWork* iconwork = iconGetWork();
	memset(work->entries, 0, sizeof(IconEntry) * iconwork->count);
}

void iconMain(void) {
	IconEntry* entry;
	int i;

	IconWork* iconwork = iconGetWork();
	if (!icon_tpl_ok) return;
	if (!icon_bin_ok) return;
	for (i = 0; i < iconwork->count; i++) {
		entry = &iconwork->entries[i];
		if (entry->flags & 1) {
			if (entry->flags & 0x2000) {

			}
			if (entry->flags & 0x1000) {
				if (entry->field_0x34) {
					entry->field_0x34--;
				}
				if (!entry->field_0x34) { //we've run out of ???
					entry->field_0x30++;
					if (entry->field_0x30 >= entry->field_0x2E) {
						entry->field_0x30 = 0;
					}
					if (icon_tpl_ok && icon_bin_ok) {

					}
				}
			}
		}
	}
}






void iconSetPos(const char* name, f32 x, f32 y, f32 z) {
	IconWork* workptr = iconGetWork();
	s32 i;
	IconEntry* entry;
	
	entry = workptr->entries;
	for (i = 0; i < workptr->count; i++, entry++) {
		if (entry->flags & 1 && !strcmp(entry->name, name)) {
			goto fuck;
		}
	}
	entry = 0;
fuck:
	if (entry) {
		Vec temp = { 0 };
		temp.x = x;
		temp.y = y;
		temp.z = z;
		entry->position = temp;
	}
}
/*
void iconSetPos(const char* name, f32 x, f32 y, f32 z) {
	IconWork* workptr = iconGetWork();
	IconEntry* entry;
	int i;

	for (i = 0; i < workptr->numEntries; i++) {
		entry = &workptr->entries[i];
		if (entry->flags & 1 && !strcmp(entry->name, name)) {
			break;
		}
	}
	if (i == workptr->numEntries) {
		entry = NULL;
	}
	if (entry) {
		Vec temp = { 0 };
		temp.x = x;
		temp.y = y;
		temp.z = z;
		entry->position = temp;
	}
}*/