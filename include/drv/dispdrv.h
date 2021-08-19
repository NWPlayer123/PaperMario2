#pragma once

#include "drv/camdrv.h"
#include <dolphin/mtx.h>
#include <dolphin/mtx/mtx44ext.h>

typedef void (*DispCallback)(CameraId cameraId, void* param);

typedef struct DispEntry {
	CameraId cameraId; //0x0
	u8 renderMode; //0x1
	//will pad 2 bytes
	f32 order; //0x4
	DispCallback callback; //0x8
	void* param; //0xC
} DispEntry;

void dispInit(void);
void dispReInit(void);
void dispEntry(CameraId cameraId, u8 renderMode, DispCallback callback, void* param, f32 order);
void dispSort(void);
f32 dispCalcZ(Vec input);




void dispDraw(CameraId cameraId);