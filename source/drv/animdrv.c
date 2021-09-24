/*
 * Warning, inlining hell
 */
#include "drv/animdrv.h"
#include "drv/dispdrv.h"
#include "mariost.h"
#include "memory.h"
#include "system.h"
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846264338327950288419716939937510f
#define DEG_TO_RAD(deg) ((deg) * 0.017453292f)

extern GlobalWork* gp;
extern int sprintf(char* str, const char* format, ...);

//.data
u8 _vivihimoData[0xC00]; //TODO: embed the actual data? gets overwritten by a/vivian.bin in animInit()

//.bss
static AnimWork work;
f32 tanfTbl[0xB2];

//.sdata
s32 vivihimoTailGroupNo[4] = { 0, 2, -1, -1 };
void* vivihimoData[4] = {
	NULL,
	&_vivihimoData,
	&_vivihimoData + 0x400,
	&_vivihimoData + 0x800,
};
static AnimWork* wp = &work;
s32 _dummy = -1;


//local declarations
void initTestHeap(void);
void* testAlloc(u32 size);
void animPose_AllocBuffer(AnimPose* pose);
void animPoseRefresh(void);


void animPaperPoseDisp(CameraId cameraId, void* param);
void animPaperPoseDispSub(s32 unused, AnimPose* pose);

AnimWork* animGetPtr(void) {
	return wp;
}

OSTime animTimeGetTime(BOOL inclBattle) {
	if (!inclBattle) {
		return OSTicksToMilliseconds(gp->mAnimationTimeNoBattle);
	}
	return OSTicksToMilliseconds(gp->mAnimationTimeInclBattle);
}

void initTestHeap(void) {
	if (!wp->testHeap) { //allocate 1.5MiB test heap
		wp->testHeap = __memAlloc(HEAP_DEFAULT, (u32)(1.5 * 1024 * 1024));
	}
	wp->testAlloc = wp->testHeap;
}

//heavily inlined
void* testAlloc(u32 size) {
	void* alloc;

	alloc = wp->testAlloc;
	if (size & 31) {
		size += 32 - (size & 31);
	}
	wp->testAlloc = (void*)((u32)wp->testAlloc + size);
	if ((u32)wp->testAlloc >= (u32)wp->testHeap + 0x180000) { //need to allocate more
		animPoseRefresh();
		alloc = testAlloc(size);
	}
	return alloc;
}

void animInit(void) {
	fileObj* file;
	int i;

	wp->mAnimFileCapacity = 64; //TODO: #define these?
	wp->mTexFileCapacity = 64;
	wp->mAnimPoseCapacity = 256;
	wp->mFloatScratchRP = 1024;
	wp->mFloatScratchWP = 0;
	wp->mpAnimFiles = __memAlloc(HEAP_DEFAULT, sizeof(AnimPoseFile) * wp->mAnimFileCapacity);
	wp->mpTexFiles = __memAlloc(HEAP_DEFAULT, sizeof(AnimTexFile) * wp->mTexFileCapacity);
	wp->mpAnimPoses = __memAlloc(HEAP_DEFAULT, sizeof(AnimPose) * wp->mAnimPoseCapacity);
	wp->mbUseFloatScratch = 0;
	wp->mpFloatScratch = __memAlloc(HEAP_DEFAULT, sizeof(Vec) * wp->mFloatScratchRP);

	wp->mpTexObjs[0] = NULL;
	wp->mpTexObjs[1] = NULL;
	wp->mpCurTexObj = NULL;

	wp->mpTexDatas[0] = NULL;
	wp->mpTexDatas[1] = NULL;
	wp->mpCurTexData = NULL;

	wp->mpPaperMtx1 = NULL;
	wp->mpPaperMtx2 = NULL;
	wp->mpPaperMtx3 = NULL;

	wp->field_0xD8 = 1;
	wp->field_0xDC = 1;

	wp->testHeap = NULL;
	wp->mbIsBattle = FALSE;
	initTestHeap();

	for (i = 0; i < 0xB2; i++) {
		tanfTbl[i] = tanf(DEG_TO_RAD(i - 0x59));
	}
	file = fileAllocf(0, "a/vivian.bin");
	if (file) {
		memcpy_as4(vivihimoData[1], *file->mppFileData, sizeof(_vivihimoData));
		memcpy_as4(vivihimoTailGroupNo, (void*)((u32)*file->mppFileData + sizeof(_vivihimoData)), sizeof(vivihimoTailGroupNo));
		fileFree(file);
	}
	fileSetCurrentArchiveType(0);
	wp->mp_ag2tg = fileAllocf(0, "a/ag2tg.bin");
}

void animMain(void) {
	dispEntry(kCamOffscreen, 1, animPaperPoseDisp, NULL, 0.0f);
}

//heavily inlined, TODO helper function for that AnimPoseData* data, copy of animPoseGetAnimBaseDataPtr with AnimPose* arg
void animPose_AllocBuffer(AnimPose* pose) {
	AnimPoseData* data = *wp->mpAnimFiles[pose->mFileIdx].mpFile->mppFileData;
	pose->mpBufferVtxPos = testAlloc(sizeof(Vec) * data->mBufferPosNum);
	pose->mpVtxArrayPos = testAlloc(sizeof(Vec) * data->mBufferPosNum);

	pose->mpBufferVtxNrm = testAlloc(sizeof(Vec) * data->mBufferNrmNum);
	pose->mpVtxArrayNrm = testAlloc(sizeof(Vec) * data->mBufferNrmNum);

	pose->mpBufferGroupVisibility = testAlloc(sizeof(u8) * data->mBufferVisibilityNum);
	pose->mpGroupVisibility = pose->mpBufferGroupVisibility;

	pose->mpBufferNode = testAlloc(sizeof(AnimPoseNode*) * data->mBufferNodeNum);
	pose->mpNodes = testAlloc(sizeof(AnimPoseNode*) * data->mBufferNodeNum);

	pose->mpBufferTexAnimEntries = testAlloc(sizeof(AnimTexMtx) * data->mBufferTexAnimNum);
	pose->mpTexAnimEntries = pose->mpBufferTexAnimEntries;

	pose->mLastAnimFrame0 = -1;
}

//heavily inlined
void animPoseRefresh(void) {
	AnimPose* pose;
	int i;

	initTestHeap(); //inline
	for (i = 0; i < wp->mAnimPoseCapacity; i++) {
		pose = animPoseGetAnimPosePtr(i); //inline
		if (pose->mFlags) {
			//TODO: cleanup in rewrite?
			if ((!wp->mbIsBattle || pose->mHeapType) && (wp->mbIsBattle || pose->mHeapType != 1)) {
				animPose_AllocBuffer(pose);
				pose->mLastAnimFrame0 = -1; //TODO: redundant?
			}
		}
	}
}

void animPoseBattleInit(void) {
	wp->mbIsBattle = TRUE;
	animPoseRefresh();
}

//TODO: needs some logic flow changes
s32 animPoseEntry(const char* animName, u32 group) {
	AnimPoseFile* file;
	AnimPoseData* data;
	AnimTexFile* tex;
	AnimPose* pose;
	s32 poseId, fileId, texId;
	int i;
	char v30[312]; //TODO: check size

	switch (group) {
		case 0:
			fileSetCurrentArchiveType(1);
			break;
		case 1:
			fileSetCurrentArchiveType(2);
			break;
		case 2:
			fileSetCurrentArchiveType(0);
			break;
	}

	if (wp->mAnimPoseCapacity > 0) {
		for (i = 0, poseId = 0; i < wp->mAnimPoseCapacity; i++, poseId++) {
			pose = &wp->mpAnimPoses[i];
			if (!pose->mFlags) {
				goto label_1;
			}
		}
	}
	pose = NULL;
	poseId = -1;
label_1:
	pose->mFlags |= 1u;
	pose->mTypeFlags = 0;
	pose->mRefCount = 0;

	for (i = 0; i < wp->mAnimFileCapacity; i++) {
		file = &wp->mpAnimFiles[i];
		data = *file->mpFile->mppFileData; //TODO: double check
		if (file->mHasData && !strcmp(data->mFileName, animName)) {
			file->mRefCount++;
			goto label_5;
		}
	}

	if (wp->mAnimFileCapacity > 0) {
		for (i = 0, fileId = 0; i < wp->mAnimFileCapacity; i++, fileId++) {
			file = &wp->mpAnimFiles[i];
			if (!file->mHasData) {
				goto label_2;
			}
		}
	}
	file = NULL;
	fileId = -1;
label_2:
	file->mHasData = TRUE;
	file->mRefCount++;
	file->mpFile = fileAllocf(5, "%s/%s", "a", animName);

	if (file->mpFile) {
		data = *file->mpFile->mppFileData;
		if (data) {
			sprintf(v30, "%s/%s-", "a", data->mTexFileName);
			texId = 0;
			for (i = 0; i < wp->mTexFileCapacity; i++) {
				tex = &wp->mpTexFiles[i];
				if (tex->mHasData) {
					if (tex->mppData) { //TODO: figure out mppData + 32
						if (!strcmp((const char*)((u32)tex->mppData + 32), v30)) {
							tex->mRefCount++;
							goto label_4;
						}
					}
				}
			}

			if (wp->mTexFileCapacity > 0) {
				for (i = 0, texId = 0; i < wp->mTexFileCapacity; i++, texId++) {
					tex = &wp->mpTexFiles[i];
					if (!tex->mHasData) {
						goto label_3;
					}
				}
			}
			tex = NULL;
			texId = -1;
label_3:
			tex->mHasData = TRUE;
			tex->mRefCount++;
			tex->mppData = fileAlloc(v30, 4);
label_4:
			file->mTexFileIdx = texId;
		}
		else {
			fileFree(file->mpFile);
			fileId = -2;
			file->mHasData = FALSE;
			file->mRefCount--;
		}
	}
	else {
		fileId = -2;
		file->mHasData = FALSE;
		file->mRefCount--;
	}
label_5:
	pose->mFileIdx = fileId;
	if (pose->mFileIdx == -2) {
		pose->mFlags = 0;
	}
	else {
		pose->mCurAnimIdx = 0;
		pose->mLocalTime = animTimeGetTime(group); //inline
		pose->mLastAnimFrame0 = -1;
		pose->mLastAnimFrameTime = 0.0f;
		pose->field_0x80 = 0;
		pose->mLoopTime = 0.0f;
		pose->field_0x70 = 0.0f;
		pose->mRotationY = 0.0f;
		pose->field_0x7C = 0.0f;
		pose->mHeapType = group;
		pose->mEffectPoseIdx = -1;
		animPose_AllocBuffer(pose);
		pose->gxCallback = NULL;
		pose->disableDraw = 0;
		pose->mMaterialFlag = 0;
		pose->mMaterialLightFlag = 0;
		pose->mMaterialEvtColor = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
		pose->mMaterialEvtColor2 = (GXColor){0xFF, 0xFF, 0xFF, 0xFF};
		pose->field_0xFC = 1.0f;
		pose->field_0xF8 = 1.0f;
		pose->field_0x100 = 0.0f;
		pose->mLocalTimeRate = 1.0f;
		MTXIdentity(pose->matrix);
		if (!strcmp(animName, "tst_vivi")) {
			pose->mVivianType = 1;
			pose->vivianGroupNo = &vivihimoTailGroupNo[0];
		}
		if (!strcmp(animName, "c_vivian")) {
			pose->mVivianType = 1;
			pose->vivianGroupNo = &vivihimoTailGroupNo[1];
		}
		if (!strcmp(animName, "c_maririn")) {
			pose->mVivianType = 2;
			pose->vivianGroupNo = &vivihimoTailGroupNo[2];
		}
		if (!strcmp(animName, "c_majyorin")) {
			pose->mVivianType = 3;
			pose->vivianGroupNo = &vivihimoTailGroupNo[3];
		}
		else { //if none match
			pose->mVivianType = 0;
			pose->vivianGroupNo = &_dummy;
		}
		pose->dispCallback = NULL;
		pose->dispUserDataCallback = NULL;
	}
	return poseId;
}

s32 animPaperPoseEntry(const char* animName, u32 group) {
	AnimPoseData* data;
	AnimPose* pose;
	s32 i, poseId;

	for (i = 0, poseId = 0; i < wp->mAnimPoseCapacity; i++, poseId++) {
		pose = &wp->mpAnimPoses[i];
		if (pose->mFlags & 1) {
			if (pose->mTypeFlags & 1 && pose->mTypeFlags & 2 && pose->mHeapType == group) {
				data = *wp->mpAnimFiles[pose->mFileIdx].mpFile->mppFileData;
				if (!strcmp(data->mFileName, animName)) {
					break;
				}
			}
		}
	}
	if (poseId == wp->mAnimPoseCapacity) {
		poseId = -1;
	}
	if (poseId < 0) {
		poseId = animPoseEntry(animName, group);
		if (poseId != -2) {
			pose = &wp->mpAnimPoses[poseId];
			pose->mTypeFlags = 3;
			pose->mRefCount = 0;
			pose->mFlags |= 1;
		}
	}
	else {
		wp->mpAnimPoses[poseId].mRefCount++;
	}
	return poseId;
}

BOOL animEffectAsync(const char* animName, u32 group) {
	s32 model, anim;
	void** ag2tg;

	switch (group) {
	case 0:
		fileSetCurrentArchiveType(1);
		break;
	case 1:
		fileSetCurrentArchiveType(2);
		break;
	case 2:
		fileSetCurrentArchiveType(0);
		break;
	}
	model = fileAsyncf(5, NULL, "%s/%s", "a", animName);
	
	return FALSE;
}

void animPosePeraOn(s32 poseId) {
	wp->mpAnimPoses[poseId].mFlags |= 8;
}

void animPosePeraOff(s32 poseId) {
	wp->mpAnimPoses[poseId].mFlags &= ~8;
}

void animPosePaperPeraOn(s32 poseId) {
	AnimPose *src, *dst;

	src = &wp->mpAnimPoses[poseId];
	dst = &wp->mpAnimPoses[src->mEffectPoseIdx];
	dst->mFlags |= 8;
	dst->field_0x70 = src->field_0x70;
	dst->mRotationY = src->mRotationY;
	dst->field_0x7C = src->field_0x7C;
}

void animPosePaperPeraOff(s32 poseId) {
	AnimPose* src;

	src = &wp->mpAnimPoses[poseId];
	wp->mpAnimPoses[src->mEffectPoseIdx].mFlags &= ~8;
}

void animPoseSetLocalTimeRate(s32 poseId, f32 localTimeRate) {
	wp->mpAnimPoses[poseId].mLocalTimeRate = localTimeRate;
}

//TODO: get animTimeGetTime to inline here
void animPoseSetLocalTime(s32 poseId, f32 localTimeFrames) {
	AnimPose *pose, *pera;

	pose = &wp->mpAnimPoses[poseId];
	pose->mLocalTime = animTimeGetTime(pose->mHeapType) - (localTimeFrames * 16.666666f);
	pose->mLocalTimeInit = pose->mLocalTime;
	if (pose->mEffectPoseIdx != -1) {
		pera = &wp->mpAnimPoses[pose->mEffectPoseIdx];
		pera->mLocalTime = pose->mLocalTime;
	}
}

void animPoseSetStartTime(s32 poseId, OSTime startTime) {
	wp->mpAnimPoses[poseId].mLocalTime = startTime;
}

void animPoseSetAnim(s32 poseId, const char* animName, BOOL reset) {
	AnimPoseData* data;
	AnimPose* pose;

	pose = &wp->mpAnimPoses[poseId];
	data = *wp->mpAnimFiles[pose->mFileIdx].mpFile->mppFileData;
}

s32 animPaperPoseGetId(const char* animName, s32 group) {
	return 0;
}

void animPoseSetPaperAnimGroup(s32 poseId, const char* animName, BOOL someflag) {

}

void animPoseSetPaperAnim(s32 poseId, const char* animName) {

}

void animPoseSetEffect(s32 poseId, const char* animName, BOOL someflag) {

}

void animPoseSetEffectAnim(s32 poseId, const char* animName, BOOL reset) {

}

/*
void animPoseSetGXFunc(s32 poseId, void (*gxCb)(s32 wXluStage), s32 wbDisableDraw) {

}
*/










AnimTableEntry* animPoseGetCurrentAnim(s32 poseId) {
	AnimPoseData* data = animPoseGetAnimBaseDataPtr(poseId);
	return &data->mpAnims[animPoseGetAnimPosePtr(poseId)->mCurAnimIdx];
}

AnimPoseData* animPoseGetAnimBaseDataPtr(s32 poseId) {
	AnimPose* pose = animPoseGetAnimPosePtr(poseId);
	return (AnimPoseData*)*wp->mpAnimFiles[pose->mFileIdx].mpFile->mppFileData;
}

AnimData* animPoseGetAnimDataPtr(s32 poseId) {
	AnimPose* pose = animPoseGetAnimPosePtr(poseId);
	return animPoseGetAnimBaseDataPtr(poseId)->mpAnims[pose->mCurAnimIdx].mpAnimData;
}

AnimPose* animPoseGetAnimPosePtr(s32 poseId) {
	return &wp->mpAnimPoses[poseId];
}




void animPaperPoseRelease(s32 poseId) {
	
}

void animPoseRelease(s32 poseId) {

}






GXColor* animPoseGetMaterialEvtColor(s32 poseId) {
	return &wp->mpAnimPoses[poseId].mMaterialEvtColor;
}

u32 animPoseGetMaterialLightFlag(s32 poseId) {
	return wp->mpAnimPoses[poseId].mMaterialLightFlag;
}

u32 animPoseGetMaterialFlag(s32 poseId) {
	return wp->mpAnimPoses[poseId].mMaterialFlag;
}

void animPoseSetMaterialEvtColor(s32 poseId, GXColor* color) {
	wp->mpAnimPoses[poseId].mMaterialEvtColor = *color;
}

void animPoseSetMaterialLightFlagOff(s32 poseId, u32 mask) {
	wp->mpAnimPoses[poseId].mMaterialLightFlag &= ~mask;
}

void animPoseSetMaterialLightFlagOn(s32 poseId, u32 mask) {
	wp->mpAnimPoses[poseId].mMaterialLightFlag |= mask;
}

void animPoseSetMaterialFlagOff(s32 poseId, u32 mask) {
	wp->mpAnimPoses[poseId].mMaterialFlag &= ~mask;
}

void animPoseSetMaterialFlagOn(s32 poseId, u32 mask) {
	wp->mpAnimPoses[poseId].mMaterialFlag |= mask;
}

BOOL animPoseGetPeraEnd(s32 poseId) {
	return FALSE;
}




f32 animPoseGetRadius(s32 poseId) {
	return 0.0f;
}

f32 animPoseGetHeight(s32 poseId) {
	return 0.0f;
}




s32 animPoseGetVivianType(s32 poseId) {
	return wp->mpAnimPoses[poseId].mVivianType;
}









f32 animPoseGetLoopTimes(s32 id) {
	return wp->mpAnimPoses[id].mLoopTime;
}



void animPoseWorldPositionEvalOn(s32 poseId) {

}

void animPoseWorldMatrixEvalOn(s32 poseId) {

}

void animPoseMain(s32 poseId) {

}






BOOL animGroupBaseAsync(const char* name, s32 group, void* callback) {
	//int i;

	switch (group) {
		case 0:
			fileSetCurrentArchiveType(1);
			break;
		case 1:
			fileSetCurrentArchiveType(2);
			break;
		case 2:
			fileSetCurrentArchiveType(0);
			break;
	}
	//fileAsyncf(5, callback, "%s/%s", "a", name);

	return FALSE;
}



void animPaperPoseDisp(CameraId cameraId, void* param) {
	AnimPose* pose;
	int i;

	for (i = 0; i < wp->mAnimPoseCapacity; i++) {
		pose = &wp->mpAnimPoses[i];
		if (pose->mFlags & 1 && pose->mEffectPoseIdx != -1) {
			animPaperPoseDispSub(0, pose);
		}
	}
}

void animPaperPoseDispSub(s32 unused, AnimPose* pose) {

}




void animPoseSetGXFunc(s32 poseId, void (*callback)(s32 wXluStage), BOOL disableDraw) {
	AnimPose* pose = &wp->mpAnimPoses[poseId];
	pose->gxCallback = callback;
	pose->disableDraw = disableDraw;
}