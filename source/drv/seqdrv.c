/* "seqdrv" - Sequence Driver
 * Status: Mostly complete, need function pointers in seq_data and
 * proper names for seqdrv_work
 * 
 * Function: used for swapping between game states (logo, game, battle, demo/e3)
 */
#include "drv/seqdrv.h"
#include "system.h"
#include <string.h> //memset prototype

//.data
extern void seq_e3Init(seqdrv_work* work);
extern void seq_e3Main(seqdrv_work* work);
extern void seq_e3Exit(seqdrv_work* work);

void(*seq_data[8][3])(seqdrv_work*) = {
	{NULL, NULL, NULL},
	{NULL, NULL, NULL},
	{NULL, NULL, NULL},
	{NULL, NULL, NULL},
	{NULL, NULL, NULL},
	{NULL, NULL, NULL},
	{NULL, NULL, NULL},
	{seq_e3Init, seq_e3Main, seq_e3Exit},
};

//.sdata
s32 now_seq = -1;
s32 next_seq = -1;
s32 prev_seq = -1;


//.sbss
s32 next_p0;
s32 next_p1;
seqdrv_work seqWork;

BOOL seqCheckSeq(void) {
	return ((next_seq - now_seq) | (now_seq - next_seq)) >> 31; //???
}

s32 seqGetNextSeq(void) {
	return next_seq;
}

s32 seqGetPrevSeq(void) {
	return prev_seq;
}

s32 seqGetSeq(void) {
	return now_seq;
}

void seqSetSeq(s32 seq, s32 p0, s32 p1) {
	next_seq = seq;
	next_p0 = p0;
	next_p1 = p1;
}

void seqMain(void) {
	if (next_seq != now_seq) {
		if (now_seq != -1) {
			sysWaitDrawSync();
			(*seq_data[next_seq][2])(&seqWork); //call Exit function
		}
		prev_seq = now_seq; //move back
		memset(&seqWork, 0, sizeof(seqWork));
		now_seq = next_seq;
		seqWork.field_0x0 = next_seq;
		seqWork.field_0x4 = 0;
		seqWork.field_0x8 = next_p0;
		seqWork.field_0xC = next_p1;
		(*seq_data[next_seq][0])(&seqWork); //call Init function
	}
	(*seq_data[now_seq][1])(&seqWork); //call Main function
}

void seqInit_MARIOSTORY(void) {
	memset(&seqWork, 0, sizeof(seqWork));
	now_seq = -1;
	next_seq = -1;
	prev_seq = -1;
}