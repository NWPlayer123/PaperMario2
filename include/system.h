#pragma once

#include <dolphin/mtx.h>

void* memcpy_as4(void* dst, void* src, u32 len);
void mtxGetRotationElement(Mtx* mtx1, Mtx* mtx2, char oldaxis, char newaxis);
f32 sysMsec2FrameFloat(f32 msec);
s32 sysMsec2Frame(s32 msec);
f32 sysFrame2SecFloat(f32 frame);
u16 sysGetToken(void);
void sysWaitDrawSync(void);


void qqsort(void* array, u32 num_elements, u32 element_size, s32(*compare)(void*, void*));


int irand(int value);

double compAngle(double angle1, double angle2);
double distABf(double x1, double y1, double x2, double y2);
double reviseAngle(double angle);
char* getMarioStDvdRoot(void);