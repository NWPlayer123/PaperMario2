#include "mgr/filemgr.h"
#include "drv/arcdrv.h"
#include "memory.h"
#include <stdarg.h>
#include <string.h>

extern int vsprintf(char* string, const char* format, va_list arg);
extern OSThread dvdmgr_thread;

//.bss
char _filename[0x400]; //for fileAllocf/fileAsyncf vsprintf

//.sbss
filemgrWork fileWork;

//.sdata
filemgrWork* afp = &fileWork;

//local prototypes
void fileGarbageDataAdrClear(u8 type);
void fileGarbageDataAdrSet(void* data, u8 type);
fileObj* _fileAlloc(const char* filename, u8 type, void (*callback)(fileObj*));
void dvdReadDoneCallBack(s32 error, DVDFileInfo* info);
s32 fileAsync(const char* filename, u8 type, void (*callback)(fileObj*));



void fileInit(void) {
	fileObj* ptr;
	int i;

	afp->entries = __memAlloc(HEAP_DEFAULT, sizeof(fileObj) * 0x400);
	afp->mCurrentArchiveType = 0;
	afp->firstused = 0;
	afp->lastused = 0;
	memset(afp->entries, 0, sizeof(fileObj) * 0x400);

	//TODO: unroll better
	ptr = afp->entries;
	for (i = 0; i < 0x400; i++) {
		ptr->next = ++ptr;
	}

	afp->firstavailable = afp->entries;
	afp->lastavailable = &afp->entries[0x3FF];
	afp->lastavailable->next = NULL;
}

void fileGarbageDataAdrClear(u8 type) {

}

void fileGarbageDataAdrSet(void* data, u8 type) {

}

void fileGarbageMoveMem(void* data, fileObj* file) {

}

void _fileGarbage(BOOL a1) {

}

fileObj* fileAllocf(u8 type, const char* format, ...) {
	va_list va;

	va_start(va, format);
	vsprintf(_filename, format, va);
	return _fileAlloc(_filename, type, NULL);
}

fileObj* fileAlloc(const char* filename, u8 type) {
	return _fileAlloc(filename, type, NULL);
}

fileObj* _fileAlloc(const char* filename, u8 type, void (*callback)(fileObj*)) {
	smartEntry* smart;
	DVDEntry* entry;
	fileObj *search, *newentry, *temp;
	void* handle;
	u32 size, test;

	for (search = afp->firstused; search; search = search->next) {
		if (search->state && !strcmp(search->filename, filename)) {
			while (search->entry) {;} //infinite loop????
			if (search->state == 3) {
				search->state = 1;
				search->references = 0;
			}
			else if (search->state == 2) {
				search->state = 1;
			}
			search->references++;
			return search;
		}
	}
	newentry = afp->firstavailable;
	if (!newentry) {
		_fileGarbage(1);
		newentry = afp->firstavailable;
	}
	if (!newentry) {
		_fileGarbage(0);
		newentry = afp->firstavailable;
	}

	handle = NULL;
	temp = newentry->next;

	switch (afp->mCurrentArchiveType) {
		case 0:
			handle = arcOpen(filename, NULL, NULL);
			break;
		case 1:
			handle = arcOpen(filename, NULL, NULL);
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			break;
		case 2:
			handle = arcOpen(filename, NULL, NULL);
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			break;
	}
	if (handle) {
		fileGarbageDataAdrSet(handle, type);
	}
	if (handle) {
		newentry->state = 1;
		newentry->mppFileData = newentry->field_0x4;
		*newentry->mppFileData = handle;
		newentry->references = 1;
		newentry->field_0x1 = type;
		newentry->next = NULL;
		strcpy(newentry->filename, filename);
		newentry->entry = NULL;
	}
	else {
		entry = DVDMgrOpen(filename, 2, 0);
		if (!entry) {
			return NULL;
		}
		size = DVDMgrGetLength(entry);
		test = ((32 - (size & 31)) + size) & ((size & 31) != 0);
		if (test) {
			DVDMgrClose(entry);
			return NULL;
		}
		if (test & 31) {
			test += 32 - (test & 31);
		}
		smart = smartAlloc(test, 0);
		smart->field_0x8 = newentry;
		DVDMgrRead(entry, smart->address, (s32)test, 0);
		DVDMgrClose(entry);
		newentry->mppFileData = &smart->address;
		newentry->state = 1;
		newentry->references = 1;
		newentry->field_0x1 = type;
		newentry->next = NULL;
		strcpy(newentry->filename, filename);
		newentry->entry = NULL;
		fileGarbageDataAdrSet(smart->address, type);
	}
	afp->firstavailable = temp;
	if (!afp->firstavailable) {
		afp->lastavailable = NULL;
	}
	if (afp->firstused) {
		afp->lastused->next = newentry;
	}
	else {
		afp->firstused = newentry;
	}
	afp->lastused = newentry;
	return newentry;
}

void fileFree(fileObj* handle) {
	if (handle) {
		if (handle->mppFileData) {
			if (handle->state == 1) {
				handle->references--;
				if (!handle->references) {
					handle->state = 2;
				}
			}
		}
	}
}

void dvdReadDoneCallBack(s32 error, DVDFileInfo* info) {

}

s32 fileAsyncf(u8 type, void (*callback)(fileObj*), const char* format, ...) {
	va_list va;

	va_start(va, format);
	vsprintf(_filename, format, va);
	return fileAsync(_filename, type, callback);
}

s32 fileAsync(const char* filename, u8 type, void (*callback)(fileObj*)) {
	fileObj *search, *newentry, *temp;
	smartEntry* smart;
	DVDEntry* entry;
	void* handle;
	u32 size, test;

	for (search = afp->firstused; search; search = search->next) {
		if (search->state && !strcmp(search->filename, filename)) {
			if (search->entry) return 0;
			if (search->state == 3) return (s32)search;
			if (search->state == 2) {
				search->state = 3;
			}
			else {
				return (s32)search;
			}
		}
	}

	newentry = afp->firstavailable;
	if (!newentry) {
		_fileGarbage(1);
		newentry = afp->firstavailable;
	}
	if (!newentry) {
		_fileGarbage(0);
		newentry = afp->firstavailable;
	}

	handle = NULL;
	temp = newentry->next;

	switch (afp->mCurrentArchiveType) {
		case 0:
			handle = arcOpen(filename, NULL, NULL);
			break;
		case 1:
			handle = arcOpen(filename, NULL, NULL);
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			break;
		case 2:
			handle = arcOpen(filename, NULL, NULL);
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			if (!handle) {
				handle = arcOpen(filename, NULL, NULL);
			}
			break;
	}

	if (handle) {
		fileGarbageDataAdrSet(handle, type);
	}
	
	if (handle) {
		newentry->state = 3;
		newentry->mppFileData = &newentry->field_0x4;
		*newentry->mppFileData = handle;
		newentry->references = 0;
		newentry->field_0x1 = type;
		newentry->next = 0;
		strcpy(newentry->filename, filename);
		newentry->entry = NULL;
		afp->firstavailable = afp->firstavailable->next;
		if (!afp->firstavailable) {
			afp->lastavailable = NULL;
		}
		if (afp->firstused) {
			afp->lastused->next = newentry;
		}
		else {
			afp->firstused = newentry;
		}
		afp->lastused = newentry;
		return (s32)newentry;
	}
	else {
		entry = DVDMgrOpen(filename, 2, 0);
		if (!entry) {
			return -1;
		}
		size = DVDMgrGetLength(entry);
		test = ((32 - (size & 31)) + size) & ((size & 31) != 0);
		if (test) {
			DVDMgrClose(entry);
			return 0;
		}
		DVDMgrClose(entry);
		if (test & 31) {
			test += 32 - (test & 31);
		}
		smart = smartAlloc(test, 0);
		smart->field_0x8 = newentry;
		newentry->mppFileData = &smart->address;
		newentry->state = 3;
		newentry->references = 0;
		newentry->field_0x1 = type;
		newentry->next = NULL;
		newentry->callback = callback;
		strcpy(newentry->filename, filename);
		afp->firstavailable = temp;
		if (!afp->firstavailable) {
			afp->lastavailable = NULL;
		}
		if (afp->firstused) {
			afp->lastused->next = newentry;
		}
		else {
			afp->firstused = newentry;
		}
		afp->lastused = newentry;
		newentry->entry = DVDMgrOpen(filename, 2u, 0);
		DVDMgrReadAsync(newentry->entry, smart->address, (s32)test, 0, dvdReadDoneCallBack);
		return 0;
	}
}

//TODO: enum of all archive types
void fileSetCurrentArchiveType(s32 type) {
	afp->mCurrentArchiveType = type;
}