#pragma once

#include <dolphin/gx.h>

extern void DEMOInit(GXRenderModeObj* mode);
extern void DEMOBeforeRender(void);
extern void DEMODoneRender(void);
extern void DEMOSwapBuffers(void);
extern GXRenderModeObj* DEMOGetRenderModeObj(void);
extern void* DEMOGetCurrentBuffer(void);
extern void DEMOEnableGPHangWorkaround(u32 timeoutFrames);
extern void DEMOReInit(GXRenderModeObj* mode);
extern void DEMOSetGPHangMetric(GXBool enable);