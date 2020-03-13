#include <dolphin/types.h>
#include <dolphin/os.h>

//flag, size / 0x400 (>> 10)
__declspec(section ".sdata") u32 size_table[6][2] = {
	{1, 0x1C84},
	{1, 0xD90},
	{1, 0xE0},
	{1, 0x80},
	{1, 0x100},
	{0, 0x64}
};
__declspec(section ".sdata") void* heapStart[6];
__declspec(section ".sdata") void* heapEnd[6];
__declspec(section ".sdata") OSHeapHandle heapHandle[6];
void* mapalloc_base_ptr;
u32 mapalloc_size;
u32 count;


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
}
