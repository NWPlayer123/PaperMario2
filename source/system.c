#include "system.h"
#include <dolphin/gx.h>
#include <dolphin/pad.h>
#include <demo/DEMOPad.h>
#include <math.h>
#include <string.h> //memcpy
#include <stdlib.h>
#include "mario/mariost.h"

#pragma pool_data on

#define HALF_PI 1.57079632679f
#define PI 3.14159265359f
#define TWO_PI 6.28318530718f

extern BOOL __mapdrv_make_dl; //mapdrv.c
extern GlobalWork* gp;

void fsort(u32* array, u32 num_elements);

//.data
static f32 angleABTBL[] = {
	1.0f, 1.273187f, 1.27303f, 1.272768f, 1.272402f, 1.271932f, 1.271358f,
	1.270681f, 1.269902f, 1.269021f, 1.268038f, 1.266956f, 1.265774f, 1.2644939f,
	1.263116f, 1.2616431f, 1.260075f, 1.258413f, 1.256659f, 1.254815f, 1.2528811f,
	1.250859f, 1.248752f, 1.24656f, 1.244285f, 1.2419291f, 1.239494f, 1.236981f,
	1.234393f, 1.2317311f, 1.228997f, 1.226192f, 1.22332f, 1.220382f, 1.217379f,
	1.2143151f, 1.211189f, 1.208006f, 1.204766f, 1.201471f, 1.1981241f, 1.1947269f,
	1.191281f, 1.1877871f, 1.18425f, 1.180669f, 1.177047f, 1.173386f, 1.169687f,
	1.165952f, 1.162184f, 1.158384f, 1.1545531f, 1.1506931f, 1.146806f, 1.142893f,
	1.138957f, 1.134998f, 1.131018f, 1.127019f, 1.1230021f, 1.118969f, 1.11492f,
	1.110858f, 1.106783f, 1.102697f, 1.098601f, 1.094496f, 1.090384f, 1.086266f,
	1.082142f, 1.078014f, 1.0738831f, 1.06975f, 1.065616f, 1.061482f, 1.057348f,
	1.053216f, 1.049087f, 1.04496f, 1.040838f, 1.036721f, 1.0326101f, 1.028504f,
	1.024406f, 1.020316f, 1.016234f, 1.0121599f, 1.0080971f, 1.004043f, 1.0f
};

//.bss
u8 tmp1[0x100];
void* tmp0[0xC00]; //size 0x3000

//.sbss
u16 token;
u32 p; //temp variable for swapping between array and tail
u32* tail;
u32* gt;
u32* hi;
u32* lo;
s32(*comp)(const void* a, const void* b);






const char* getMarioStDvdRoot(void) {
	return ".";
}

f32 reviseAngle(f32 angle) {
	f32 result;
	f32 remainder = (f32)fmod(angle, 360.0);

	result = remainder;
	if (remainder != remainder) result = 0.0f;
	if (result < 0.0f) {
		result += 360.0f;
		if (result >= 360.0f) {
			result = 0.0f;
		}
	}
	return result;
}

//TODO: finish
f32 distABf(f32 x1, f32 y1, f32 x2, f32 y2) {
	/*f32 result;
	f64 v5;
	s32 v10, v8;
	f64 temp0 = 0.5;
	f64 temp1 = 3.0;
	f64 temp2 = 0.0;*/

	return (((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))); //sqrt((x2-x1)^2 + (y2-y1)^2)
	/*if (result > 0.0f) {
		v5 = __frsqrte(result);
		v5 = temp0 * v5 * -(result * (v5 * v5) - temp1);
		v5 = temp0 * v5 * -(result * (v5 * v5) - temp1);
		return (result * (temp0 * v5 * -(result * (v5 * v5) - temp1)));
	}
	else {
		if (result >= temp2) {
			v10 = (s32)result;
			if ((v10 & 0x7F800000) == 0x7F800000) {
				if ((v10 & 0x7FFFFF) != 0) {
					v8 = 1;
				}
				else {
					v8 = 2;
				}
			}
			else if ((v10 & 0x7F800000) >= 0x7F800000 || (v10 & 0x7F800000) != 0) {
				v8 = 4;
			}
			else if ((v10 & 0x7FFFFF) != 0) {
				v8 = 5;
			}
			else {
				v8 = 3;
			}
			if (v8 == 1) {
				result = 0x7FFFFFFF;
			}
		}
		else {
			result = 0x7FFFFFFF;
		}
	}
	return result;*/
}

f32 compAngle(f32 angle1, f32 angle2) {
	if (fabs((angle2 - angle1)) >= 180.0f) {
		if (angle2 >= angle1) {
			angle2 -= 360.0f;
		}
		else {
			angle2 += 360.0f;
		}
	}
	return (angle2 - angle1);
}

static f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2) {
	f32 xdiff, ydiff;
	f32 xabs, yabs;
	f32 v8;
	f32 v9;
	int v10;
	f32 v13;
	f32 v15;
	f32 test;

	xdiff = x2 - x1;
	ydiff = y2 - y1;
	xabs = __fabsf(xdiff);
	yabs = __fabsf(ydiff);
	if (xabs > yabs) {
		v8 = 45.0f * (yabs / xabs);
		v9 = 2.0f * v8;
		if (v9 >= 0.0f) {
			v10 = (int)(0.5 + v9);
		}
		else {
			v10 = -(int)(0.5 - v9);
		}
		test = v8 * angleABTBL[v10];
		if (xdiff >= 0.0f) {
			if (ydiff >= 0.0f) {
				v13 = 90.0f + test;
			}
			else {
				v13 = 90.0f - test;
			}
		}
		else {
			if (ydiff >= 0.0f) {
				v13 = 270.0f - test;
			}
			else {
				v13 = 270.0f + test;
			}
		}
	}
	else {
		if (yabs == 0.0f) {
			return 0.0f;
		}
		else {
			v15 = 45.0f * (xabs / yabs);
			v9 = 2.0f * v15;
			if (v9 >= 0.0f) {
				v10 = (int)(0.5 + v9);
			}
			else {
				v10 = -(int)(0.5 - v9);
			}
			test = v15 * angleABTBL[v10];
			if (ydiff >= 0.0f) {
				if (xdiff >= 0.0f) {
					v13 = 180.0f - test;
				}
				else {
					v13 = 180.0f + test;
				}
			}
			else {
				if (xdiff >= 0.0f) {
					v13 = 0.0f + test;
				}
				else {
					v13 = 360.0f - test;
				}
			}
		}
	}
	return v13;
}




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
	return gp->rumbleStatus[chan];
}

void padRumbleHardOff(u32 chan) {
	gp->rumbleStatus[chan] = PAD_MOTOR_STOP_HARD;
}

void padRumbleOff(u32 chan) {
	gp->rumbleStatus[chan] = PAD_MOTOR_STOP;
}

void padRumbleOn(u32 chan) {
	gp->rumbleStatus[chan] = PAD_MOTOR_RUMBLE;
}

s8 keyGetSubStickY(u32 chan) {
	return gp->substickY[chan];
}

//unused on retail
s8 keyGetSubStickX(u32 chan) {
	return gp->substickX[chan];
}

s8 keyGetStickY(u32 chan) {
	return gp->stickY[chan];
}

s8 keyGetStickX(u32 chan) {
	return gp->stickX[chan];
}

u32 keyGetButtonTrg(u32 chan) {
	return gp->buttonNew[chan];
}

u32 keyGetDirTrg(u32 chan) {
	return gp->dirsNew[chan];
}

u32 keyGetButtonRep(u32 chan) {
	return gp->buttonRepeat[chan];
}

u32 keyGetDirRep(u32 chan) {
	return gp->dirsRepeat[chan];
}

u32 keyGetButton(u32 chan) {
	return gp->button[chan];
}

u32 keyGetDir(u32 chan) {
	return gp->dirs[chan];
}

void makeKey(void) {
	int i;

	DEMOPadRead();
	//Directions
	for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		gp->dirsNew[i] = PADButtonDown(gp->dirs[i], DemoPad[i].dirs);
		gp->dirsRepeat[i] = gp->dirsNew[i];
		if (DemoPad[i].dirs && (DemoPad[i].dirs != gp->dirs[i])) {
			gp->dirsRepeatDelay[i]--;
			if (!gp->dirsRepeatDelay[i]) {
				gp->dirsRepeat[i] = DemoPad[i].dirs;
				gp->dirsRepeatDelay[i] = 6;
			}
		}
		else {
			gp->dirsRepeatDelay[i] = 24;
		}
		gp->dirs[i] = DemoPad[i].dirs;
	}
	//Buttons
	for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		gp->buttonNew[i] = PADButtonDown(gp->button[i], DemoPad[i].pst.button);
		gp->buttonRepeat[i] = gp->buttonNew[i];
		if (DemoPad[i].pst.button && (DemoPad[i].pst.button != gp->button[i])) {
			gp->buttonRepeatDelay[i]--;
			if (!gp->buttonRepeatDelay[i]) {
				gp->buttonRepeat[i] = DemoPad[i].pst.button;
				gp->buttonRepeatDelay[i] = 6;
			}
		}
		else {
			gp->buttonRepeatDelay[i] = 24;
		}
		gp->button[i] = DemoPad[i].pst.button;
		gp->buttonUp[i] = DemoPad[i].buttonUp;
	}

	//unrolled to 2 loops with double writes
	for (i = 0; i < PAD_MAX_CONTROLLERS; i++) {
		gp->stickX[i] = DemoPad[i].pst.stickX;
		gp->stickY[i] = DemoPad[i].pst.stickY;
		gp->substickX[i] = DemoPad[i].pst.substickX;
		gp->substickY[i] = DemoPad[i].pst.substickY;
		gp->triggerLeft[i] = DemoPad[i].pst.triggerLeft;
		gp->triggerRight[i] = DemoPad[i].pst.triggerRight;
	}

	if (gp->field_0x1294) {
		for (i = 0; i < 4; i++) {
			if (gp->field_0x1310[i]) {
				PADStopMotorHard(i);
			}
			else {
				if (gp->prevRumbleStatus[i] != gp->rumbleStatus[i]) {
					switch (gp->rumbleStatus[i]) {
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
					gp->prevRumbleStatus[i] = gp->rumbleStatus[i];
				}
			}
		}
	}
	gp->field_0x1324 = TRUE;
}

/*
void qqsort(void* array, u32 num_elements, u32 element_size, s32(*compare)(const void*, const void*)) {
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

		fsort((u32*)tmp0, num_elements);
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
}*/

#ifdef __MWERKS__
asm void qqsort(void* array, u32 num_elements, u32 element_size, s32(*compare)(const void*, const void*)) {
	nofralloc
	stwu    r1, -0x40(r1)
	mflr    r0
	lis     r7, tmp0@ha
	stw     r0, 0x44(r1)
	stmw    r21, 0x14(r1)
	mr      r30, r4
	lis     r4, tmp1@ha
	mr      r29, r3
	addi    r0, r4, tmp1@l
	cmplwi  r30, 1
	mr      r31, r5
	mr      r23, r0
	stw     r6, comp(r13)
	addi    r6, r7, tmp0@l
	mr      r24, r6
	ble     @8
	cmplwi  r30, 0
	mr      r5, r29
	mr      r4, r6
	mr      r3, r30
	ble     @3
	srwi.   r0, r3, 3
	mtctr   r0
	beq     @1
	@0:
	stw     r5, 0(r4)
	add     r5, r5, r31
	stw     r5, 4(r4)
	add     r5, r5, r31
	stw     r5, 8(r4)
	add     r5, r5, r31
	stw     r5, 0xC(r4)
	add     r5, r5, r31
	stw     r5, 0x10(r4)
	add     r5, r5, r31
	stw     r5, 0x14(r4)
	add     r5, r5, r31
	stw     r5, 0x18(r4)
	add     r5, r5, r31
	stw     r5, 0x1C(r4)
	add     r5, r5, r31
	addi    r4, r4, 0x20
	bdnz    @0
	andi.   r3, r3, 7
	beq     @3
	@1:
	mtctr   r3
	@2:
	stw     r5, 0(r4)
	add     r5, r5, r31
	addi    r4, r4, 4
	bdnz    @2
	@3:
	mr      r3, r24
	mr      r4, r30
	bl      fsort
	mr      r27, r29
	mr      r26, r24
	li      r25, 0
	b       @7
	@4:
	lwz     r0, 0(r26)
	cmplwi  r0, 0
	beq     @6
	cmplw   r0, r27
	mr      r22, r26
	beq     @6
	mr      r3, r23
	mr      r21, r27
	mr      r4, r27
	mr      r5, r31
	bl      memcpy
	li      r28, 0
	@5:
	lwz     r4, 0(r22)
	mr      r3, r21
	mr      r5, r31
	bl      memcpy
	lwz     r21, 0(r22)
	subf    r0, r29, r21
	stw     r28, 0(r22)
	divwu   r0, r0, r31
	slwi    r0, r0, 2
	add     r22, r24, r0
	lwz     r0, 0(r22)
	cmplw   r0, r27
	bne     @5
	mr      r3, r21
	mr      r4, r23
	mr      r5, r31
	bl      memcpy
	li      r0, 0
	stw     r0, 0(r22)
	@6:
	add     r27, r27, r31
	addi    r26, r26, 4
	@7:
	cmplw   r25, r30
	addi    r25, r25, 1
	blt     @4
	@8:
	lmw     r21, 0x14(r1)
	lwz     r0, 0x44(r1)
	mtlr    r0
	addi    r1, r1, 0x40
	blr
}

asm void fsort(u32* array, u32 num_elements) {
	nofralloc
	stwu    r1, -0x30(r1)
	mflr    r0
	stw     r0, 0x34(r1)
	stmw    r25, 0x14(r1)
	mr      r25, r3
	mr      r26, r4
	@0:
	addi    r0, r26, -1
	cmplwi  r26, 2
	slwi    r0, r0, 2
	add     r30, r25, r0
	stw     r30, tail(r13)
	bne     @1
	lwz     r12, comp(r13)
	lwz     r3, 0(r25)
	lwz     r4, 0(r30)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @43
	lwz     r0, 0(r25)
	lwz     r3, tail(r13)
	stw     r0, p(r13)
	lwz     r0, 0(r3)
	stw     r0, 0(r25)
	lwz     r0, p(r13)
	lwz     r3, tail(r13)
	stw     r0, 0(r3)
	b       @43
	@1:
	cmplwi  r26, 4
	bne     @2
	addi    r0, r30, -4
	stw     r25, lo(r13)
	mr      r29, r30
	stw     r30, gt(r13)
	stw     r0, hi(r13)
	b       shared_80006A94
	@2:
	cmplwi  r26, 40
	bge     @8
	lwz     r12, comp(r13)
	extlwi  r0, r26, 30,1
	lwz     r3, 0(r25)
	add     r29, r25, r0
	lwz     r4, 0(r30)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @5
	lwz     r12, comp(r13)
	lwz     r3, 0(r25)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	bge     @3
	lwz     r3, tail(r13)
	lwz     r0, 0(r3)
	stw     r0, p(r13)
	lwz     r0, 0(r29)
	stw     r0, 0(r3)
	lwz     r0, 0(r25)
	stw     r0, 0(r29)
	lwz     r0, p(r13)
	stw     r0, 0(r25)
	b       @7
	@3:
	lwz     r4, tail(r13)
	lwz     r12, comp(r13)
	lwz     r3, 0(r29)
	lwz     r4, 0(r4)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	bge     @4
	lwz     r0, 0(r25)
	stw     r0, p(r13)
	lwz     r0, 0(r29)
	stw     r0, 0(r25)
	lwz     r3, tail(r13)
	lwz     r0, 0(r3)
	stw     r0, 0(r29)
	lwz     r0, p(r13)
	lwz     r3, tail(r13)
	stw     r0, 0(r3)
	b       @7
	@4:
	lwz     r0, 0(r25)
	lwz     r3, tail(r13)
	stw     r0, p(r13)
	lwz     r0, 0(r3)
	stw     r0, 0(r25)
	lwz     r0, p(r13)
	lwz     r3, tail(r13)
	stw     r0, 0(r3)
	b       @7
	@5:
	lwz     r12, comp(r13)
	lwz     r3, 0(r25)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @6
	lwz     r0, 0(r25)
	stw     r0, p(r13)
	lwz     r0, 0(r29)
	stw     r0, 0(r25)
	lwz     r0, p(r13)
	stw     r0, 0(r29)
	b       @7
	@6:
	lwz     r4, tail(r13)
	lwz     r12, comp(r13)
	lwz     r3, 0(r29)
	lwz     r4, 0(r4)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @7
	lwz     r0, 0(r29)
	lwz     r3, tail(r13)
	stw     r0, p(r13)
	lwz     r0, 0(r3)
	stw     r0, 0(r29)
	lwz     r0, p(r13)
	lwz     r3, tail(r13)
	stw     r0, 0(r3)
	@7:
	cmplwi  r26, 3
	beq     @43
	lwz     r3, tail(r13)
	addi    r0, r25, 4
	stw     r0, lo(r13)
	addi    r0, r3, -4
	stw     r3, gt(r13)
	stw     r0, hi(r13)
	b       shared_80006A94
	@8:
	li      r7, -1
	li      r6, 0x20
	li      r5, 0x3F
	b       @13
	@9:
	srawi.r3, r6, 1
	and r4, r7, r26
	li      r0, 1
	beq     @10
	mr      r0, r3
	@10:
	cmplwi  r4, 0
	slw     r0, r7, r0
	and r7, r7, r0
	beq     @11
	subf    r5, r6, r5
	@11:
	cmplwi  r4, 0
	bne     @12
	srw     r7, r7, r6
	@12:
	mr      r6, r3
	@13:
	cmplwi  r7, 0
	bne     @9
	li      r0, 0x20
	subfc   r0, r5, r0
	subfe   r3, r0, r0
	subfic  r0, r5, 0x20
	andc    r3, r0, r3
	clrlwi.r0, r3, 31
	srawi   r0, r3, 1
	srw     r4, r26, r0
	beq     @14
	srawi   r0, r3, 1
	li      r3, 1
	slw     r4, r3, r0
	@14:
	divwu   r26, r26, r4
	srwi    r3, r4, 31
	addi    r0, r4, -1
	add     r3, r3, r4
	mr      r28, r25
	srawi   r3, r3, 1
	mullw   r0, r0, r26
	slwi    r31, r26, 2
	mullw   r3, r3, r26
	slwi    r0, r0, 2
	add     r0, r25, r0
	stw     r0, tail(r13)
	slwi    r0, r3, 2
	add     r29, r25, r0
	@15:
	lwz     r4, tail(r13)
	stw     r28, lo(r13)
	subf    r0, r28, r4
	srawi   r0, r0, 2
	stw     r4, gt(r13)
	addze   r3, r0
	srwi    r0, r3, 31
	stw     r4, hi(r13)
	add     r0, r0, r3
	srawi   r0, r0, 1
	divwu   r0, r0, r26
	mullw   r0, r26, r0
	slwi    r0, r0, 2
	add     r27, r28, r0
	b       @19
	@16:
	lwz     r12, comp(r13)
	lwz     r3, 0(r3)
	lwz     r4, 0(r27)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @17
	lwz     r0, hi(r13)
	stw     r0, gt(r13)
	b       @18
	@17:
	blt     @24
	@18:
	lwz     r0, hi(r13)
	subf    r0, r31, r0
	stw     r0, hi(r13)
	@19:
	lwz     r3, hi(r13)
	lwz     r0, lo(r13)
	cmplw   r3, r0
	bge     @16
	b       @25
	b       @24
	@20:
	lwz     r12, comp(r13)
	lwz     r3, 0(r3)
	lwz     r4, 0(r27)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	blt     @23
	lwz     r5, lo(r13)
	lwz     r4, hi(r13)
	lwz     r0, 0(r5)
	stw     r0, p(r13)
	lwz     r0, 0(r4)
	stw     r0, 0(r5)
	lwz     r0, p(r13)
	lwz     r4, hi(r13)
	stw     r0, 0(r4)
	ble     @21
	lwz     r0, hi(r13)
	stw     r0, gt(r13)
	@21:
	neg     r0, r3
	andc    r0, r0, r3
	srwi.r0, r0, 31
	bne     @22
	lwz     r27, hi(r13)
	@22:
	lwz     r3, lo(r13)
	lwz     r0, hi(r13)
	add     r3, r3, r31
	subf    r0, r31, r0
	stw     r3, lo(r13)
	stw     r0, hi(r13)
	b       @19
	@23:
	lwz     r0, lo(r13)
	add     r0, r0, r31
	stw     r0, lo(r13)
	@24:
	lwz     r3, lo(r13)
	lwz     r0, hi(r13)
	cmplw   r3, r0
	blt     @20
	@25:
	lwz     r3, gt(r13)
	cmplw   r3, r27
	bge     @26
	lwz     r0, 0(r3)
	stw     r0, p(r13)
	lwz     r0, 0(r27)
	stw     r0, 0(r3)
	lwz     r0, p(r13)
	stw     r0, 0(r27)
	lwz     r0, gt(r13)
	add     r0, r0, r31
	stw     r0, gt(r13)
	@26:
	lwz     r0, gt(r13)
	cmplw   r0, r29
	bgt     @27
	mr      r28, r0
	b       @28
	@27:
	lwz     r0, hi(r13)
	cmplw   r0, r29
	blt     @29
	stw     r0, tail(r13)
	@28:
	lwz     r0, tail(r13)
	cmplw   r28, r0
	blt     @15
	@29:
	stw     r25, lo(r13)
	stw     r30, tail(r13)
	stw     r30, gt(r13)
	stw     r30, hi(r13)
	b       shared_80006A94
	@30:
	lwz     r12, comp(r13)
	lwz     r3, 0(r3)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @31
	lwz     r0, hi(r13)
	stw     r0, gt(r13)
	b       @32
	@31:
	blt     @37
	@32:
	lwz     r3, hi(r13)
	addi    r0, r3, -4
	stw     r0, hi(r13)
	shared_80006A94 :
	lwz     r3, hi(r13)
	lwz     r0, lo(r13)
	cmplw   r3, r0
	bge     @30
	b       @38
	b       @37
	@33:
	lwz     r12, comp(r13)
	lwz     r3, 0(r3)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	blt     @36
	lwz     r5, lo(r13)
	lwz     r4, hi(r13)
	lwz     r0, 0(r5)
	stw     r0, p(r13)
	lwz     r0, 0(r4)
	stw     r0, 0(r5)
	lwz     r0, p(r13)
	lwz     r4, hi(r13)
	stw     r0, 0(r4)
	ble     @34
	lwz     r0, hi(r13)
	stw     r0, gt(r13)
	@34:
	neg     r0, r3
	andc    r0, r0, r3
	srwi.r0, r0, 31
	bne     @35
	lwz     r29, hi(r13)
	@35:
	lwz     r4, lo(r13)
	lwz     r3, hi(r13)
	addi    r4, r4, 4
	addi    r0, r3, -4
	stw     r4, lo(r13)
	stw     r0, hi(r13)
	b       shared_80006A94
	@36:
	lwz     r3, lo(r13)
	addi    r0, r3, 4
	stw     r0, lo(r13)
	@37:
	lwz     r3, lo(r13)
	lwz     r0, hi(r13)
	cmplw   r3, r0
	blt     @33
	@38:
	lwz     r4, gt(r13)
	cmplw   r4, r29
	bge     @39
	lwz     r3, 0(r4)
	addi    r0, r4, 4
	stw     r3, p(r13)
	lwz     r3, 0(r29)
	stw     r0, gt(r13)
	stw     r3, 0(r4)
	lwz     r0, p(r13)
	stw     r0, 0(r29)
	@39:
	lwz     r4, hi(r13)
	lwz     r5, gt(r13)
	subf    r0, r25, r4
	lwz     r6, tail(r13)
	srawi   r0, r0, 2
	addze   r3, r0
	subf    r0, r5, r6
	srawi   r0, r0, 2
	addze   r0, r0
	cmpw    r3, r0
	bge     @40
	stw     r25, hi(r13)
	mr      r25, r5
	stw     r4, lo(r13)
	b       @41
	@40:
	stw     r6, lo(r13)
	stw     r4, tail(r13)
	stw     r5, hi(r13)
	@41:
	lwz     r0, tail(r13)
	lwz     r5, lo(r13)
	lwz     r3, hi(r13)
	subf    r0, r25, r0
	srawi   r0, r0, 2
	addze   r4, r0
	cmplw   r5, r3
	addi    r26, r4, 1
	ble     @42
	subf    r0, r3, r5
	srawi   r0, r0, 2
	addze   r4, r0
	addi    r4, r4, 1
	bl      fsort
	@42:
	cmplwi  r26, 1
	bgt     @0
	@43:
	lmw     r25, 0x14(r1)
	lwz     r0, 0x34(r1)
	mtlr    r0
	addi    r1, r1, 0x30
	blr
}
#endif

/*
void fsort(u32* array, u32 num_elements) {
	u32* last_element;
	u32* important;
	do {
		last_element = &array[num_elements - 1];
		tail = last_element;
		if (num_elements == 2) {
			if (comp(*array, *last_element) > 0) {
				p = *array;
				*array = *tail;
				*tail = p;
			}
			return;
		}
		if (num_elements == 4) {
			lo = array;
			gt = last_element;
			hi = last_element - 1;
			//goto shared_80006A94;
		}
		if (num_elements < 40) {
			//comp(*array, *last_element)
		}

		


		if (gt < important) {
			p = *gt++;
			*gt = *important;
			*important = p;
		}
		if (hi - array >= tail - gt) {
			lo = tail;
			tail = hi;
			hi = gt;
		}
		else {
			hi = array;
			array = gt;
			lo = hi;
		}
		num_elements = tail - array + 1;
		if (lo > hi) {
			fsort(hi, lo - hi + 1);
		}
	} while (num_elements > 1);
}*/

/*
void fsort(void* array, u32 num_elements) {
	TODO: finish or embed, looks like it inlined itself
	
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

	}
}
*/




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







/*double distABf(double x1, double y1, double x2, double y2) { //TODO: decompile properly? see below with checks
	return sqrt(((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1))); //sqrt((x2-x1)^2+(y2-y1)^2);
}*/

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

u8* LZ77Decode(u8* input, u8* output) {
	return NULL;
}

f32 intplGetValue(s32 mode, s32 currStep, s32 steps, f32 start, f32 end) { //1:1
    f32 step;
    f32 total;
    f32 f0, f1, f2, f3, f4;

    if (!steps) {
        return end;
    }
    step = (f32)currStep;
    total = (f32)steps;
    switch (mode) {
        case 0:
            return start + ((step * (end - start)) / total);
        case 1:
            return start + (((step * step) * (end - start)) / (total * total));
        case 2:
            return start + (((step * step * step) * (end - start)) / (total * total * total));
        case 3:
            return start + (((step * step * step * step) * (end - start)) / (total * (total * (total * total))));
        case 7:
            f2 = (f32)cos(4.0f * (PI * (step / total)));
            f1 = (end - start);
            f3 = (total - step);
            return end - ((f3 * (f3 * (f1 * f2))) / (total * total));
        case 8:
            f2 = (f32)cos((4.0f * (PI * ((step * step) / total))) / ((15.0f * total) / 100.0f));
            f1 = (end - start);
            f3 = (total - step);
            return end - ((f3 * (f3 * (f1 * f2))) / (total * total));
        case 9:
            return end - ((step * (step * ((end - start) * (f32)cos((4.0f * (PI * ((step * step) / total))) / ((15.0f * total) / 100.0f))))) / (total * total));
        case 4:
            f1 = (total - step);
            return (start + (end - start)) - (((f1 * f1) * (end - start)) / (total * total));
        case 5:
            f4 = (total - step);
            return (start + (end - start)) - (((f4 * f4 * f4) * (end - start)) / (total * total * total));
        case 6:
            f4 = (total - step);
            return (start + (end - start)) - (((f4 * f4 * f4 * f4) * (end - start)) / (total * total * total * total));
        case 10:
            f2 = (f32)cos((4.0f * (PI * ((step * step) / total))) / ((40.0f * total) / 100.0f));
            f3 = (total - step);
            f1 = (f3 * (f3 * f2)) / (total * total);
            if (f1 < 0.0f) {
                f1 = -f1;
            }
            return -((f1 * (end - start)) - end);
        case 11:
            f3 = (f32)cos((PI * step) / total);
            f2 = (end - start);
            f0 = 0.5f;
            f1 = (f2 * (1.0f - f3));
            return start + (f1 * f0);
        case 12:
            f1 = (f32)sin((HALF_PI * step) / total);
            f0 = (end - start);
            return start + (f0 * f1);
        case 13:
            f2 = (f32)cos((HALF_PI * step) / total);
            f1 = (end - start);
            return start + (f1 * (1.0f - f2));
        default:
            return ((f32(*)(s32, s32, f32, f32))mode)(currStep, steps, start, end);
    }
}

#pragma pool_data off