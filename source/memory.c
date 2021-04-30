#include "memory.h"
#include <dolphin/os.h>
#include <string.h> //memset
#include "system.h"

//.bss
smartWork_s smartWork;

//.sdata
//flag, size / 0x400 (>> 10)
u32 size_table[6][2] = {
	{1, 0x1C84}, //0x721000, 7300 KiB/7.13 MiB
	{1, 0xD90}, //0x364000, 3472 KiB/3.39 MiB
	{1, 0xE0}, //0x38000, 224 KiB
	{1, 0x80}, //0x20000, 128 KiB
	{1, 0x100}, //0x40000, 256 KiB
	{0, 0x64}
};
static smartWork_s* wp = &smartWork;

//.sbss
u32 count;
BOOL g_bFirstSmartAlloc;
u32 mapalloc_size;
void* mapalloc_base_ptr;
void* heapEnd[6];
void* heapStart[6];
OSHeapHandle heapHandle[6];


//unfinished x4
void smartAutoFree(int r3);
void _fileGarbage(int r3);
void smartGarbage(void);
void smartReInit(void); //need to double check


GXTexObj* smartTexObj(GXTexObj* obj, void** image_ptr) {
	if (image_ptr != NULL)
		GXInitTexObjData(obj, *image_ptr);

	return obj;
}

void smartAutoFree(int r3) {

}

void smartGarbage(void) {

}

void smartReInit(void) {

}


//finished
void memInit(void) {
	void *arenaLo, *arenaHi, *heap, *ptr;
	u32 temp, size;
	int i;

	arenaLo = OSGetArenaLo();
	arenaHi = OSGetArenaHi();
	arenaLo = OSInitAlloc(arenaLo, arenaHi, 6);
	OSSetArenaLo(arenaLo);
	arenaLo = (void*)OSRoundUp32B(arenaLo);
	arenaHi = (void*)OSRoundDown32B(arenaHi);
	heap = arenaLo;


	for (i = 0; i < 6; i++) {
		if (size_table[i][0] == 1) {
			heapStart[i] = heap;
			heap = (void*)((u32)heap + (size_table[i][1] << 10));
			heapEnd[i] = heap;
		}
	}

	arenaLo = (void*)((u32)arenaHi - (u32)heap);

	for (i = 0; i < 6; i++) {
		if (size_table[i][0] == 0) {
			temp = (u32)((size_table[i][1] * (u64)arenaLo) / 100Ull); //64-bit, don't lose precision
			heapStart[i] = heap;
			heap = (void*)((u32)heap + (temp - (temp & 31)));
			heapEnd[i] = heap;
		}
	}

	for (i = 0; i < 6; i++) {
		heapHandle[i] = OSCreateHeap(heapStart[i], heapEnd[i]);
	}
	OSSetArenaLo(arenaHi);
	for (i = 0; i < 6; i++) {
		OSDestroyHeap(heapHandle[i]);
		if (OSCreateHeap(heapStart[i], heapEnd[i]) == 1) { //use heap 1 for allocation
			size = (u32)heapEnd[1] - (u32)heapStart[1] - 32;
			ptr = OSAllocFromHeap(heapHandle[i], size); //TODO: just call __memAlloc?
			if (ptr != NULL) {
				memset(ptr, 0, size);
				DCFlushRange(ptr, size);
			}
			*(u32*)((u32)ptr + 0) = 0; //TODO: memory object prototype?
			*(u32*)((u32)ptr + 4) = size - 32;
			*(u32*)((u32)ptr + 8) = 0;
			mapalloc_base_ptr = ptr;
			mapalloc_size = size;
			count = 0;
		}
	}
}

void* __memAlloc(u32 heap, u32 size) {
	void* ptr;

	ptr = OSAllocFromHeap(heapHandle[heap], size);
	if (ptr != NULL) {
		memset(ptr, 0, size);
		DCFlushRange(ptr, size);
	}
	return ptr;
}

void __memFree(u32 heap, void* ptr) {
	OSFreeToHeap(heapHandle[heap], ptr);
}

smartEntry* smartAlloc(u32 size, u8 type) {
	smartEntry *entry1, *entry2;
	int i;

	if (g_bFirstSmartAlloc == FALSE) {
		g_bFirstSmartAlloc = TRUE;
		smartAutoFree(3);
	}

	if (wp->waitsync) {
		sysWaitDrawSync();
		wp->waitsync = 0;
	}

	entry2 = wp->head;

	if (entry2->prev) {
		entry2->prev->next = entry2->next;
	}
	else {
		wp->head = entry2->next;
		if (wp->head) {
			wp->head->prev = NULL;
		}
	}

	if (entry2->next) {
		entry2->next->prev = entry2->prev;
	}
	else {
		wp->tail = entry2->prev;
		if (wp->tail) {
			wp->tail->next = NULL;
		}
	}

	if (size & 31) {
		size = OSRoundUp32B(size);
	}

	entry2->field_0xC = 1;
	entry2->type = type;
	entry2->size = size;
	entry2->field_0x8 = 0;
	if (wp->bytesLeft < size) { //full alloc
		for (entry1 = wp->field_0xE008; entry1; entry1 = entry1->next) {
			if (entry1->field_0x10 >= size) {
				entry2->address = (void*)((u32)entry1->address + entry1->size);
				entry2->field_0x10 = entry1->field_0x10 - size;
				entry2->next = entry1->next;
				entry2->prev = entry1;
				entry1->field_0x10 = 0;
				if (entry1->next) {
					entry1->next->prev = entry2;
				}
				else {
					wp->field_0xE00C = entry2;
				}
				entry1->next = entry2;
				return entry2;
			}
		}
		for (i = 0; i < 3; i++) {
			switch (i) {
				case 1:
					_fileGarbage(1);
					break;
				case 2: //TODO: default?
					_fileGarbage(0);
					break;
					//default, fall through
			}
			smartGarbage();
			entry1 = wp->field_0xE00C;
			if (entry1->field_0x10 >= size) {
				entry2->address = (void*)((u32)entry1->address + entry1->size);
				entry2->field_0x10 = entry1->field_0x10 - size;
				entry2->next = entry1->next;
				entry2->prev = entry1;
				entry1->field_0x10 = 0;
				if (entry1->next) {
					entry1->next->prev = entry2;
				}
				else {
					wp->field_0xE00C = entry2;
				}
				entry1->next = entry2;
				return entry2;
			}
		}
		return NULL;
	}
	else {
		entry2->address = wp->ptr;
		entry2->field_0x10 = wp->bytesLeft - size;
		entry2->next = wp->field_0xE008;
		entry2->prev = NULL;
		wp->bytesLeft = 0;
		if (wp->field_0xE008) {
			wp->field_0xE008->prev = entry2;
		}
		wp->field_0xE008 = entry2;
		if (!entry2->next) {
			entry2->field_0x10 = (u32)wp->ptr + (((((u32)heapEnd[5] - (u32)heapStart[5]) - 32) - (u32)entry2->address) - entry2->size);
			wp->field_0xE00C = entry2;
		}
		return entry2;
	}
}

void _mapFree(OSModuleInfo* module) {
	/*OSModuleInfo *prev_module, *next_module;

	if (module) {
		prev_module = &module[-1];
		if (prev_module->link.prev) {
			next_module = prev_module->link.next;
			if ((prev_module->id != 0) && 
		}
	}*/
}

void* _mapAlloc(u32 size) {
	return NULL;
}

void smartInit(void) {
	int i;
	void* ptr;
	u32 size;
	size = (u32)heapEnd[5] - (u32)heapStart[5] - 32;
	ptr = OSAllocFromHeap(heapHandle[5], size);
	if (ptr != NULL) {
		memset(ptr, 0, size);
		DCFlushRange(ptr, size);
	}
	wp->ptr = ptr;
	wp->field_0xE008 = 0;
	wp->field_0xE00C = 0;
	wp->bytesLeft = size;
	memset(wp->ptr, 0, size);
	memset(wp->work, 0, sizeof(wp->work));
	for (i = 0; i < 0x800; i++) {
		wp->work[i].next = &wp->work[i + 1];
		wp->work[i].prev = &wp->work[i - 1];
	}
	wp->head = &wp->work[0];
	wp->head->prev = NULL;
	wp->tail = &wp->work[0x7FF];
	wp->tail->next = NULL;
	wp->waitsync = 0;
	g_bFirstSmartAlloc = FALSE;
}


/*
* TODO: cleanup

void smartReInit(void) {
	u32 size;
	int i;
	wp->field_0xE008 = 0;
	wp->field_0xE00C = 0;
	size = (heapEnd[5] - heapStart[5]) - 32;
	wp->bytesLeft = size;
	memset(wp->ptr, 0, size);
	memset(wp->work, 0, 0xE000);
	for (i = 0; i < 0x800; i++) {
		wp->work[i].next = &wp->work[i + 1];
		wp->work[i].prev = &wp->work[i - 1];
	}
	wp->head = &wp->work[0];
	wp->head->prev = NULL;
	wp->tail = &wp->work[0x7FF]; //TODO: double check
	wp->tail->next = NULL;
	wp->waitsync = 0;
	g_bFirstSmartAlloc = 0;
}


*/