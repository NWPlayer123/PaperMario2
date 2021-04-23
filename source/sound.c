#include "sound.h"
#include "memory.h"
#include <dolphin/ai.h>
#include <dolphin/ar.h>
#include <dolphin/arq.h>
#include <musyx.h>

//.sbss
u32 aramMemArray[8];

//local prototypes
void* sndMalloc(u32 size);
void sndFree(void* ptr);

void* sndMalloc(u32 size) {
	return __memAlloc(HEAP_DEFAULT, size);
}

void sndFree(void* ptr) {
	__memFree(HEAP_DEFAULT, ptr);
}

void SoundInit(void) {
	ARInit(aramMemArray, 8);
	ARQInit();
	AIInit(NULL);
	ARAlloc(0xBFC000); //12MB-16KB (reserved)
}

BOOL SoundLoadDVD2(const char* path) {
	return FALSE;
}