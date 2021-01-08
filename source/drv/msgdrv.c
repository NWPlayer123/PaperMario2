#include "drv/msgdrv.h"

/*
TODO: deal with this old file


extern marioSt_gp* gp;

//.bss
u16 active; //TODO: retype
void* msgSortDvdData;
BOOL msgSEflag;
msgWork_gp msgWork;

//.sdata
msgWork_gp* msgw = &msgWork;
char* ErrMessage[2] = {
	"<col ff0000ff>メッセージがありません</col>\n<key>",
	"<col ff0000ff>No messages.<col>\n<key>",
};

char* paperPoseTbl[6] = {
	"PM_Y_1B",
	"PM_Y_2B",
	"PM_F_1B",
	"PM_F_3B",
	"PM_C_1B",
	"PM_C_2B",
};

char* language_dir[2] = {
	"msg",
	"emsg",
};

//TODO: finish
char* msgSearch(char* tag) {
	int i, r24, r25, r26;
	u32* r28;
	for (i = 0; i < 2; i++) {
		msgWorkStruct* somestruct = &msgw->somearray[i];
		r28 = *somestruct->lookup;
		if (somestruct->lookup != 0) {
			r25 = 0;
			r26 = somestruct->count;
			while (r26 >= r25) {
				r24 = (r25 + r26) / 2;
				if (r24 >= somestruct->count) break; //TODO: check fall

			}
		}
	}
}

void msgLoad(char* filename, int index) {
	char filepath[64]; //0x8-0x48
	char dvdpath[132]; //0x48-0xCC aka stack frame, probably wrong
	char *address, *offset;
	u32 numStrings;
	int i;

	msgWorkStruct* msgEntry;
	dvdq_entry* dvdEntry;
	sprintf(filepath, "./%s/%s.txt", language_dir[gp->isJP], filename);
	msgEntry = &msgw->somearray[index];
	if (msgEntry->address != NULL) {
		if (index == 1) //alloctype?
			__memFree(0);
		msgEntry->address = NULL;
	}
	msgEntry->length = 0;
	if (msgEntry->address == NULL) { //else skip dvd shit
		sprintf(dvdpath, "%s/%s", getMarioStDvdRoot(), filepath);
		dvdEntry = DVDMgrOpen(dvdpath, 2, 0);
		if (dvdEntry != NULL) {
			msgEntry->length = DVDMgrGetLength(dvdEntry);
			switch (index) {
				case 0:
					msgEntry->address = _mapAlloc(OSRoundUp32B(msgEntry->length));
					break;
				case 1:
					msgEntry->address = __memAlloc(0, OSRoundUp32B(msgEntry->length));
					break;
			}
			DVDMgrRead(dvdEntry, msgEntry->address, OSRoundUp32B(msgEntry->length), 0);
			DVDMgrClose(dvdEntry);
		}
	}
	if (msgEntry->address == NULL) {
		msgEntry->length = 0;
		msgEntry->count = 0;
	} else {
		 //parse all strings
		address = msgEntry->address;
		while (*address != '\0') {
			address += strlen(address) + 1; //null byte
			numStrings++;
		}
		msgEntry->count = numStrings / 2; //TODO: check which div
	}
	if (msgEntry->lookup != NULL) {
		smartFree(msgEntry->lookup);
	}
	if (msgEntry->count == 0) {
		msgEntry->lookup = NULL;
	} else {
		//+12 is offset table I think
		msgEntry->lookup = smartAlloc(msgEntry->count * 8, 0);
		offset = 0; //TODO: check
		address = msgEntry->address;
		for (i = 0; i < msgEntry->count; i++) { //TODO: while loop?
			//we divided by two for [tag+data], now read twice
			msgEntry->lookup[(i * 2)] = offset;
			offset += strlen(address) + 1;
			address += strlen(address) + 1;
			msgEntry->lookup[(i * 2) + 1] = offset;
			offset += strlen(address) + 1;
			address += strlen(address) + 1;
		}
	}
	msgSortDvdData = msgEntry->address;
	qsort(*msgEntry->lookup, msgEntry->count, 8, msg_compare);
}

void msgInit(void) {
	msgw->animIndex = animPoseEntry("hoshi_2", 2);
	animPoseSetAnim(msgw->animIndex, "HOS_C_1", 1);
	animPoseSetMaterialFlagOn(msgw->animIndex, 0x1800);
	animPoseSetMaterialFlagOff(msgw->animIndex, 0x40);
	animPoseSetGXFunc(msgw->animIndex, msgDispKeyWait_render, 0);
	msgw->somearray[0].address = NULL;
	msgw->somearray[0].lookup = 0;
	msgw->somearray[1].address = NULL;
	msgw->somearray[1].lookup = 0;
	msgLoad("global", 1);
}

*/