/* "swdrv" - Saved Work Driver (for evt)
 * Status: Complete, not 1:1 (would have to re-type a bunch of stuff in
 * evtmgr/_cmd and here to be signed but that's dumb)
 * 
 * Function: Helper driver for accessing the "saved work" data for evt
 * scripts
 */
#include "drv/swdrv.h"
#include "mariost.h"
#include <string.h>

extern marioStruct* gp;

u32 _swByteGet(u32 index) {
	return gp->mLSW[index];
}

void _swByteSet(u32 index, u8 value) {
	gp->mLSW[index] = value;
}

void _swClear(u32 index) { //clear specific bit
	gp->mLSWF[index / 8] &= ~(1 << (index % 8));
}

u8 _swGet(u32 index) { //get specific bit
	return (u8)((gp->mLSWF[index / 8] & (1 << (index % 8))) >> index % 8);
}

void _swSet(u32 index) { //set specific bit
	gp->mLSWF[index / 8] |= (1 << (index % 8));
}

u32 swByteGet(u32 index) {
	if (index == 0) {
		return gp->mGSW0;
	}
	else {
		return gp->mGSW[index];
	}
}

void swByteSet(u32 index, u32 value) {
	if (index == 0) {
		gp->mGSW0 = value;
	}
	else {
		gp->mGSW[index] = (u8)value;
	}
}

void swClear(u32 index) { //clear specific bit
	gp->mGSFW[index / 8] &= ~(1 << (index % 8));
}

u8 swGet(u32 index) { //get specific bit
	return (u8)((gp->mGSFW[index / 8] & (1 << (index % 8))) >> index % 8);
}

void swSet(u32 index) { //set specific bit
	gp->mGSFW[index / 8] |= (1 << (index % 8));
}

void swReInit(void) {
	memset(gp->mLSWF, 0, sizeof(gp->mLSWF));
	memset(gp->mLSW, 0, sizeof(gp->mLSW));
}

void swInit(void) {
	memset(gp->mGSFW, 0, sizeof(gp->mGSFW));
	memset(gp->mGSW, 0, sizeof(gp->mGSW));
	memset(gp->mLSWF, 0, sizeof(gp->mLSWF));
	memset(gp->mLSW, 0, sizeof(gp->mLSW));
	gp->mGSW0 = 0;
}