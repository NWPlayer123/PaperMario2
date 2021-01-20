#include "seq/seq_title.h"
#include "mariost.h"
#include "pmario_sound.h"
#include <string.h>

extern marioStruct* gp;

seq_title_work work2;
seq_title_work* wp2 = &work2;

void titleInit(seqdrv_work* work);
void titleMain(seqdrv_work* work);

void titleMain(seqdrv_work* work) {

}

void titleInit(seqdrv_work* work) {
	memset(wp2, 0, 0x34); //TODO: double check, might be bug or might be sizeof - 4
	
}

s32 getDebugMode(void) {
	return wp2->debug_mode;
}

void seq_titleMain(seqdrv_work* work) {
	titleMain(work);
}

void seq_titleExit(seqdrv_work* work) {
	fileFree(wp2->handle);
	if (seqGetNextSeq() != 6) { //TODO: enum
		psndStopAllFadeOut();
	}
}

void seq_titleInit(seqdrv_work* work) {
	titleInit(work);
	gp->mFlags &= 0xFFFFFFFD;
	gp->mFlags &= 0xFFFFFFBF;
	gp->mFlags &= 0xFFFFFFDF;
	gp->mFlags &= 0xFFFFEFFF;
}

//used for debugging, pointer to string
char* DbgBtlSel_GetMsgDataPtr(void) {
	return NULL;
}