#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include <math.h>
#include <string.h> //memcpy
#include <stdlib.h>
#include "mariost.h"

extern BOOL __mapdrv_make_dl; //mapdrv.c
extern marioStruct* gp;

void qqsort(void* array, u32 num_elements, u32 element_size, s32(*compare)(void*, void*));
void fsort(void* array, u32 num_elements);

//.bss
u8 tmp1[0x100];
void* tmp0[0xC00]; //size 0x3000

//.sbss
u16 token;
void** p; //TODO: re-type these
void** tail;
u32 gt;
u32 hi;
u32 lo;
s32(*comp)(void* entry1, void* entry2);


#ifdef __MWERKS__
asm void* memcpy_as4(void* dst, void* src, u32 nulenm) {
	nofralloc
	addi    r3, r3, -4
	srwi    r5, r5, 2
	addi    r4, r4, -4
	mtctr   r5
	@0:
	lwzu    r11, 4(r4)
	stwu    r11, 4(r3)
	bdnz    @0
	blr
}
#endif

/* unfinished, unused, probably don't care enough to finish it
#define read_u8(ptr) (*(u8*)(ptr))
#define read_u8_off(ptr, offset) (*(u8*)((u32)ptr + offset))

u32 LZ77Decode(u8* input, u8* output) {
	u8* originalPtr;
	u32 size, ret;
	u8 bitsLeft, byte1, byte2, length;
	u16 readFlag, rollBack;
	s8 bitFlag; //signed

	originalPtr = output;
	if (*input != 0x10) return 0; //not LZ10 compressed
	size = (u32)(input[3] << 16) + (u32)(input[2] << 8) + (u32)input[1];
	//size = (read_u8_off(input, 3) << 16) + (read_u8_off(input, 2) << 8) + read_u8_off(input, 1);
	input += 4;
	while (1) {
		ret = ((u32)output - (u32)originalPtr);
		if (size == ret) break;
		if (!bitsLeft) {
			bitsLeft = 8;
			bitFlag = (s8)*input++;
		}
		if (bitFlag < 0) { //highest bit/sign bit == 1, RLE
			byte1 = *input++;
			byte2 = *input++;
			length = (byte1 >> 4) + 3;
			rollBack = (((byte1 & 0xF) << 16) + byte2) + 1;
		}
		else { //copy one byte from input
			*output++ = *input++;
		}
		bitsLeft--;
		bitFlag *= 2; //multiply by two to get next sign bit
	}
}*/

//TODO: match retail better?
void mtxGetRotationElement(Mtx* mtx1, Mtx* mtx2, char oldaxis, char newaxis) {
	Vec vecX, vecY, vecZ;
	switch (oldaxis) {
		case 'x':
		case 'X':
			vecX.x = (*mtx1)[0][0];
			vecX.y = (*mtx1)[1][0];
			vecX.z = (*mtx1)[2][0];
			VECNormalize(&vecX, &vecX);
			switch (newaxis) {
				case 'y':
				case 'Y':
					vecY.x = (*mtx1)[0][1];
					vecY.y = (*mtx1)[1][1];
					vecY.z = (*mtx1)[2][1];
					VECNormalize(&vecY, &vecY);
					VECCrossProduct(&vecX, &vecY, &vecZ);
					VECCrossProduct(&vecZ, &vecX, &vecY);
					break;
				case 'z':
				case 'Z':
					vecZ.x = (*mtx1)[0][2];
					vecZ.y = (*mtx1)[1][2];
					vecZ.z = (*mtx1)[2][2];
					VECNormalize(&vecZ, &vecZ);
					VECCrossProduct(&vecZ, &vecX, &vecY);
					VECCrossProduct(&vecX, &vecY, &vecZ);
					break;
			}
			break;
		case 'y':
		case 'Y':
			vecY.x = (*mtx1)[0][1];
			vecY.y = (*mtx1)[1][1];
			vecY.z = (*mtx1)[2][1];
			VECNormalize(&vecY, &vecY);
			switch (newaxis) {
				case 'x':
				case 'X':
					vecX.x = (*mtx1)[0][0];
					vecX.y = (*mtx1)[1][0];
					vecX.z = (*mtx1)[2][0];
					VECNormalize(&vecX, &vecX);
					VECCrossProduct(&vecX, &vecY, &vecZ);
					VECCrossProduct(&vecY, &vecZ, &vecX);
					break;
				case 'z':
				case 'Z':
					vecZ.x = (*mtx1)[0][2];
					vecZ.y = (*mtx1)[1][2];
					vecZ.z = (*mtx1)[2][2];
					VECNormalize(&vecZ, &vecZ);
					VECCrossProduct(&vecY, &vecZ, &vecX);
					VECCrossProduct(&vecX, &vecY, &vecZ);
					break;

			}
			break;
		case 'z':
		case 'Z':
			vecZ.x = (*mtx1)[0][2];
			vecZ.y = (*mtx1)[1][2];
			vecZ.z = (*mtx1)[2][3];
			VECNormalize(&vecZ, &vecZ);
			switch (newaxis) {
				case 'x':
				case 'X':
					vecX.x = (*mtx1)[0][0];
					vecX.y = (*mtx1)[1][0];
					vecX.z = (*mtx1)[2][0];
					VECNormalize(&vecX, &vecX);
					VECCrossProduct(&vecZ, &vecX, &vecY);
					VECCrossProduct(&vecY, &vecZ, &vecX);
					break;
				case 'y':
				case 'Y':
					vecY.x = (*mtx1)[0][1];
					vecY.y = (*mtx1)[1][1];
					vecY.z = (*mtx1)[2][1];
					VECNormalize(&vecY, &vecY);
					VECCrossProduct(&vecY, &vecZ, &vecX);
					VECCrossProduct(&vecZ, &vecX, &vecY);
					break;
			}
			break;
	}
	(*mtx2)[0][0] = vecX.x;
	(*mtx2)[1][0] = vecX.y;
	(*mtx2)[2][0] = vecX.z;
	(*mtx2)[0][1] = vecY.x;
	(*mtx2)[1][1] = vecY.y;
	(*mtx2)[2][1] = vecY.z;
	(*mtx2)[0][2] = vecZ.x;
	(*mtx2)[1][2] = vecZ.y;
	(*mtx2)[2][2] = vecZ.z;
	(*mtx2)[0][3] = 0.0f;
	(*mtx2)[1][3] = 0.0f;
	(*mtx2)[2][3] = 0.0f;
}




u8 padGetRumbleStatus(u32 chan) {
	return gp->mRumbleStatus[chan];
}

void padRumbleHardOff(u32 chan) {
	gp->mRumbleStatus[chan] = PAD_MOTOR_STOP_HARD;
}

void padRumbleOff(u32 chan) {
	gp->mRumbleStatus[chan] = PAD_MOTOR_STOP;
}

void padRumbleOn(u32 chan) {
	gp->mRumbleStatus[chan] = PAD_MOTOR_RUMBLE;
}

s8 keyGetSubStickY(u32 chan) {
	return gp->mSubStickY[chan];
}

//unused on retail
s8 keyGetSubStickX(u32 chan) {
	return gp->mSubStickX[chan];
}

s8 keyGetStickY(u32 chan) {
	return gp->mStickY[chan];
}

s8 keyGetStickX(u32 chan) {
	return gp->mStickX[chan];
}

u32 keyGetButtonTrg(u32 chan) {
	return gp->mButtonTrg[chan];
}

u32 keyGetDirTrg(u32 chan) {
	return gp->mDirTrg[chan];
}

u32 keyGetButtonRep(u32 chan) {
	return gp->mButtonRep[chan];
}

u32 keyGetDirRep(u32 chan) {
	return gp->mDirRep[chan];
}

u32 keyGetButton(u32 chan) {
	return gp->mButton[chan];
}

u32 keyGetDir(u32 chan) {
	return gp->mDir[chan];
}

void makeKey(void) {
	int i;

	DEMOPadRead();
	//Directions
	for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		gp->mDirTrg[i] = PADButtonDown(gp->mDir[i], DemoPad[i].dirs);
		gp->mDirRep[i] = gp->mDirTrg[i];
		if (DemoPad[i].dirs && (DemoPad[i].dirs != gp->mDir[i])) {
			gp->mDirRepeatDelay[i]--;
			if (!gp->mDirRepeatDelay[i]) {
				gp->mDirRep[i] = DemoPad[i].dirs;
				gp->mDirRepeatDelay[i] = 6;
			}
		}
		else {
			gp->mDirRepeatDelay[i] = 24;
		}
		gp->mDir[i] = DemoPad[i].dirs;
	}
	//Buttons
	for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		gp->mButtonTrg[i] = PADButtonDown(gp->mButton[i], DemoPad[i].pst.button);
		gp->mButtonRep[i] = gp->mButtonTrg[i];
		if (DemoPad[i].pst.button && (DemoPad[i].pst.button != gp->mButton[i])) {
			gp->mButtonRepeatDelay[i]--;
			if (!gp->mButtonRepeatDelay[i]) {
				gp->mButtonRep[i] = DemoPad[i].pst.button;
				gp->mButtonRepeatDelay[i] = 6;
			}
		}
		else {
			gp->mButtonRepeatDelay[i] = 24;
		}
		gp->mButton[i] = DemoPad[i].pst.button;
		gp->mButtonUp[i] = DemoPad[i].buttonUp;
	}

	//unrolled to 2 loops with double writes
	for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		gp->mStickX[i] = DemoPad[i].pst.stickX;
		gp->mStickY[i] = DemoPad[i].pst.stickY;
		gp->mSubStickX[i] = DemoPad[i].pst.substickX;
		gp->mSubStickY[i] = DemoPad[i].pst.substickY;
		gp->mTriggerL[i] = DemoPad[i].pst.triggerLeft;
		gp->mTriggerR[i] = DemoPad[i].pst.triggerRight;
	}

	if (gp->field_0x1294) {
		for (i = 0; i < 4; i++) {
			if (gp->field_0x1310[i]) {
				PADStopMotorHard(i);
			}
			else {
				if (gp->field_0x13D4[i] != gp->mRumbleStatus[i]) {
					switch (gp->mRumbleStatus[i]) {
						case PAD_MOTOR_STOP:
							PADStopMotor(i);
							break;
						case PAD_MOTOR_RUMBLE:
							PADStartMotor(i);
							break;
						case PAD_MOTOR_STOP_HARD:
							PADStopMotorHard(i);
							break;
						//default: fall through
					}
					gp->field_0x13D4[i] = gp->mRumbleStatus[i];
				}
			}
		}
	}
	gp->field_0x1324 = 1;
}

void qqsort(void* array, u32 num_elements, u32 element_size, s32 (*compare)(void*, void*)) {
	u32 array_ptr, tmp0_entry, array_entry;
	int i;

	comp = compare;
	if (num_elements > 1) { //actually have to do work
		array_ptr = (u32)array;
		//compiled is more efficient, does 8 words and then singular words
		for (i = 0; i < num_elements; i++) {
			tmp0[i] = (void*)array_ptr; //store all ptrs
			array_ptr += element_size;
		}

		fsort(tmp0, num_elements);
		array_ptr = (u32)array;
		for (i = 0; i < num_elements; i++) {
			if (tmp0[i] != NULL) {
				tmp0_entry = (u32)&tmp0[i];
				if (tmp0[i] != (void*)array_ptr) {
					array_entry = array_ptr;
					memcpy(&tmp1, (void*)array_ptr, element_size); //hold old entry
					do {
						memcpy((void*)array_entry, *(void**)tmp0_entry, element_size); //copy sorted entry into its place
						array_entry = *(u32*)tmp0_entry;
						*(void**)tmp0_entry = NULL;
						tmp0_entry = (u32)&tmp0[(array_entry - (u32)array) / element_size];
					} while (*(u32*)tmp0_entry != array_ptr);
					memcpy((void*)array_entry, &tmp1, element_size); //copy old entry back so we don't lose it in swaps
					*(void**)tmp0_entry = NULL;
				}
			}
			array_ptr += element_size;
		}
	}
}

void fsort(void* array, u32 num_elements) {
	/* TODO: finish or embed, looks like it inlined itself
	
	tail = &array + ((num_elements - 1) * 4);
	//tail = (u32)array + ((num_elements - 1) * 4); //TODO: re-type tail
	if (num_elements == 2) {
		if (comp(array, *tail) > 0) {
			*p = array;
			array = *tail;
			*tail = *p;
		}
		return;
	}
	if (num_elements == 4) {

	}*/
}




f32 sysMsec2FrameFloat(f32 msec) {
	return (msec * 60.0f) / 1000.0f;
}

s32 sysMsec2Frame(s32 msec) {
	return (msec * 60) / 1000;
}

f32 sysFrame2SecFloat(f32 frame) {
	return frame / 60.0f;
}

u16 sysGetToken(void) {
	return token++;
}

void sysWaitDrawSync(void) {
	u16 this_token;
	OSTick tick;

	this_token = token++;
	tick = OSGetTick();
	if (!__mapdrv_make_dl) {
		GXSetDrawSync(this_token);
		while ((this_token != GXReadDrawSync()) && (OSTicksToMilliseconds(OSGetTick() - tick) <= 100)) {
			;
		}
	}
}


int irand(int scalar) {
	int ret;

	if (scalar == 0) {
		ret = 0;
	}
	else {
		ret = rand();
		ret -= (ret / scalar) * scalar;
	}
	return ret;
}





double compAngle(double angle1, double angle2) {
	if (fabs((angle2 - angle1)) >= 180.0) {
		if (angle2 >= angle1) {
			angle2 -= 360.0;
		} else {
			angle2 += 360.0;
		}
	}
	return (angle2 - angle1);
}

double distABf(double x1, double y1, double x2, double y2) { //TODO: decompile properly? see below with checks
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))); //sqrt((x2-x1)^2+(y2-y1)^2);
}

/*
float distABf(float x1, float y1, float x2, float y2) { //TODO: cleanup?
	double result, v5, v6, v7;

	result = (((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))); //sqrt((x2-x1)^2 + (y2-y1)^2)
	if (result <= 0.0) {
		if (result < 0.0) {
			result = NAN;
		}
		if ((result == INFINITY) && (((u32)result & 0x7FFFFF) != 0)) { //some mantissa thing?
			result = NAN;
		}
	}
	else {
		v5 = sqrt(result);
		v6 = 0.5 * v5 * -(result * (v5 * v5) - 3.0);
		v7 = 0.5 * v6 * -(result * (v6 * v6) - 3.0);
		result = (result * (0.5 * v7 * -(result * (v7 * v7) - 3.0)));
	}
	return result;
}
*/

double reviseAngle(double angle) {
	double result;
	
	result = fmod(angle, 360.0);
	if (result < 0.0) {
		result += 360.0;
		if (result >= 360.0) {
			result = 0.0;
		}
	}
	return result;
}

char* getMarioStDvdRoot(void) {
	return ".";
}