#include "drv/msgdrv.h"
#include "drv/animdrv.h"
#include "mgr/dvdmgr.h"
#include "mariost.h"
#include "memory.h"
#include "system.h"
#include <string.h>
#include <stdlib.h>

extern marioStruct* gp;
extern int sprintf(char* str, const char* format, ...);

typedef struct MsgLookupEntry {
	u32 startaddr; //0x0
	u32 endaddr; //0x4
} MsgLookupEntry;

//.sbss
u16 active; //TODO: re-type
void* msgSortDvdData; //TODO: re-type
BOOL msgSEflag; //msgSEInit and related functions are unused
MessageWork msgWork;

//.sdata
MessageWork* msgw = &msgWork;
const char* ErrMessage[2] = {
	"<col ff0000ff>メッセージがありません</col>\n<key>",
	"<col ff0000ff>No messages.<col>\n<key>"
};
const char* paperPoseTbl[6] = {
	"PM_Y_1B", "PM_Y_2B", "PM_F_1B",
	"PM_F_3B", "PM_C_1B", "PM_C_2B"
};
const char* language_dir[2] = {"msg", "emsg"};





//local prototypes
int msg_compare(const void* a, const void* b);
void msgDispKeyWait_render(s32 wXluStage);

void msgInit(void) {
	msgw->poseId = animPoseEntry("hoshi_2", 2);
	animPoseSetAnim(msgw->poseId, "HOS_C_1", TRUE);
	animPoseSetMaterialFlagOn(msgw->poseId, 0x1800);
	animPoseSetMaterialFlagOff(msgw->poseId, 0x40);
	animPoseSetGXFunc(msgw->poseId, msgDispKeyWait_render, 0);
	msgw->entries[0].address = NULL;
	msgw->entries[0].lookup = NULL;
	msgw->entries[1].address = NULL;
	msgw->entries[1].lookup = NULL;
	msgLoad("global", 1);
}

void msgLoad(const char* filename, s32 index) {
	MessageEntry* entry;
	DVDEntry* dvd;
	char* msgptr;
	s32 msgcount;
	char filepath[64];
	char dvdpath[128]; //TODO: double check size
	int i;
	u32 r27, r30;
	MsgLookupEntry* lookup;

	entry = &msgw->entries[index];
	sprintf(filepath, "./%s/%s.txt", language_dir[gp->mLanguage], filename);
	
	if (entry->address) {
		if (index == 1) {
			__memFree(HEAP_DEFAULT, entry->address);
		}
		entry->address = NULL;
	}
	entry->length = 0;

	if (!entry->address) {
		sprintf(dvdpath, "%s/%s", getMarioStDvdRoot(), filepath);
		dvd = DVDMgrOpen(dvdpath, 2, 0);
		if (dvd) {
			entry->length = DVDMgrGetLength(dvd);
			switch (index) {
				case 0:
					entry->address = _mapAlloc(OSRoundUp32B(entry->length));
					break;
				case 1:
					entry->address = __memAlloc(HEAP_DEFAULT, OSRoundUp32B(entry->length));
					break;
			}
			DVDMgrRead(dvd, entry->address, OSRoundUp32B(entry->length), 0);
			DVDMgrClose(dvd);
		}
	}

	if (entry->address) {
		msgptr = entry->address;
		msgcount = 0;
		while (*msgptr) {
			msgcount++;
			msgptr += strlen(msgptr) + 1;
		}
		entry->count = (u32)(msgcount / 2); //tag, message
	}
	else {
		entry->length = 0;
		entry->count = 0;
	}

	if (entry->count) {
		r27 = 0;
		entry->lookup = smartAlloc(entry->count * sizeof(MsgLookupEntry), 0);
		r30 = (u32)entry->address;
		lookup = entry->lookup->address;
		for (i = 0; i < entry->count; i++) {
			lookup->startaddr = r27;
			r27 += strlen((const char*)r30) + 1;
			r30 += strlen((const char*)r30) + 1;
			lookup->endaddr = r27;
			r27 += strlen((const char*)r30) + 1;
			r30 += strlen((const char*)r30) + 1;
			lookup++;
		}
		msgSortDvdData = entry->address;
		qsort(entry->lookup->address, entry->count, sizeof(MsgLookupEntry), msg_compare);
	}
	else {
		entry->lookup = NULL;
	}
}

//s32 msgMain(void* somearray)
//BOOL msgDisp(unused r3, r4, f32 f1, f32 f2)

void msgDispKeyWait_render(s32 wXluStage) {

}

//void msgAnalize(void* sameasmsgmain)

MessageWork* msgGetWork(void) {
	return &msgWork;
}

int msg_compare(const void* a, const void* b) {
	return 0;
}

const char* msgSearch(const char* tag) { //1:1
	MessageEntry* entry;
	MsgLookupEntry* lookup;
	smartEntry* smart;
	u32 i, max, j, index;
	s32 result;

	for (i = 0; i < 2; i++) { //each entry
		entry = &msgw->entries[i];
		smart = entry->lookup;
		if (smart) {
			lookup = smart->address;
			j = 0;
			max = entry->count;
			while (max >= j) {
				index = (u32)((s32)(j + max) / 2); //tag, message
				if (index >= entry->count) {
					break;
				}
				result = strcmp(tag, (const char*)((u32)entry->address + lookup[index].startaddr));
				if (!result) {
					return (const char*)((u32)entry->address + lookup[index].endaddr);
				}
				if (result < 0) {
					max = index - 1;
				}
				else {
					j = index + 1;
				}
			}
		}
	}
	return ErrMessage[gp->mLanguage];
}

BOOL _ismbblead(u32 a1) {
	u8 check = (u8)a1;
	if ((check >= 0x81 && check <= 0x9F) || (check >= 0xE0 && check <= 0xFC)) {
		return TRUE;
	}
	return FALSE;
}