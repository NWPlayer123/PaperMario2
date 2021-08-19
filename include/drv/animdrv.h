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
	u8 field_0xF8[0x100 - 0xF8]; //0xF8
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
	u64 mLocalTime; //0x18
	u8 field_0x20[0x3C - 0x20]; //0x20
	s32 mLastAnimFrame0; //0x3C
	u8 field_0x40[0x44 - 0x40]; //0x40
	f32 mLocalTimeRate; //0x44
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
	u8 field_0x80[0x84 - 0x80]; //0x80
	f32 mLoopTime; //0x84
	u64 mLocalTimeInit; //0x88
	s32 mEffectPoseIdx; //0x90
	u8 field_0x94[0xE0 - 0x94]; //0x94
	void (*gxCallback)(s32 wXluStage); //0xE0
	BOOL disableDraw; //0xE4
	u32 mMaterialFlag; //0xE8
	u32 mMaterialLightFlag; //0xEC
	GXColor mMaterialEvtColor; //0xF0
	GXColor mMaterialEvtColor2; //0xF4
	u8 field_0xF8[0x11C - 0xF8]; //0xF8
	s32 mVivianType; //0x11C
	u8 field_0x120[0x170 - 0x120]; //0x120
};

struct AnimPoseFile {
	BOOL mHasData; //0x0
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
	u8 field_0xC4[0xD0 - 0xC4]; //0xC4
	Vec bboxMin; //0xD0
	Vec bboxMax; //0xDC
	u8 field_0xE8[0xF0 - 0xE8]; //0xE8
	u32 mBufferPosNum; //0xF0
	u32 mBufferPosIdxNum; //0xF4
	u32 mBufferNrmNum; //0xF8
	u32 mBufferNrmIdxNum; //0xFC
	u8 field_0x100[0x12C - 0x100]; //0x100
	u32 mBufferTexAnimNum; //0x12C
	u8 field_0x130[0x13C - 0x130]; //0x130
	u32 mBufferVisibilityNum; //0x13C
	u32 mBufferNodeNum; //0x140
	s32 mGroupNum; //0x144
	s32 mAnimCount; //0x148
	u8 field_0x14C[0x1AC - 0x14C]; //0x14C
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
	BOOL mHasData; //0x0
	u32 mRefCount; //0x4
	void* mppData; //0x8
};

AnimWork* animGetPtr(void);
OSTime animTimeGetTime(BOOL InclBattle);
void animInit(void);
void animMain(void);
void animPoseBattleInit(void);

s32 animPoseEntry(const char* animName, u32 group); //TODO: add enum?
s32 animPaperPoseEntry(const char* animName, u32 group);

BOOL animEffectAsync(const char* animName, u32 group);

void animPosePeraOn(s32 poseId);
void animPosePeraOff(s32 poseId);

void animPosePaperPeraOn(s32 poseId);
void animPosePaperPeraOff(s32 poseId);

void animPoseSetLocalTimeRate(s32 poseId, f32 localTimeRate);
void animPoseSetLocalTime(s32 poseId, f32 localTimeFrames);
void animPoseSetStartTime(s32 poseId, OSTime startTime);

void animPoseSetAnim(s32 poseId, const char* animName, BOOL reset);

s32 animPaperPoseGetId(const char* animName, s32 group);
void animPoseSetPaperAnimGroup(s32 poseId, const char* animName, BOOL someflag);
void animPoseSetPaperAnim(s32 poseId, const char* animName);
void animPoseSetEffect(s32 poseId, const char* animName, BOOL someflag);
void animPoseSetEffectAnim(s32 poseId, const char* animName, BOOL reset);




AnimTableEntry* animPoseGetCurrentAnim(s32 poseId);
AnimPoseData* animPoseGetAnimBaseDataPtr(s32 poseId);
AnimData* animPoseGetAnimDataPtr(s32 poseId);
AnimPose* animPoseGetAnimPosePtr(s32 poseId);



void animPaperPoseRelease(s32 poseId);
void animPoseRelease(s32 poseId);



GXColor* animPoseGetMaterialEvtColor(s32 poseId);
u32 animPoseGetMaterialLightFlag(s32 poseId);
u32 animPoseGetMaterialFlag(s32 poseId);
void animPoseSetMaterialEvtColor(s32 poseId, GXColor* color);
void animPoseSetMaterialLightFlagOff(s32 poseId, u32 mask);
void animPoseSetMaterialLightFlagOn(s32 poseId, u32 mask);
void animPoseSetMaterialFlagOff(s32 poseId, u32 mask);
void animPoseSetMaterialFlagOn(s32 poseId, u32 mask);
BOOL animPoseGetPeraEnd(s32 poseId);


f32 animPoseGetRadius(s32 poseId);
f32 animPoseGetHeight(s32 poseId);


s32 animPoseGetVivianType(s32 poseId);



f32 animPoseGetLoopTimes(s32 id);

void animPoseWorldPositionEvalOn(s32 poseId);
void animPoseWorldMatrixEvalOn(s32 poseId);
void animPoseMain(s32 poseId);





BOOL animGroupBaseAsync(const char* name, s32 group, void* callback);



void animPoseSetGXFunc(s32 poseId, void (*callback)(s32 wXluStage), BOOL disableDraw);