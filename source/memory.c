#include "memory.h"

//.bss
smartWork_s smartWork;

//.sdata
//flag, size / 0x400 (>> 10)
u32 size_table[6][2] = {
	{1, 0x1C84},
	{1, 0xD90},
	{1, 0xE0},
	{1, 0x80},
	{1, 0x100},
	{0, 0x64}
};
smartWork_s* wp = &smartWork;

//.sbss
u32 count;
void* g_bFirstSmartAlloc;
u32 mapalloc_size;
void* mapalloc_base_ptr;
u32* heapEnd[6];
u32* heapStart[6];
OSHeapHandle heapHandle[6];

GXTexObj* smartTexObj(GXTexObj* obj, void** image_ptr) {
	if (image_ptr != NULL)
		GXInitTexObjData(obj, *image_ptr);

	return obj;
}

/*
void memInit(void) {
	int i;
	u32 size;
	void* arenaStart;
	void* arenaEnd;
	u32 heap;
	u32 temp;
	void* ptr;

	arenaStart = OSGetArenaLo();
	arenaEnd = OSGetArenaHi();
	arenaStart = OSInitAlloc(arenaStart, arenaEnd, 6); //6 heaps
	OSSetArenaLo(arenaStart);
	arenaStart = (void*)OSRoundUp32B(arenaStart);
	arenaEnd = (void*)OSRoundDown32B(arenaEnd);
	heap = (u32)arenaStart;

	for (i = 0; i < 6; i++) {
		if (size_table[i][0] == 1) {
			heapStart[i] = (void*)heap;
			heapEnd[i] = (void*)((size_table[i][1] << 10) + heap);
			heap += size_table[i][1] << 10;
		}
	}

	arenaStart = (void*)((u32)arenaEnd - heap); //r31

	for (i = 0; i < 6; i++) {
		if (size_table[i][0] == 0) {
			temp = ((u64)size_table[i][1] * (u64)arenaStart) / 100Ull;
			heapStart[i] = (void*)heap;
			heapEnd[i] = (void*)(heap + (temp - (temp & 31)));
			heap += (temp - (temp & 31));
			//heapEnd[i] = (void*)((void*)(OSRoundDown32B(((u32)arenaEnd - (u32)arenaStart) / 100)) + (u32)arenaStart);
		}
	}

	for (i = 0; i < 6; i++) {
		heapHandle[i] = OSCreateHeap(heapStart[i], heapEnd[i]);
	}
	OSSetArenaLo(arenaEnd);
	for (i = 0; i < 6; i++) {
		OSDestroyHeap(heapHandle[i]);
		if (OSCreateHeap(heapStart[i], heapEnd[i]) == 1) { //allocate from second heap
			size = (u32)heapEnd[1] - (u32)heapStart[1] - 32;
			ptr = OSAllocFromHeap(heapHandle[i], size);
			if (ptr != NULL) {
				memset(ptr, 0, size);
				DCFlushRange(ptr, size);
			}
			*(u32*)((u32)ptr + 0) = 0;
			*(u32*)((u32)ptr + 4) = size - 32;
			*(u16*)((u32)ptr + 8) = 0;
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
}*/

//TODO: extreme cleanup
smartEntry* smartAlloc(u32 size, u8 type) {
	int i;
	smartEntry *entry1, *entry2;
	if (g_bFirstSmartAlloc == 0) {
		g_bFirstSmartAlloc = 1;
		smartAutoFree(3);
	}
	if (wp->waitsync != 0) {
		sysWaitDrawSync();
		wp->waitsync = 0;
	}

	entry2 = wp->head; //TODO: not head?

	if (entry2->prev == NULL) {
		wp->head = entry2->next;
		if (wp->head != NULL) {
			wp->head->prev = NULL;
		}
	} else {
		entry2->prev->next = entry2->next;
	}

	if (entry2->next == NULL) {
		wp->tail = entry2->prev;
		if (wp->tail != NULL) {
			wp->tail->next = NULL;
		}
	} else {
		entry2->next->prev = entry2->prev;
	}

	if (size & 31) {
		//technically size += 32 - (size & 31) but this is easier
		size = OSRoundUp32B(size);
	}
	entry2->field_0xC = 1;
	entry2->type = type;
	entry2->size = size;
	entry2->field_0x8 = 0;
	if (wp->bytesLeft < size) { //full alloc
		entry1 = wp->field_0xE008;
		while (entry1 != NULL) {
			if (size <= entry1->field_0x10) {
				entry2->field_0x0 += entry1->size;
				entry2->field_0x10 -= size;
				entry2->next = entry1->next;
				entry2->prev = entry1;
				entry1->field_0x10 = 0;
				if (entry1->next == NULL) {
					wp->field_0xE00C = entry2;
				} else {
					entry1->next->prev = entry2;
				}
				entry1->next = entry2;
				return entry2;
			}
			entry1 = entry1->next;
		}
		for (i = 0; i < 3; i++) {
			switch (i) {
				case 1:
					_fileGarbage(1);
					break;
				case 2:
					_fileGarbage(0);
					break;
				//default, fall through
			}
			smartGarbage();
			entry1 = wp->field_0xE00C;
			if (size <= entry1->field_0x10) {
				entry2->field_0x0 = entry1->field_0x0 + entry1->size;
				entry2->field_0x10 = entry1->field_0x10 - size;
				entry2->next = entry1->next;
				entry2->prev = entry1;
				entry1->field_0x10 = 0;
				if (entry1->next == (smartEntry*)0x0) {
					wp->field_0xE00C = entry2;
				}
				else {
					entry1->next->prev = entry2;
				}
				entry1->next = entry2;
				return entry2;
			}
		}
		entry2 = NULL;
	} else {
		entry2->field_0x0 = wp->ptr;
		entry2->field_0x10 = wp->bytesLeft - size;
		entry2->next = wp->field_0xE008;
		entry2->prev = NULL;
		wp->bytesLeft = 0;
		if (wp->field_0xE008 != NULL) {
			wp->field_0xE008->prev = entry2;
		}
		wp->field_0xE008 = entry2;
		if (entry2->next == NULL) {
			entry2->field_0x10 = (u32)wp->ptr + ((((heapEnd[5] - heapStart[5]) - 32) - entry2->field_0x0) - entry2->size);
			wp->field_0xE00C = entry2;
		}
		
	}
}

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

void smartInit(void) {
	int i;
	void* ptr;
	u32 size;
	size = (heapEnd[5] - heapStart[5]) - 32;
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

