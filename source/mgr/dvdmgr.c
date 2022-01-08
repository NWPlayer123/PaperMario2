/* "dvdmgr" - DVD File Manager
 * Status: Module finished, write-up/documentation needed
 *
 * Function: easy file handling for loading data from the disc, instead of
 * having to call DVD functions directly and handle errors
 * 
 * Last Update: 12/18/2021 more clean-up, latest changes, update types
 */
#include "mgr/dvdmgr.h"
#include "memory.h"
#include "system.h"
#include <dolphin/os.h>
#include <string.h>

//.bss
OSThread dvdmgr_thread;
static u8 stack[0x4000];

//.sbss
DVDEntry* dvdq;
static DVDMgrCallback _callback;
BOOL dvdmgr_thread_on;

//local prototypes
void* proc_main(void* param);
static s32 compare(const void* a, const void* b);

void* proc_main(void* param) {
	while (1) {
		DVDMgrMain();
		OSYieldThread();
	}
}

void DVDMgrInit(void) {
	dvdq = __memAlloc(HEAP_DEFAULT, sizeof(DVDEntry) * DVDEntryCount);
	memset(dvdq, 0, sizeof(DVDEntry) * DVDEntryCount);
	if (!OSCreateThread(&dvdmgr_thread, proc_main, NULL, (void*)((u32)stack + sizeof(stack)), sizeof(stack), 16, OS_THREAD_ATTR_DETACH)) {
		while (1) {}
	}
	dvdmgr_thread_on = TRUE;
	OSResumeThread(&dvdmgr_thread);
}

void DVDMgrDelete(void) {
	if (dvdmgr_thread_on) {
		OSCancelThread(&dvdmgr_thread);
		dvdmgr_thread_on = FALSE;
	}
}

static s32 compare(const void* a, const void* b) {
	u8 value1, value2;
	value1 = (u8)dvdq[*(u8*)a].priority;
	value2 = (u8)dvdq[*(u8*)b].priority;
	if (value1 > value2) {
		return 1;
	}
	else {
		return -(value1 < value2);
	}
}

void DVDMgrMain(void) {
	u8 array[0x100];
	u8* table;
	DVDEntry* entry;
	s32 i, length, error, dobreak;
	u32 count = 0;

	memset(&array, 0, sizeof(array));
	for (entry = dvdq, i = 0; i < DVDEntryCount; i++, entry++) {
		if (entry->status & DVDMGR_INUSE) {
			array[i] = (u8)i;
			count++;
		}
	}
	qqsort(array, count, sizeof(u8), compare);
	for (table = array, i = 0; i < count; i++, table++) {
		dobreak = FALSE;
		entry = &dvdq[*table];
		if (!(entry->status & DVDMGR_UNK_ERROR2) && (entry->status & DVDMGR_READING)) {
			length = entry->bytesLeft;
			if (entry->priority && length > 0x40000) {
				length = 0x40000;
			}
			error = DVDRead(&entry->info, entry->address, length, entry->offset + entry->position);
			if (error == DVD_RESULT_CANCELED) {
				entry->status &= ~DVDMGR_READING;
				entry->status |= DVDMGR_FINISHED;
				if (entry->callback) {
					entry->callback(error, &entry->info);
				}
			}
			else if (error >= 0) {
				entry->bytesLeft -= length;
				entry->position += length;
				entry->address = (void*)((s32)entry->address + length);
				if (!entry->bytesLeft) {
					entry->status &= ~DVDMGR_READING;
					entry->status |= DVDMGR_FINISHED;
					if (entry->callback) {
						entry->callback(error, &entry->info);
					}
				}
				dobreak = TRUE;
			}
			else {
				entry->status |= DVDMGR_UNK_ERROR1;
				entry->status |= DVDMGR_UNK_ERROR2;
			}
		}
		if (entry->status & DVDMGR_CLOSED) {
			DVDClose(&entry->info);
			memset(&entry, 0, sizeof(DVDEntry));
		}
		if (dobreak) break;
	}
	if (_callback) {
		_callback();
	}
}

DVDEntry* DVDMgrOpen(const char* path, u8 priority, u16 unknown) {
	DVDEntry* entry;
	int i;

	if (DVDConvertPathToEntrynum(path) == -1) {
		return NULL;
	}

	for (entry = dvdq, i = 0; i < DVDEntryCount; i++, entry++) {
		if (!(entry->status & DVDMGR_INUSE)) {
			break;
		}
	}
	if (i >= 256) {
		return NULL;
	}

	memset(entry, 0, sizeof(DVDEntry));
	strcpy(entry->name, path);
	entry->status = 0;
	entry->priority = priority;
	entry->unknown = unknown;
	entry->address = NULL;
	entry->bytesLeft = 0;
	entry->offset = 0;
	entry->position = 0;
	entry->status |= DVDMGR_INUSE;
	if (DVDOpen(entry->name, &entry->info)) {
		return entry;
	}
	memset(entry, 0, sizeof(DVDEntry));
	return NULL;
}

u32 DVDMgrRead(DVDEntry* entry, void* address, u32 size, s32 offset) {
	entry->address = address;
	entry->bytesLeft = size;
	entry->offset = offset;
	entry->status |= DVDMGR_READING;
	entry->status &= ~DVDMGR_FINISHED;
	entry->callback = NULL;
	entry->position = 0;
	while (!(entry->status & DVDMGR_FINISHED)) {
		OSYieldThread();
	}
	return entry->info.length;
}

void DVDMgrReadAsync(DVDEntry* entry, void* address, u32 size, s32 offset, DVDCallback callback) {
	entry->address = address;
	entry->bytesLeft = size;
	entry->offset = offset;
	entry->status |= DVDMGR_READING;
	entry->status &= ~DVDMGR_FINISHED;
	entry->callback = callback;
	entry->position = 0;
}

void DVDMgrClose(DVDEntry* entry) {
	entry->status |= DVDMGR_CLOSED;
}

u32 DVDMgrGetLength(DVDEntry* entry) {
	return entry->info.length;
}

void DVDMgrSetupCallback(DVDMgrCallback callback) {
	_callback = callback;
}
