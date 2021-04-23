#pragma once

#include <dolphin/mtx.h>

char* getMarioStDvdRoot(void);
f32 reviseAngle(f32 angle);
f32 distABf(f32 x1, f32 y1, f32 x2, f32 y2);
f32 compAngle(f32 angle1, f32 angle2);






void* memcpy_as4(void* dst, void* src, u32 len);
void mtxGetRotationElement(Mtx* mtx1, Mtx* mtx2, char oldaxis, char newaxis);
f32 sysMsec2FrameFloat(f32 msec);
s32 sysMsec2Frame(s32 msec);
f32 sysFrame2SecFloat(f32 frame);
u16 sysGetToken(void);
void sysWaitDrawSync(void);



void makeKey(void);


u32 keyGetButton(u32 chan);
u32 keyGetButtonTrg(u32 chan);
u32 keyGetButtonRep(u32 chan);
u32 keyGetDir(u32 chan);
u32 keyGetDirTrg(u32 chan);
u32 keyGetDirRep(u32 chan);
s8 keyGetStickX(u32 chan);
s8 keyGetStickY(u32 chan);
s8 keyGetSubStickX(u32 chan);
s8 keyGetSubStickY(u32 chan);
void padRumbleOff(u32 chan);
void padRumbleOn(u32 chan);
void padRumbleHardOff(u32 chan);
u8 padGetRumbleStatus(u32 chan);




void qqsort(void* array, u32 num_elements, u32 element_size, s32(*compare)(void*, void*));


int irand(int value);