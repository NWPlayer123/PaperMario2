#include "drv/camdrv.h"
#include "sdk/DEMOInit.h"
#include "memory.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846264338327950288419716939937510

//.bss
cameraObj* camPtrTbl[13]; //TODO: #define?

//.sdata
Vec camPtDefault = { 0.0f, 0.0f, 1000.0f };
Vec camAtDefault = { 0.0f, 0.0f, 0.0f };
Vec camUpDefault = { 0.0f, 1.0f, 0.0f };


//local callbacks
void camEffMain(cameraObj* camera);
void cam3dImgMain(cameraObj* camera);
void cam3dMain(cameraObj* camera);

cameraObj* camEntryOrtho(void (*callback)(cameraObj*), char* camera_name, f32 fovY, f32 aspect, f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far);
cameraObj* camEntryPersp(void (*callback)(cameraObj*), char* camera_name, f32 fovY, f32 aspect, f32 near, f32 far);


void camEffMain(cameraObj* camera) {

}

void cam3dImgMain(cameraObj* camera) {

}

void cam3dMain(cameraObj* camera) {

}







void camDraw(void) {

}

void camEvalNearFar(cameraObj* camera) {

}

void camMain(void) {

}

cameraObj* camEntryOrtho(void (*callback)(cameraObj*), char* camera_name, f32 fovY, f32 aspect, f32 top, f32 bottom, f32 left, f32 right, f32 near, f32 far) {
	cameraObj* camera;

	camera = __memAlloc(0, sizeof(cameraObj));
	memset(camera, 0, sizeof(cameraObj));
	MTXOrtho(camera->mProjectionMtx, top, bottom, left, right, near, far);
	camera->mProjectionType = GX_ORTHOGRAPHIC;
	camera->mCameraPos = camPtDefault;
	camera->mTarget = camAtDefault;
	camera->mCameraUp = camUpDefault;
	camera->mNearZ = near;
	camera->mFarZ = far;
	camera->field_0x114 = 0.0f;
	camera->field_0x118 = VECDistance(&camera->mCameraPos, &camera->mTarget);
	camera->mpCallback = callback;
	camera->mMode = 0;
	camera->mBankRotation = 0.0f;
	camera->mPostTranslation = (Vec){ 0.0f, 0.0f, 0.0f };
	camera->mFovY = fovY;
	camera->mAspect = aspect;
	camera->field_0x1E8 = 8;
	strcpy((char*)&camera->mName, camera_name);
	camera->mScissor[0] = 0;
	camera->mScissor[1] = 0;
	camera->mScissor[2] = DEMOGetRenderModeObj()->fbWidth;
	camera->mScissor[3] = DEMOGetRenderModeObj()->efbHeight;
	camera->mProjection[0] = 0.0f;
	camera->mProjection[1] = 0.0f;
	camera->mProjection[2] = (f32)DEMOGetRenderModeObj()->fbWidth;
	camera->mProjection[3] = (f32)DEMOGetRenderModeObj()->efbHeight;
	camera->mProjection[4] = 0.0f;
	camera->mProjection[5] = 1.0f;
	return camera;
}

cameraObj* camEntryPersp(void (*callback)(cameraObj*), char* camera_name, f32 fovY, f32 aspect, f32 near, f32 far) {
	return NULL;
}

void camInit(void) {
	camPtrTbl[0] = camEntryOrtho(NULL, "off", 25.0f, 76.0f / 60.0f, 240.0f, -240.0f, -304.0f, 304.0f, 1.0f, 32768.0f);
	camPtrTbl[1] = camEntryOrtho(NULL, "off2", 25.0f, 76.0f / 60.0f, 0.0f, 480.0f, 0.0f, 608.0f, 1.0f, 32768.0f);
	camPtrTbl[2] = camEntryOrtho(NULL, "shadow", 25.0f, 76.0f / 60.0f, 0.0f, 480.0f, 0.0f, 608.0f, 1.0f, 32768.0f);
	camPtrTbl[3] = camEntryOrtho(NULL, "bg", 25.0f, 76.0f / 60.0f, 0.0f, 480.0f, 0.0f, 608.0f, 1.0f, 32768.0f);
	camPtrTbl[4] = camEntryPersp(cam3dMain, "3d", 25.0f, 76.0f / 60.0f, 1.0f, 32768.0f);
	camPtrTbl[5] = camEntryPersp(camEffMain, "3deff_A", 25.0f, 76.0f / 60.0f, 100.0f, 10000.0f);
	camPtrTbl[6] = camEntryPersp(cam3dImgMain, "3dimg", 25.0f, 4.0f / 3.0f, 1.0f, 32768.0f);
	camPtrTbl[6]->mCameraPos = (Vec){ 0.0f, 0.0f, 24.0f / (f32)tan((f64)0x3FCBECDE20000000) }; //((12.499998355705682 * PI) / 180.0)
	camPtrTbl[7] = camEntryPersp(camEffMain, "3deff_B", 25.0f, 76.0f / 60.0f, 100.0f, 10000.0f);
	camPtrTbl[8] = camEntryPersp(NULL, "2d", 25.0f, 76.0f / 60.0f, 100.0f, 10000.0f);
	camPtrTbl[8]->mCameraPos = (Vec){ 0.0f, 0.0f, 24.0f / (f32)tan((f64)0x3FCBECDE20000000) };
	camPtrTbl[9] = camEntryPersp(NULL, "fade", 25.0f, 4.0f / 3.0f, 1.0f, 32768.0f);
	camPtrTbl[9]->mCameraPos = (Vec){ 0.0f, 0.0f, 24.0f / (f32)tan((f64)0x3FCBECDE20000000) };
	camPtrTbl[10] = camEntryPersp(NULL, "fade2", 25.0f, 4.0f / 3.0f, 1.0f, 32768.0f);
	camPtrTbl[10]->mCameraPos = (Vec){ 0.0f, 0.0f, 24.0f / (f32)tan((f64)0x3FCBECDE20000000) };
	camPtrTbl[11] = camEntryOrtho(NULL, "dbg", 25.0f, 76.0f / 60.0f, 0.0f, 480.0f, 0.0f, 608.0f, 1.0f, 32768.0f);
	camPtrTbl[12] = camEntryPersp(NULL, "dbg3d", 25.0f, 76.0f / 60.0f, 1.0f, 32768.0f);
}