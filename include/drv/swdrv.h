#pragma once

#include <string.h>
#include <dolphin/types.h>
#include "mariost.h"

u32 _swByteGet(u32 index);
void _swByteSet(u32 index, u32 value);
void _swClear(u32 index);
u32 _swGet(u32 index);
void _swSet(u32 index);

u32 swByteGet(u32 index);
void swByteSet(u32 index, u32 value);
void swClear(u32 index);
u32 swGet(u32 index);
void swSet(u32 index);

void swReInit(void);
void swInit(void);