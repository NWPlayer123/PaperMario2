#include "mgr/filemgr.h"
#include "memory.h"
#include <stdarg.h>
#include <string.h>

//.bss
char _filename[0x400]; //for fileAllocf/fileAsyncf vsprintf

//.sbss
filemgrWork fileWork;

//.sdata
filemgrWork* afp = &fileWork;



extern int vsprintf(char* string, const char* format, va_list arg);

fileObj* _fileAlloc(char* filename, int r4, int r5);

fileObj* _fileAlloc(char* filename, int r4, int r5) {
	return NULL;
}

void fileFree(fileObj* handle) {

}

int fileAsyncf(int a1, int a2, char* a3, ...) {
	return 0;
}

fileObj* fileAlloc(char* filename, int smth) {
	return _fileAlloc(filename, smth, 0);
}

fileObj* fileAllocf(int smth, char* format, ...) {
	va_list va;

	va_start(va, format);
	vsprintf(_filename, format, va);
	return _fileAlloc(_filename, smth, 0);
}

/*
void _fileGarbage(void* someobj) {

}

void fileGarbageMoveMem(void* moveto, void* someobj) {

}

void fileGarbageDataAdrSet(void* someobj, s32 type) {
	switch 0xA filetype
}

void fileGarbageDataAdrClear(void* someobj) {
	switch 0xA filetype obj+1, smartObj?
}
*/

void fileInit(void) {
	fileObj* ptr;
	int i;

	afp->field_0x0 = __memAlloc(0, sizeof(fileObj) * 0x400);
	afp->mCurrentArchiveType = 0;
	afp->field_0x8 = 0;
	afp->field_0xC = 0;
	memset(afp->field_0x0, 0, sizeof(fileObj) * 0x400);

	//TODO: unroll better
	ptr = &afp->field_0x0[0];
	for (i = 0; i < 0x400; i++) {
		ptr->next = &ptr[1];
		ptr++;
	}

	afp->field_0x10 = afp->field_0x0; //"head"?
	afp->field_0x14 = &afp->field_0x0[0x3FF]; //"tail"?
	afp->field_0x14->next = NULL;
}

//TODO: enum of all archive types
void fileSetCurrentArchiveType(u32 type) {
	afp->mCurrentArchiveType = type;
}

/*
* TODO: sort through old file
* 
char _filename[0x400];
fileWork_gp fileWork;
fileWork_gp* afp = &fileWork;

void* _fileAlloc(char* filename, u32 smth, u32 flag) {

}

void* fileAlloc(char* filename, u32 smth) {
	return _fileAlloc(filename, smth, 0);
}

void* fileAllocf(u32 smth, char* format, ...) {
	void* ret;
	va_list args;

	va_start(args, format);
	vsprintf(_filename, format, args);
	ret = _fileAlloc(_filename, smth, 0);
	va_end(args);
	return ret;
}

//fucky linked lists, TODO: double check asm
void fileInit(void) {
	int i;
	file_somestruct* ptr;

	afp->list_start = __memAlloc(0, 0x2C000);
	afp->field_0x4 = 0;
	afp->field_0x8 = 0;
	afp->field_0xC = 0;
	memset(afp->list_start, 0, 0x2C000);

	ptr = afp->list_start; //cast
	for (i = 0; i < 1024; i++) { //unrolled into 128 * 8 writes per loop
		ptr[i].next_entry = &ptr[i + 1];
	}
	afp->list_base = afp->list_start;
	afp->list_end = &ptr[1023]; //last entry
	ptr[1023].next_entry = 0;
}*/