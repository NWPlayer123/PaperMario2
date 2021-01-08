#pragma once

#include <dolphin/types.h>

void swInit(void);
void swReInit(void);

//Local Saved Work
u32 _swByteGet(u32 index);
void _swByteSet(u32 index, u8 value);
//Local Saved Work Flags
void _swClear(u32 index);
u8 _swGet(u32 index);
void _swSet(u32 index);
//Global Saved Work
u32 swByteGet(u32 index);
void swByteSet(u32 index, u32 value);
//Global Saved Work Flags
void swClear(u32 index);
u8 swGet(u32 index);
void swSet(u32 index);
