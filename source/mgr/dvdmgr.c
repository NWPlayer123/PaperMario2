/* "dvdmgr" - DVD File Manager
 * Status: Module finished, write-up/documentation needed
 *
 * Function: easy file handling for loading data from the disc, instead of
 * having to call DVD functions directly and handle errors
 * 
 * Last Update: 5/1/2021 clean-up and re-ordering to match the rest of the codebase
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
	dvdq = (DVDEntry*)__memAlloc(HEAP_DEFAULT, sizeof(DVDEntry) * DVDEntryCount);
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
	DVDEntry* entry;
	u8 array[0x100];
	s32 i, length, error, dobreak;
	u32 count;

	memset(&array, 0, sizeof(array));
	for (i = 0, count = 0; i < sizeof(array); i++) {
		if (dvdq[i].status & DVDMGR_INUSE) {
			array[i] = (u8)i;
			count++;
		}
	}
	qqsort(array, count, 1, compare);
	for (i = 0; i < count; i++) {
		entry = &dvdq[array[i]];
		if (!(entry->status & DVDMGR_UNK_ERROR2) && (entry->status & DVDMGR_READING)) {
			length = (s32)entry->bytesLeft;
			if (entry->priority) {
				if (length > 0x40000) {
					length = 0x40000;
				}
			}
			error = DVDRead(&entry->info, entry->address, length,
				(s32)(entry->offset + entry->position));
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
				entry->address = (void*)((u32)entry->address + length);
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

DVDEntry* DVDMgrOpen(const char* path, u32 priority, u16 unknown)
{
	DVDEntry* entry; // r31
	int v8; // r3
	int v9; // ctr
	u16 v10; // r0
	u16 v11; // r0
	u16 v12; // r0
	u16 v13; // r0
	u16 v14; // r0
	u16 v15; // r0
	u16 v16; // r0

	if (DVDConvertPathToEntrynum(path) == -1)
	{
		return 0;
	}
	entry = dvdq;
	v8 = 0;
	v9 = 32;
	do
	{
		if ((entry->status & 0x8000) == 0)
		{
			break;
		}
		v10 = entry[1].status;
		++v8;
		++entry;
		if ((v10 & 0x8000) == 0)
		{
			break;
		}
		v11 = entry[1].status;
		++v8;
		++entry;
		if ((v11 & 0x8000) == 0)
		{
			break;
		}
		v12 = entry[1].status;
		++v8;
		++entry;
		if ((v12 & 0x8000) == 0)
		{
			break;
		}
		v13 = entry[1].status;
		++v8;
		++entry;
		if ((v13 & 0x8000) == 0)
		{
			break;
		}
		v14 = entry[1].status;
		++v8;
		++entry;
		if ((v14 & 0x8000) == 0)
		{
			break;
		}
		v15 = entry[1].status;
		++v8;
		++entry;
		if ((v15 & 0x8000) == 0)
		{
			break;
		}
		v16 = entry[1].status;
		++v8;
		++entry;
		if ((v16 & 0x8000) == 0)
		{
			break;
		}
		++v8;
		++entry;
		--v9;
	}   while (v9);
	if (v8 >= 256)
	{
		return 0;
	}
	memset(entry, 0, sizeof(DVDEntry));
	strcpy(entry->name, path);
	entry->status = 0;
	entry->priority = (u8)priority;
	entry->unknown = unknown;
	entry->address = 0;
	entry->bytesLeft = 0;
	entry->offset = 0;
	entry->position = 0;
	entry->status |= 0x8000u;
	if (DVDOpen(entry->name, &entry->info))
	{
		return entry;
	}
	memset(entry, 0, sizeof(DVDEntry));
	return 0;
}

/*
//default priority is 2
DVDEntry* DVDMgrOpen(const char* path, u32 priority, u16 unknown) {
	DVDEntry* entry;
	int i;

	if (DVDConvertPathToEntrynum(path) == -1) {
		return NULL;
	}

	for (i = 0; i < DVDEntryCount; i++) {
		if (!(dvdq[i].status & DVDMGR_INUSE)) {
			break; //found a hit
		}
	}
	if (i >= DVDEntryCount) return NULL;
	entry = &dvdq[i];
	memset(&entry, 0, sizeof(DVDEntry));
	strcpy(entry->name, path);
	entry->status = 0;
	entry->priority = (u8)priority;
	entry->unknown = unknown;
	entry->address = NULL;
	entry->bytesLeft = 0;
	entry->offset = 0;
	entry->position = 0;
	entry->status |= DVDMGR_INUSE;
	if (DVDOpen(entry->name, &entry->info)) {
		return entry;
	}
	memset(&entry, 0, sizeof(DVDEntry));
	return NULL;
}*/

void DVDMgrRead(DVDEntry* entry, void* address, u32 size, u32 offset) {
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
}

void DVDMgrReadAsync(DVDEntry* entry, void* address, u32 size, u32 offset, void (*callback)(s32, DVDFileInfo*)) {
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

void DVDMgrSetupCallback(BOOL (*callback)(void)) {
	_callback = callback;
}
