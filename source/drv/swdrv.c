#include "drv/swdrv.h"

extern marioSt_gp* gp;

u32 _swByteGet(u32 index) {
	return gp->_swByteData[index];
}

void _swByteSet(u32 index, u32 value) {
	gp->_swByteData[index] = value;
}

void _swClear(u32 index) { //clear specific bit
	gp->_swBitData[index / 32] &= ~(1 << (index % 32));
}

u32 _swGet(u32 index) { //get specific bit
	return (gp->_swBitData[index / 32] & (1 << (index % 32))) >> index % 32;
}

void _swSet(u32 index) { //set specific bit
	gp->_swBitData[index / 32] |= (1 << (index % 32));
}

u32 swByteGet(u32 index) {
	if (index == 0)
		return gp->field_0x174;
	return gp->swByteData[index];
}

void swByteSet(u32 index, u32 value) {
	if (index == 0)
		gp->field_0x174 = value;
	else
		gp->swByteData[index] = value; //TODO: & 0xFF?
}

void swClear(u32 index) { //clear specific bit
	gp->swBitData[index / 32] &= ~(1 << (index % 32));
}

u32 swGet(u32 index) { //get specific bit
	return (gp->swBitData[index / 32] & (1 << (index % 32))) >> index % 32;
}

void swSet(u32 index) { //set specific bit
	gp->swBitData[index / 32] |= (1 << (index % 32));
}

void swReInit(void) {
	memset(gp->_swBitData, 0, sizeof(gp->_swBitData));
	memset(gp->_swByteData, 0, sizeof(gp->_swByteData));
}

void swInit(void) {
	memset(gp->swBitData, 0, sizeof(gp->swBitData));
	memset(gp->swByteData, 0, sizeof(gp->swByteData));
	memset(gp->_swBitData, 0, sizeof(gp->_swBitData));
	memset(gp->_swByteData, 0, sizeof(gp->_swByteData));
	gp->field_0x174 = 0;
}