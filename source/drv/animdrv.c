/*
 * Warning, 
 */
#include "drv/animdrv.h"
#include "mariost.h"
#include "memory.h"
#include "system.h"
#include <math.h>
#include <string.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419716939937510
#endif

extern marioStruct* gp;

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


//local declarations
void initTestHeap(void);
void* testAlloc(u32 size);
void animPose_AllocBuffer(AnimPose* pose);
void animPoseRefresh(void);

AnimWork* animGetPtr(void) {
	return wp;
}

OSTime animTimeGetTime(BOOL InclBattle) {
	if (InclBattle == FALSE) {
		return OSTicksToMilliseconds(gp->mAnimationTimeNoBattle);
	}
	return OSTicksToMilliseconds(gp->mAnimationTimeInclBattle);
}

void initTestHeap(void) {
	if (!wp->mTestHeap) { //allocate 1.5MiB test heap
		wp->mTestHeap = __memAlloc(HEAP_DEFAULT, (u32)(1.5 * 1024 * 1024));
	}
	wp->mTestHeapPtr = wp->mTestHeap;
}

//heavily inlined
void* testAlloc(u32 size) {
	void* alloc;

	alloc = wp->mTestHeapPtr;
	//size = OSRoundUp32B(size); //TODO: check asm
	if (size & 31) {
		size += 32 - (size & 31);
	}
	wp->mTestHeapPtr = (void*)((u32)wp->mTestHeapPtr + size);
	if ((u32)wp->mTestHeapPtr >= (u32)wp->mTestHeap + 0x180000) { //need to allocate more
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
	wp->mpAnimFiles = (AnimPoseFile*)__memAlloc(HEAP_DEFAULT, sizeof(AnimPoseFile) * wp->mAnimFileCapacity);
	wp->mpTexFiles = (AnimTexFile*)__memAlloc(HEAP_DEFAULT, sizeof(AnimTexFile) * wp->mTexFileCapacity);
	wp->mpAnimPoses = (AnimPose*)__memAlloc(HEAP_DEFAULT, sizeof(AnimPose) * wp->mAnimPoseCapacity);
	wp->mbUseFloatScratch = 0;
	wp->mpFloatScratch = (Vec*)__memAlloc(HEAP_DEFAULT, sizeof(Vec) * wp->mFloatScratchRP);

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

	wp->mTestHeap = NULL;
	wp->mbIsBattle = FALSE;
	initTestHeap();

	for (i = 0; i < 0xB2; i++) {
		tanfTbl[i] = (f32)tan((M_PI / 180.0) * (f64)(i - 0x59));
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
	//dispEntry(kCamOffscreen, 1, 0.0f, animPaperPoseDisp, NULL);
}

//heavily inlined, TODO helper function for that AnimPoseData* data, copy of animPoseGetAnimBaseDataPtr with AnimPose* arg
void animPose_AllocBuffer(AnimPose* pose) {
	AnimPoseData* data = *wp->mpAnimFiles[pose->mFileIdx].mpFile->mppFileData;
	pose->mpBufferVtxPos = (Vec*)testAlloc(sizeof(Vec) * data->mBufferPosNum);
	pose->mpVtxArrayPos = (Vec*)testAlloc(sizeof(Vec) * data->mBufferPosNum);

	pose->mpBufferVtxNrm = (Vec*)testAlloc(sizeof(Vec) * data->mBufferNrmNum);
	pose->mpVtxArrayNrm = (Vec*)testAlloc(sizeof(Vec) * data->mBufferNrmNum);

	pose->mpBufferGroupVisibility = (u8*)testAlloc(sizeof(u8) * data->mBufferVisibilityNum);
	pose->mpGroupVisibility = pose->mpBufferGroupVisibility;

	pose->mpBufferNode = (AnimPoseNode*)testAlloc(sizeof(AnimPoseNode*) * data->mBufferNodeNum);
	pose->mpNodes = (AnimPoseNode*)testAlloc(sizeof(AnimPoseNode*) * data->mBufferNodeNum);

	pose->mpBufferTexAnimEntries = (AnimTexMtx*)testAlloc(sizeof(AnimTexMtx) * data->mBufferTexAnimNum);
	pose->mpTexAnimEntries = pose->mpBufferTexAnimEntries;

	pose->mLastAnimFrame0 = -1;
}

//heavily inlined
void animPoseRefresh(void) {
	AnimPose* pose;
	int i;

	initTestHeap(); //inline
	for (i = 0; i < wp->mAnimPoseCapacity; i++) {
		pose = animPoseGetAnimPosePtr(i);
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

//TODO: finish this, needs some logic flow changes
s32 animPoseEntry(const char* animName, u32 group) {
	AnimPoseFile* file;
	AnimPoseData* data;
	AnimPose* pose;
	s32 poseId, fileId;
	int i;

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
		//default, fall through
	}
	poseId = 0;
	if (wp->mAnimPoseCapacity <= 0) {
		pose = NULL;
		poseId = -1; //no more poses available to allocate
	}
	else {
		for (i = 0; i < wp->mAnimPoseCapacity; i++) {
			if (!wp->mpAnimPoses[i].mFlags) {
				pose = &wp->mpAnimPoses[i];
				break;
			}
			poseId++;
		}
	}
	pose->mFlags |= 1;
	pose->mTypeFlags = 0;
	pose->mRefCount = 0;
	for (i = 0; i < wp->mAnimFileCapacity; i++) {
		file = &wp->mpAnimFiles[i];
		data = (AnimPoseData*)*file->mpFile->mppFileData; //TODO: double check
		if (file->mHasData && !strcmp(data->mFileName, animName)) {
			file->mRefCount++;
			break; //GOTO some other part of the code, skip alloc and etc
		}
	}
	fileId = 0;
	if (wp->mAnimFileCapacity <= 0) {
		file = NULL;
		fileId = -1; //no more files available to allocate
	}
	else {
		for (i = 0; i < wp->mAnimFileCapacity; i++) {
			if (!wp->mpAnimFiles[i].mHasData) {
				file = &wp->mpAnimFiles[i];
				break;
			}
			fileId++;
		}
	}
	file->mHasData = TRUE;
	file->mRefCount++;
	file->mpFile = fileAllocf(5, "%s/%s", "a", animName);
	if (file->mpFile) {

	}


	return poseId;
}

s32 animPaperPoseEntry(const char* animName, u32 group) {
	return 0;
}

BOOL animEffectAsync(const char* animName, u32 group) {
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
		//default, fall through
	}
	//r31 = fileAsyncf(5, 0, "%s/%s", "a", animName);
	return FALSE;
}

void animPosePeraOn(s32 poseId) {
	wp->mpAnimPoses[poseId].mFlags |= 8u;
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
	pose->mLocalTime = (u64)(animTimeGetTime((BOOL)pose->mHeapType) - (localTimeFrames * (16 + (2/3))));
	pose->mLocalTimeInit = pose->mLocalTime;
	if (pose->mEffectPoseIdx != -1) {
		pera = &wp->mpAnimPoses[pose->mEffectPoseIdx];
		pera->mLocalTime = pose->mLocalTime;
	}
}

void animPoseSetStartTime(s32 poseId, OSTime startTime) {
	AnimPose* pose = &wp->mpAnimPoses[poseId];
	pose->mLocalTime = (u64)startTime;
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













f32 animPoseGetLoopTimes(s32 id) {
	return wp->mpAnimPoses[id].mLoopTime;
}