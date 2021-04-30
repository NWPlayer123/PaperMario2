#pragma once

#include <dolphin/types.h>

void psndInit(void);
void psndMain(void);

void psndExit(void);
BOOL psndBGMOn(u32 r3, char* bgm_name);
void psndStopAllFadeOut(void);
BOOL psndENVOff(u32 r3);
void psndSFXAllOff(void);