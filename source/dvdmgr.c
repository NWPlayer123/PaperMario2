#include "dvdmgr.h"

//.sbss
void* _callback; //duplicate, check
dvdq_entry (*dvdq)[0x100]; //dvdq_entry[0x100]* = 0x9800

//forward declarations
void DVDMgrSetupCallback(void* callback);
u32 DVDMgrGetLength(dvdq_entry* entry);
void DVDMgrClose(dvdq_entry* entry);
void DVDMgrReadAsync(dvdq_entry* entry, u32 r4, u32 r5, u32 r6, void* callback);
void DVDMgrRead(dvdq_entry* entry, u32 r4, u32 r5, u32 r6);
dvdq_entry* DVDMgrOpen(char* path, u8 r4, u16 r5);


void DVDMgrSetupCallback(void* callback) {
	_callback = callback;
}

u32 DVDMgrGetLength(dvdq_entry* entry) {
	return entry->fileInfo.length; //0x74
}

void DVDMgrClose(dvdq_entry* entry) {
	entry->status |= 8;
}

void DVDMgrReadAsync(dvdq_entry* entry, u32 r4, u32 r5, u32 r6, void* callback) {
    entry->field_0x7C = r4;
    entry->field_0x80 = r5;
    entry->field_0x84 = r6;
    entry->status |= 1;
    entry->status &= 0xFFFD;
    entry->callback = callback;
    entry->field_0x88 = 0;
}

void DVDMgrRead(dvdq_entry* entry, u32 r4, u32 r5, u32 r6) {
    entry->field_0x7C = r4;
    entry->field_0x80 = r5;
    entry->field_0x84 = r6;
    entry->status |= 1;
    entry->status &= 0xFFFD;
    entry->callback = NULL;
    entry->field_0x88 = 0;
    while ((entry->status & 2) == 0) {
        OSYieldThread();
    }
}

dvdq_entry* DVDMgrOpen(char* path, u8 r4, u16 r5) {
    int i;
    if (DVDConvertPathToEntrynum(path) == -1)
        return NULL;
    for (i = 0; i < 0x100; i++) {
        if ((dvdq[i]->status & 0x8000) == 0)
            break; //found a hit
    }
    if (i == 0x100) //didn't find a hit
        return NULL;
    memset(dvdq[i], 0, sizeof(dvdq_entry));
    strcpy(dvdq[i]->fileName, path);
    dvdq[i]->status = 0;
    dvdq[i]->field_0x92 = r4; //cast to u8 or smth
    dvdq[i]->field_0x94 = r5;
    dvdq[i]->field_0x7C = 0;
    dvdq[i]->field_0x80 = 0;
    dvdq[i]->field_0x84 = 0;
    dvdq[i]->field_0x88 = 0;
    dvdq[i]->status |= 0x8000;
    if (DVDOpen(dvdq[i]->fileName, &dvdq[i]->fileInfo) != 0) {
        memset(dvdq[i], 0, sizeof(dvdq_entry));
        return NULL;
    }
    return dvdq[i];
}