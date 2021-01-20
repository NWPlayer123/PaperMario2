#pragma once

#include "mgr/filemgr.h"
#include <dolphin/mtx.h>
#include <dolphin/os.h>

typedef struct AnimWork AnimWork;
typedef struct AnimPose AnimPose;
typedef struct AnimData AnimData;
typedef struct AnimPoseFile AnimPoseFile;
typedef struct AnimPoseData AnimPoseData;
typedef struct AnimPoseNode AnimPoseNode;
typedef struct AnimTexMtx AnimTexMtx;
typedef struct AnimTexFile AnimTexFile;
typedef struct AnimTableEntry AnimTableEntry;

struct AnimWork {
	AnimPoseFile* mpAnimFiles; //0x0
	u32 mAnimFileCapacity; //0x4
	AnimTexFile* mpTexFiles; //0x8
	u32 mTexFileCapacity; //0xC
	AnimPose* mpAnimPoses; //0x10
	u32 mAnimPoseCapacity; //0x14
	AnimPose* mpCurPose; //0x18
	AnimData* mpCurAnimData; //0x1C
	AnimPoseData* mpCurPoseData; //0x20
	void* mpTexDatas[2]; //0x24
	void* mpCurTexData; //0x2C
	GXTexObj* mpTexObjs[2]; //0x30
	GXTexObj* mpCurTexObj; //0x38
	u8 field_0x3C[0xCC - 0x3C]; //0x3C
	Mtx* mpPaperMtx1; //0xCC
	Mtx* mpPaperMtx2; //0xD0
	Mtx* mpPaperMtx3; //0xD4
	u32 field_0xD8; //0xD8
	u32 field_0xDC; //0xDC
	u8 field_0xE0[0xE8 - 0xE0]; //0xE0
	u32 mbUseFloatScratch; //0xE8
	Vec* mpFloatScratch; //0xEC
	u32 mFloatScratchRP; //0xF0
	u32 mFloatScratchWP; //0xF4
	u8 field_0xF8[0x104 - 0xF8]; //0xF8
	fileObj* mp_ag2tg; //0x100
	void* mTestHeap; //0x104
	void* mTestHeapPtr; //0x108, TODO better name?
	BOOL mbIsBattle; //0x10C
};

struct AnimPose {
	u32 mFlags; //0x0
	u32 mTypeFlags; //0x4
	u32 mRefCount; //0x8
	u32 mHeapType; //0xC
	u32 mFileIdx; //0x10
	u32 mCurAnimIdx; //0x14
	u8 field_0x18[0x3C - 0x18]; //0x18
	s32 mLastAnimFrame0; //0x3C
	u8 field_0x40[0x48 - 0x40]; //0x40
	Vec* mpBufferVtxPos; //0x48
	Vec* mpVtxArrayPos; //0x4C
	Vec* mpBufferVtxNrm; //0x50
	Vec* mpVtxArrayNrm; //0x54
	u8* mpBufferGroupVisibility; //0x58
	u8* mpGroupVisibility; //0x5C
	AnimPoseNode* mpBufferNode; //0x60
	AnimPoseNode* mpNodes; //0x64
	AnimTexMtx* mpBufferTexAnimEntries; //0x68
	AnimTexMtx* mpTexAnimEntries; //0x6C
	f32 field_0x70; //0x70
	f32 mRotationY; //0x74
	f32 field_0x78; //0x78
	f32 field_0x7C; //0x7C
	u8 field_0x80[0x90 - 0x80]; //0x80
	s32 mEffectPoseIdx; //0x90
	u8 field_0x94[0x170 - 0x94]; //0x94
};

struct AnimPoseFile {
	u32 mbHasData; //0x0
	u32 mRefCount; //0x4
	fileObj* mpFile; //0x8
	void* mTexFileIdx; //0xC
};

//all of these fall under "AnimPoseData"
struct AnimData {
	u8 field_0x0[0x44 - 0]; //0x0
};

struct AnimTableEntry {
	char mAnimName[60]; //0x0
	AnimData* mpAnimData; //0x3C
};




//end "AnimPoseData" substructs
struct AnimPoseNode {
	Vec mTranslation; //0x0
	Vec mScale; //0xC
	Vec mRot1; //0x18
	Vec mRot2; //0x24
	Vec mRotationCenter; //0x30
	Vec mScaleCenter; //0x3C
	Vec mRotationTranslation; //0x48
	Vec mScaleTranslation; //0x54
};

//this is the structure of the actual stored files
struct AnimPoseData {
	u32 mSize; //0x0
	char mFileName[64]; //0x4
	char mTexFileName[64]; //0x44
	char mBuildTime[64]; //0x84
	u8 field_0xC4[0xF0 - 0xC4]; //0xC4
	u32 mBufferPosNum; //0xF0
	u32 mBufferPosIdxNum; //0xF4
	u32 mBufferNrmNum; //0xF8
	u32 mBufferNrmIdxNum; //0xFC
	u8 field_0x100[0x12C - 0x100]; //0x100
	u32 mBufferTexAnimNum; //0x12C
	u8 field_0x130[0x13C - 0x130]; //0x130
	u32 mBufferVisibilityNum; //0x13C
	u32 mBufferNodeNum; //0x140
	u8 field_0x144[0x1AC - 0x144]; //0x144
	AnimTableEntry* mpAnims; //0x1AC
};

struct AnimTexMtx {
	u8 mTexBindIdx; //0x0
	u8 align[3]; //0x1, ???
	f32 mTransS; //0x4
	f32 mTransT; //0x8
	f32 mScaleS; //0xC
	f32 mScaleT; //0x10
	f32 mRotate; //0x14
};

struct AnimTexFile {
	u32 mbHasData; //0x0
	u32 mRefCount; //0x4
	void* mppData; //0x8
};

AnimTableEntry* animPoseGetCurrentAnim(s32 poseId);
AnimPoseData* animPoseGetAnimBaseDataPtr(s32 poseId);
AnimData* animPoseGetAnimDataPtr(s32 poseId);
AnimPose* animPoseGetAnimPosePtr(s32 poseId);


//TODO: add enum when changing filemgr archivetype
s32 animPoseEntry(char* fileName, u32 heapType);
void animPoseBattleInit(void);
void animMain(void);
void animInit(void);
OSTime animTimeGetTime(BOOL InclBattle);
AnimWork* animGetPtr(void);