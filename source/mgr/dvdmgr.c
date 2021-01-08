/* "dvdmgr" - DVD File Manager
 * Status: Functions finished, need to figure out names for struct and params
 *
 * Function: easy file handling for loading data from the disc, instead of
 * having to call DVD functions directly and handle errors
 */
#include "mgr/dvdmgr.h"
#include "memory.h"
#include "system.h"
#include <dolphin/os.h>
#include <string.h>

#define DVDEntryCount 0x100

//.bss
static u8 stack[0x4000];
OSThread dvdmgr_thread;

//.sbss
BOOL dvdmgr_thread_on;
static BOOL (*_callback)(void);
DVDEntry* dvdq; //DVDEntry[0x100]* = 0x9800;

static s32 compare(void* entry1, void* entry2);
void* proc_main(void* param);

void DVDMgrSetupCallback(BOOL (*callback)(void)) {
	_callback = callback;
}

u32 DVDMgrGetLength(DVDEntry* entry) {
	return entry->info.length;
}

void DVDMgrClose(DVDEntry* entry) {
	entry->status |= 8;
}

void DVDMgrReadAsync(DVDEntry* entry, void* address, s32 a3, s32 a4, void (*callback)(s32, DVDFileInfo*)) {
	entry->address = address;
	entry->bytesLeft = a3;
	entry->field_0x84 = a4;
	entry->status |= 1;
	entry->status &= 0xFFFD;
	entry->callback = callback;
	entry->field_0x88 = 0;
}

void DVDMgrRead(DVDEntry* entry, void* address, s32 a3, s32 a4) {
	entry->address = address;
	entry->bytesLeft = a3;
	entry->field_0x84 = a4;
	entry->status |= 1;
	entry->status &= 0xFFFD;
	entry->callback = NULL;
	entry->field_0x88 = 0;
	while (!(entry->status & 2)) {
		OSYieldThread();
	}
}

DVDEntry* DVDMgrOpen(char* path, u8 a2, u16 a3) {
	DVDEntry* entry;
	int i;

	if (DVDConvertPathToEntrynum(path) == -1) return NULL;
	for (i = 0; i < DVDEntryCount; i++) {
		entry = &dvdq[i];
		if (!(entry->status & 0x8000)) {
			break; //found a hit
		}
	}
	if (i >= DVDEntryCount) return NULL;
	memset(&entry, 0, sizeof(DVDEntry));
	strcpy(entry->name, path);
	entry->status = 0;
	entry->field_0x92 = a2;
	entry->field_0x94 = a3;
	entry->address = NULL;
	entry->bytesLeft = 0;
	entry->field_0x84 = 0;
	entry->field_0x88 = 0;
	entry->status |= 0x8000;
	if (DVDOpen(entry->name, &entry->info)) {
		return entry;
	}
	memset(&entry, 0, sizeof(DVDEntry));
	return NULL;
}

void DVDMgrMain(void) {
	DVDEntry* entry;
	u8 array[0x100];
	int i, length, error, dobreak;
	u32 count;

	memset(&array, 0, sizeof(array));
	for (i = 0, count = 0; i < sizeof(array); i++) {
		if (dvdq[i].status & 0x8000) {
			array[i] = (u8)i;
			count++;
		}
	}
	qqsort(array, count, 1, compare);
	for (i = 0; i < count; i++) {
		entry = &dvdq[array[i]];
		if (!(entry->status & 0x80) && (entry->status & 1)) {
			length = entry->bytesLeft;
			if (entry->field_0x92) { //dochunks?
				if (length > 0x40000) {
					length = 0x40000;
				}
			}
			error = DVDRead(&entry->info, entry->address, length,
							entry->field_0x84 + entry->field_0x88);
			if (error == DVD_RESULT_CANCELED) {
				entry->status &= 0xFFFE;
				entry->status |= 2;
				if (entry->callback) {
					entry->callback(error, &entry->info);
				}
			}
			else if (error >= 0) {
				entry->bytesLeft -= length;
				entry->field_0x88 += length;
				entry->address = (void*)((u32)entry->address + length);
				if (!entry->bytesLeft) {
					entry->status &= 0xFFFE;
					entry->status |= 2;
					if (entry->callback) {
						entry->callback(error, &entry->info);
					}
				}
				dobreak = TRUE;
			}
			else {
				entry->status |= 0x20;
				entry->status |= 0x80;
			}
		}
		if (entry->status & 8) {
			DVDClose(&entry->info);
			memset(&entry, 0, sizeof(DVDEntry));
		}
		if (dobreak) break;
	}
	if (_callback) {
		_callback();
	}
}

static s32 compare(void* entry1, void* entry2) {
	u8 value1, value2;
	value1 = (u8)dvdq[*(u8*)entry1].field_0x92;
	value2 = (u8)dvdq[*(u8*)entry2].field_0x92;
	if (value1 > value2) {
		return 1;
	}
	else {
		return -(value1 < value2);
	}
}

void DVDMgrDelete(void) {
	if (dvdmgr_thread_on) {
		OSCancelThread(&dvdmgr_thread);
		dvdmgr_thread_on = FALSE;
	}
}

void DVDMgrInit(void) {
	dvdq = (DVDEntry*)__memAlloc(0, sizeof(DVDEntry) * DVDEntryCount);
	if (!OSCreateThread(&dvdmgr_thread, proc_main, NULL, (void*)((u32)stack + sizeof(stack)), sizeof(stack), 16, OS_THREAD_ATTR_DETACH)) {
		while (1) { ; } //infinite loop
	}
	dvdmgr_thread_on = TRUE;
	OSResumeThread(&dvdmgr_thread);
}

void* proc_main(void* param) {
	while (1) {
		DVDMgrMain();
		OSYieldThread();
	}
}