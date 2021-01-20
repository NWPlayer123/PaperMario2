/*
 * Warning, 
 */
#include "drv/animdrv.h"
#include "mariost.h"
#include "memory.h"
#include "system.h"
#include <math.h>
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
void animPoseRefresh(void);
void animPose_AllocBuffer(AnimPose* pose);
void* testAlloc(u32 size);
void initTestHeap(void);

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


//TODO: finish this
s32 animPoseEntry(char* fileName, u32 heapType) {
	AnimPose* pose;
	s32 result;
	int i;

	switch (heapType) {
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
	result = 0;
	if (wp->mAnimPoseCapacity <= 0) {
		pose = NULL;
		result = -1; //ERROR
	}
	else {
		for (i = 0; i < wp->mAnimPoseCapacity; i++) {
			if (!wp->mpAnimPoses[i].mFlags) {
				pose = &wp->mpAnimPoses[i];
				break;
			}
			result++;
		}
	}
	pose->mFlags |= 1;
	pose->mTypeFlags = 0;
	pose->mRefCount = 0;
	for (i = 0; i < wp->mAnimPoseCapacity; i++) {

	}
}

void animPoseBattleInit(void) {
	wp->mbIsBattle = TRUE;
	animPoseRefresh();
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
			if ((wp->mbIsBattle || pose->mHeapType != 0) && (wp->mbIsBattle || pose->mHeapType != 1)) {
				animPose_AllocBuffer(pose);
				pose->mLastAnimFrame0 = -1; //TODO: redundant?
			}
		}
	}
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

void animMain(void) {
	//dispEntry(0, 1, animPaperPoseDisp, 0, 0.0f);
}

void animInit(void) {
	fileObj* file;
	int i;

	wp->mAnimFileCapacity = 0x40; //TODO: #define these?
	wp->mTexFileCapacity = 0x40;
	wp->mAnimPoseCapacity = 0x100;
	wp->mFloatScratchRP = 0x400;
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

//heavily inlined
void* testAlloc(u32 size) {
	void* alloc;

	alloc = wp->mTestHeapPtr;
	size = OSRoundUp32B(size); //TODO: check asm
	wp->mTestHeapPtr = (void*)((u32)wp->mTestHeapPtr + size);
	if ((u32)wp->mTestHeapPtr >= (u32)wp->mTestHeap + 0x180000) { //need to allocate more
		animPoseRefresh();
		alloc = testAlloc(size);
	}
	return alloc;
}

void initTestHeap(void) {
	if (!wp->mTestHeap) { //allocate 1.5MiB test heap
		wp->mTestHeap = __memAlloc(HEAP_DEFAULT, (u32)(1.5 * 1024 * 1024));
	}
	wp->mTestHeapPtr = wp->mTestHeap;
}

OSTime animTimeGetTime(BOOL InclBattle) {
	if (InclBattle) {
		return OSTicksToSeconds(gp->mAnimationTimeInclBattle);
	}
	else {
		return OSTicksToSeconds(gp->mAnimationTimeNoBattle);
	}
}

AnimWork* animGetPtr(void) {
	return wp;
}