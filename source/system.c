#include <dolphin/os.h>

u8 tmp1[0x100]; //for memcpy element in qqsort
u32 tmp0[0xC00]; //buffer for working with array in qqsort

extern void* memset(void* ptr, int val, u32 num);
extern void* memcpy(void* dst, const void* src, u32 num);

extern BOOL __mapdrv_make_dl;
int irand(int scalar);
void qqsort(void* array, u32 size, u32 elemsize, void* compare);
void fsort(u32* table, u32 size);

typedef s32 (*sortcmp)(u32 num1, u32 num2);
u16 token;
u32 p;
u32* tail;
u32* gt;
u32* hi;
u32* lo;
sortcmp* comp;

void sysWaitDrawSync(void) {
	u16 this_token;
	OSTick tick1, tick2;

	this_token = token++;
	tick1 = OSGetTick();
	if (__mapdrv_make_dl) return;
	GXSetDrawSync(this_token);
	while (GXReadDrawSync() != this_token) {
		if ((OSGetTick() - tick1) / ((OS_BUS_CLOCK >> 2) / 1000) > 100) break; //OSTicksToMilliseconds
	}
}

u16 sysGetToken(void) {
	return token++;
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

#ifndef __INTELLISENSE__
asm void qqsort(void* array, u32 size, u32 elemsize, void* compare) {
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
	stw     r6, comp
	addi    r6, r7, tmp0@l
	mr      r24, r6
	ble     @8
	cmplwi  r30, 0
	mr      r5, r29
	mr      r4, r6
	mr      r3, r30
	ble     @3
	srwi.r0, r3, 3
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
	andi.r3, r3, 7
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

asm void fsort(u32* table, u32 size) {
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
	stw     r30, tail
	bne     @1
	lwz     r12, comp
	lwz     r3, 0(r25)
	lwz     r4, 0(r30)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @44
	lwz     r0, 0(r25)
	lwz     r3, tail
	stw     r0, p
	lwz     r0, 0(r3)
	stw     r0, 0(r25)
	lwz     r0, p
	lwz     r3, tail
	stw     r0, 0(r3)
	b       @44
	@1:
	cmplwi  r26, 4
	bne     @2
	addi    r0, r30, -4
	stw     r25, lo
	mr      r29, r30
	stw     r30, gt
	stw     r0, hi
	b       @33
	@2:
	cmplwi  r26, 0x28
	bge     @8
	lwz     r12, comp
	extlwi  r0, r26, 30,1
	lwz     r3, 0(r25)
	add     r29, r25, r0
	lwz     r4, 0(r30)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @5
	lwz     r12, comp
	lwz     r3, 0(r25)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	bge     @3
	lwz     r3, tail
	lwz     r0, 0(r3)
	stw     r0, p
	lwz     r0, 0(r29)
	stw     r0, 0(r3)
	lwz     r0, 0(r25)
	stw     r0, 0(r29)
	lwz     r0, p
	stw     r0, 0(r25)
	b       @7
	@3:
	lwz     r4, tail
	lwz     r12, comp
	lwz     r3, 0(r29)
	lwz     r4, 0(r4)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	bge     @4
	lwz     r0, 0(r25)
	stw     r0, p
	lwz     r0, 0(r29)
	stw     r0, 0(r25)
	lwz     r3, tail
	lwz     r0, 0(r3)
	stw     r0, 0(r29)
	lwz     r0, p
	lwz     r3, tail
	stw     r0, 0(r3)
	b       @7
	@4:
	lwz     r0, 0(r25)
	lwz     r3, tail
	stw     r0, p
	lwz     r0, 0(r3)
	stw     r0, 0(r25)
	lwz     r0, p
	lwz     r3, tail
	stw     r0, 0(r3)
	b       @7
	@5:
	lwz     r12, comp
	lwz     r3, 0(r25)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @6
	lwz     r0, 0(r25)
	stw     r0, p
	lwz     r0, 0(r29)
	stw     r0, 0(r25)
	lwz     r0, p
	stw     r0, 0(r29)
	b       @7
	@6:
	lwz     r4, tail
	lwz     r12, comp
	lwz     r3, 0(r29)
	lwz     r4, 0(r4)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @7
	lwz     r0, 0(r29)
	lwz     r3, tail
	stw     r0, p
	lwz     r0, 0(r3)
	stw     r0, 0(r29)
	lwz     r0, p
	lwz     r3, tail
	stw     r0, 0(r3)
	@7:
	cmplwi  r26, 3
	beq     @44
	lwz     r3, tail
	addi    r0, r25, 4
	stw     r0, lo
	addi    r0, r3, -4
	stw     r3, gt
	stw     r0, hi
	b       @33
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
	stw     r0, tail
	slwi    r0, r3, 2
	add     r29, r25, r0
	@15:
	lwz     r4, tail
	stw     r28, lo
	subf    r0, r28, r4
	srawi   r0, r0, 2
	stw     r4, gt
	addze   r3, r0
	srwi    r0, r3, 31
	stw     r4, hi
	add     r0, r0, r3
	srawi   r0, r0, 1
	divwu   r0, r0, r26
	mullw   r0, r26, r0
	slwi    r0, r0, 2
	add     r27, r28, r0
	b       @19
	@16:
	lwz     r12, comp
	lwz     r3, 0(r3)
	lwz     r4, 0(r27)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @17
	lwz     r0, hi
	stw     r0, gt
	b       @18
	@17:
	blt     @24
	@18:
	lwz     r0, hi
	subf    r0, r31, r0
	stw     r0, hi
	@19:
	lwz     r3, hi
	lwz     r0, lo
	cmplw   r3, r0
	bge     @16
	b       @25
	b       @24
	@20:
	lwz     r12, comp
	lwz     r3, 0(r3)
	lwz     r4, 0(r27)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	blt     @23
	lwz     r5, lo
	lwz     r4, hi
	lwz     r0, 0(r5)
	stw     r0, p
	lwz     r0, 0(r4)
	stw     r0, 0(r5)
	lwz     r0, p
	lwz     r4, hi
	stw     r0, 0(r4)
	ble     @21
	lwz     r0, hi
	stw     r0, gt
	@21:
	neg     r0, r3
	andc    r0, r0, r3
	srwi.r0, r0, 31
	bne     @22
	lwz     r27, hi
	@22:
	lwz     r3, lo
	lwz     r0, hi
	add     r3, r3, r31
	subf    r0, r31, r0
	stw     r3, lo
	stw     r0, hi
	b       @19
	@23:
	lwz     r0, lo
	add     r0, r0, r31
	stw     r0, lo
	@24:
	lwz     r3, lo
	lwz     r0, hi
	cmplw   r3, r0
	blt     @20
	@25:
	lwz     r3, gt
	cmplw   r3, r27
	bge     @26
	lwz     r0, 0(r3)
	stw     r0, p
	lwz     r0, 0(r27)
	stw     r0, 0(r3)
	lwz     r0, p
	stw     r0, 0(r27)
	lwz     r0, gt
	add     r0, r0, r31
	stw     r0, gt
	@26:
	lwz     r0, gt
	cmplw   r0, r29
	bgt     @27
	mr      r28, r0
	b       @28
	@27:
	lwz     r0, hi
	cmplw   r0, r29
	blt     @29
	stw     r0, tail
	@28:
	lwz     r0, tail
	cmplw   r28, r0
	blt     @15
	@29:
	stw     r25, lo
	stw     r30, tail
	stw     r30, gt
	stw     r30, hi
	b       @33
	@30:
	lwz     r12, comp
	lwz     r3, 0(r3)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	ble     @31
	lwz     r0, hi
	stw     r0, gt
	b       @32
	@31:
	blt     @38
	@32:
	lwz     r3, hi
	addi    r0, r3, -4
	stw     r0, hi
	@33:
	lwz     r3, hi
	lwz     r0, lo
	cmplw   r3, r0
	bge     @30
	b       @39
	b       @38
	@34:
	lwz     r12, comp
	lwz     r3, 0(r3)
	lwz     r4, 0(r29)
	mtctr   r12
	bctrl
	cmpwi   r3, 0
	blt     @37
	lwz     r5, lo
	lwz     r4, hi
	lwz     r0, 0(r5)
	stw     r0, p
	lwz     r0, 0(r4)
	stw     r0, 0(r5)
	lwz     r0, p
	lwz     r4, hi
	stw     r0, 0(r4)
	ble     @35
	lwz     r0, hi
	stw     r0, gt
	@35:
	neg     r0, r3
	andc    r0, r0, r3
	srwi.r0, r0, 31
	bne     @36
	lwz     r29, hi
	@36:
	lwz     r4, lo
	lwz     r3, hi
	addi    r4, r4, 4
	addi    r0, r3, -4
	stw     r4, lo
	stw     r0, hi
	b       @33
	@37:
	lwz     r3, lo
	addi    r0, r3, 4
	stw     r0, lo
	@38:
	lwz     r3, lo
	lwz     r0, hi
	cmplw   r3, r0
	blt     @34
	@39:
	lwz     r4, gt
	cmplw   r4, r29
	bge     @40
	lwz     r3, 0(r4)
	addi    r0, r4, 4
	stw     r3, p
	lwz     r3, 0(r29)
	stw     r0, gt
	stw     r3, 0(r4)
	lwz     r0, p
	stw     r0, 0(r29)
	@40:
	lwz     r4, hi
	lwz     r5, gt
	subf    r0, r25, r4
	lwz     r6, tail
	srawi   r0, r0, 2
	addze   r3, r0
	subf    r0, r5, r6
	srawi   r0, r0, 2
	addze   r0, r0
	cmpw    r3, r0
	bge     @41
	stw     r25, hi
	mr      r25, r5
	stw     r4, lo
	b       @42
	@41:
	stw     r6, lo
	stw     r4, tail
	stw     r5, hi
	@42:
	lwz     r0, tail
	lwz     r5, lo
	lwz     r3, hi
	subf    r0, r25, r0
	srawi   r0, r0, 2
	addze   r4, r0
	cmplw   r5, r3
	addi    r26, r4, 1
	ble     @43
	subf    r0, r3, r5
	srawi   r0, r0, 2
	addze   r4, r0
	addi    r4, r4, 1
	bl      fsort
	@43:
	cmplwi  r26, 1
	bgt     @0
	@44:
	lmw     r25, 0x14(r1)
	lwz     r0, 0x34(r1)
	mtlr    r0
	addi    r1, r1, 0x30
	blr
}
/*char* end;
do {
    end = table + ((size - 1) << 2);
    tail = end; //TODO: elim end?
    if (size == 2) {
        if ((*comp)(*table, *end) < 1)
            return;
        p = *table;
        *table = *tail;
        *tail = p;
        return;
    }
    if (size == 4) {
        lo = table;
        gt = end;
        hi = end - 4;
        goto something;
    }
    if (size < 0x28) {
        if ((*comp)(*table, *end) > 0) {
            if ((*comp)(*table, *(u32*)(table + ((size << 1) & 0xFFFFFFFC))))
        }
    }
} while (1);*/
#endif