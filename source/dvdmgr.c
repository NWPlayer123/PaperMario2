#include "dvdmgr.h"

//.bss
u8 stack[0x4000];
OSThread dvdmgr_thread;

//.sbss
BOOL dvdmgr_thread_on;
void (*_callback)(void); //TODO: re-type
dvdq_entry (*dvdq)[0x100]; //dvdq_entry[0x100]* = 0x9800

//forward declarations
void DVDMgrInit(void);
void DVDMgrDelete(void);
void DVDMgrMain(void);
dvdq_entry* DVDMgrOpen(char* path, u8 r4, u16 r5);
void DVDMgrRead(dvdq_entry* entry, void* address, u32 bytesLeft, u32 r6);
void DVDMgrReadAsync(dvdq_entry* entry, void* address, u32 bytesLeft, u32 r6, void* callback);
void DVDMgrClose(dvdq_entry* entry);
u32 DVDMgrGetLength(dvdq_entry* entry);
void DVDMgrSetupCallback(void* callback);


void DVDMgrSetupCallback(void* callback) {
	_callback = callback;
}

u32 DVDMgrGetLength(dvdq_entry* entry) {
	return entry->fileInfo.length; //0x74
}

void DVDMgrClose(dvdq_entry* entry) {
	entry->status |= 8;
}

void DVDMgrReadAsync(dvdq_entry* entry, void* address, u32 bytesLeft, u32 r6, void* callback) {
    entry->address = address;
    entry->bytesLeft = bytesLeft;
    entry->field_0x84 = r6;
    entry->status |= 1;
    entry->status &= 0xFFFD;
    entry->callback = callback;
    entry->bytesRead = 0;
}

void DVDMgrRead(dvdq_entry* entry, void* address, u32 bytesLeft, u32 r6) {
    entry->address = address;
    entry->bytesLeft = bytesLeft;
    entry->field_0x84 = r6;
    entry->status |= 1;
    entry->status &= 0xFFFD;
    entry->callback = NULL;
    entry->bytesRead = 0;
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
    dvdq[i]->address = 0;
    dvdq[i]->bytesLeft = 0;
    dvdq[i]->field_0x84 = 0;
    dvdq[i]->bytesRead = 0;
    dvdq[i]->status |= 0x8000;
    if (DVDOpen(dvdq[i]->fileName, &dvdq[i]->fileInfo) == 0) {
        memset(dvdq[i], 0, sizeof(dvdq_entry));
        return NULL;
    }
    return dvdq[i];
}

void DVDMgrMain(void) {
    u8 somearray[0x100]; //stack+8
    int i, count, err, dobreak; //TODO: u32?
    u32 length;
    dvdq_entry* entry;
    for (i = 0; i < 0x100; i++) {
        if (dvdq[i]->status & 0x8000) {
            somearray[i] = i;
            count++;
        }
    }
    qqsort(somearray, count, 1, compare);
    for (i = 0; i < count; i++) { //TODO: change to while loop?
        entry = dvdq[somearray[i]];
        if (!(entry->status & 0x80) && (entry->status & 1)) {
            length = entry->bytesLeft;
            if (entry->field_0x92 != 0) { //dochunks?
                if (length > 0x40000)
                    length = 0x40000;
            }
            err = DVDReadPrio(&entry->fileInfo, entry->address, length,
                               entry->field_0x84 + entry->bytesRead, 2);
            if (err == DVD_RESULT_CANCELED) {
                entry->status &= 0xFFFE;
                entry->status |= 2;
                if (entry->callback != NULL) {
                    entry->callback(err, entry->fileInfo);
                }
            }
            else if (err < 0) { //other error
                entry->status |= 0x20;
                entry->status |= 0x80;
            }
            else { //successfully read data, update struct info
                entry->bytesLeft -= length;
                entry->bytesRead += length;
                entry->address += length;
                if (entry->bytesLeft == 0) {
                    entry->status &= 0xFFFE;
                    entry->status |= 2;
                    if (entry->callback != NULL) {
                        entry->callback(err, entry->fileInfo);
                    }
                }
                dobreak = TRUE;
            }
        }
        if (entry->status & 8) {
            DVDClose(&entry->fileInfo);
            memset(entry, 0, sizeof(dvdq_entry));
        }
        if (dobreak) break;
    }
    if (_callback != NULL) {
        _callback();
    }
}

#ifdef __MWERKS__
void compare(void) {
    lbz     r3, 0(r3)
	lbz     r0, 0(r4)
	lwz     r4, dvdq@sda21(0)
	mulli   r3, r3, 0x98
	addi    r4, r4, 0x92
	mulli   r0, r0, 0x98
	lhzx    r3, r4, r3
	clrlwi  r3, r3, 24
	lhzx    r0, r4, r0
	clrlwi  r0, r0, 24
	cmplw   r3, r0
	ble     @0
	li      r3, 1
	blr
	@0:
	subfc   r0, r0, r3
	subfe   r3, r0, r0
	blr
}
#endif

void DVDMgrDelete(void) {
    if (dvdmgr_thread_on != FALSE) {
        OSCancelThread(&dvdmgr_thread);
        dvdmgr_thread_on = FALSE;
    }
}

void DVDMgrInit(void) {
    BOOL threadCreated;
    dvdq = __memAlloc(0, sizeof(dvdq_entry) * 0x100);
    memset(dvdq, 0, sizeof(dvdq_entry) * 0x100);
    sizeof(OSThread);
    threadCreated = OSCreateThread(&dvdmgr_thread, proc_main, 0, &stack + sizeof(stack), sizeof(stack), 16, OS_THREAD_ATTR_DETACH);
    if (!threadCreated) {
        while (1) {} //hang
    }
    dvdmgr_thread_on = TRUE;
    OSResumeThread(&dvdmgr_thread);
}

void* proc_main(void* val) {
    while (1) {
        DVDMgrMain();
        OSYieldThread();
    }
}