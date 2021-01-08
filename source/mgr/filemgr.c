#include "mgr/filemgr.h"
#include <stdarg.h>

extern int vsprintf(char* string, const char* format, va_list arg);

char _filename[0x400]; //internal, for fileAllocf

filemgr_handle* _fileAlloc(char* filename, int r4, int r5);

filemgr_handle* fileAlloc(char* filename, int r4) {
	return _fileAlloc(filename, r4, 0);
}

filemgr_handle* fileAllocf(int r3, char* format, ...) {
	va_list va;

	va_start(va, format);
	vsprintf(_filename, format, va);
	return _fileAlloc(_filename, r3, 0);
}

filemgr_handle* _fileAlloc(char* filename, int r4, int r5) {
	return NULL;
}

void fileFree(filemgr_handle* handle) {

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