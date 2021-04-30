#pragma once

#include <dolphin/types.h>
#include <musyx.h>

void SoundInit(void);
void SoundMain(void);


BOOL SoundLoadDVD2(const char* path);



void SoundSetOutputMode(SND_OUTPUTMODE mode);
void SoundOpenCover(void);
void SoundCloseCover(void);