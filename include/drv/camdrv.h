#pragma once

#include <dolphin/gx.h>
#include <dolphin/mtx.h>

//#pragma enumsalwaysint on

//see camInit
typedef enum CameraId {
	kCamOffscreen = 0,
	kCamOffscreen2,
	kCamShadow,
	kCamBackground,
	kCam3d,
	kCam3dEffectA,
	kCam3dImage,
	kCam3dEffectB,
	kCam2d,
	kCamFade,
	kCamFade2,
	kCamDebug,
	kCamDebug3d
} CameraId;

typedef struct cameraObj { //TODO: better name?
	u16 mFlags; //0x0
	u16 mMode; //0x2
	u16 field_0x4; //0x4
	u16 field_0x6; //0x6, padding?
	f32 field_0x8; //0x8
	Vec mCameraPos; //0xC
	Vec mTarget; //0x18
	Vec mCameraUp; //0x24
	f32 mNearZ; //0x30
	f32 mFarZ; //0x34
	f32 mFovY; //0x38
	f32 mAspect; //0x3C
	u8 field_0x40[0xF4 - 0x40]; //0x40
	u16 mScissor[4]; //0xF4, TODO retype?
	f32 mProjection[6]; //0xFC, TODO retype?
	f32 field_0x114; //0x114
	f32 field_0x118; //0x118
	Mtx mViewMtx; //0x11C
	f32 mBankRotation; //0x14C
	Vec mPostTranslation; //0x150
	Mtx44 mProjectionMtx; //0x15C
	GXProjectionType mProjectionType; //0x19C
	u8 field_0x1A0[0x1E8 - 0x1A0]; //0x1A0
	u32 field_0x1E8; //0x1E8
	void* mpCallback; //0x1EC, TODO rename?
	Vec field_0x1F0; //0x1F0
	u32 field_0x1FC; //0x1FC, unknown
	Vec field_0x200; //0x200
	Vec field_0x20C; //0x20C
	u16 field_0x218; //0x218
	u16 field_0x21A; //0x21A
	u16 field_0x21C; //0x21C
	u16 field_0x21E; //0x21E
	u32 field_0x220; //0x220, unknown
	Vec field_0x224; //0x224
	Vec field_0x230; //0x230
	Vec field_0x23C; //0x23C
	Vec field_0x248; //0x248
	char mName[0xC]; //0x254
} cameraObj;

//u32 test = sizeof(cameraObj); //0x260/608

void camInit(void);
void camMain(void);
void camDraw(void);
void camEvalNearFar(cameraObj* camera);
cameraObj* camGetPtr(s32 id);
cameraObj* camGetCurPtr(void);
