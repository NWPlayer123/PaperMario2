#include "drv/npcdrv.h"
#include "drv/animdrv.h"
#include "drv/dispdrv.h"
#include "mario.h"
#include "mariost.h"
#include "memory.h"
#include <string.h>

extern marioStruct* gp;
typedef struct NpcWork2 {
	NpcWork field; //0x0
	NpcWork battle; //0x14
} NpcWork2;

//.sbss
BOOL dbg_battleskip;
u32 npcMainCount;
NpcFiledEntry* release_wp;
static NpcWork2 work;


//usually inlined
NpcWork* npcGetWorkPtr(void) {
	return gp->isBattleInit ? &work.battle : &work.field;
}

void npcReleaseFiledNpc(void) {
	NpcFiledEntry* field;
	NpcWork* fieldwork;
	NpcEntry* entry;
	AnimPose* pose;
	int i;

	fieldwork = &work.field;
	if (fieldwork->wFlags & 2) {
		for (i = 0; i < fieldwork->npcMaxCount; i++) {
			entry = &fieldwork->entries[i];
			if (entry->mFlags & 1 && entry->mFlags & 2 &&
				animPoseGetAnimPosePtr(entry->poseId)->mEffectPoseIdx == -1 &&
				entry->poseId >= 0) {

					field = &release_wp[i];
					pose = animPoseGetAnimPosePtr(entry->poseId);
					field->mFlags = pose->mFlags;
					field->wCurrentRenderRotationMinus90Y = pose->field_0x70;
					field->wRotationY = pose->mRotationY;
					field->wScaleZ_flip = pose->field_0x78;
					field->wInitToZero4 = pose->field_0x7C;
					strcpy(field->mModelName, animPoseGetAnimBaseDataPtr(entry->poseId)->mFileName);
					strcpy(field->mAnimName, animPoseGetCurrentAnim(entry->poseId)->mAnimName);
					animPoseRelease(entry->poseId);
					entry->poseId = -1;
			}
		}
	}
}

void npcRecoveryFiledNpc(void) {
	NpcFiledEntry* field;
	NpcWork* fieldwork;
	NpcEntry* entry;
	AnimPose* pose;
	int i;

	fieldwork = &work.field;
	if (fieldwork->wFlags & 2) {
		for (i = 0; i < fieldwork->npcMaxCount; i++) {
			entry = &fieldwork->entries[i];
			if (entry->mFlags & 1 && entry->mFlags & 2 && entry->poseId == -1) {
				field = &release_wp[i];
				entry->poseId = animPoseEntry(field->mModelName, 0);
				animPoseSetAnim(entry->poseId, field->mAnimName, TRUE);
				pose = animPoseGetAnimPosePtr(entry->poseId);
				pose->mFlags = field->mFlags;
				pose->field_0x70 = field->wCurrentRenderRotationMinus90Y;
				pose->mRotationY = field->wRotationY;
				pose->field_0x78 = field->wScaleZ_flip;
				pose->field_0x7C = field->wInitToZero4;

				if (pose->mFlags & 0x40) {
					pose->mFlags &= ~0x40;
					animPoseWorldPositionEvalOn(entry->poseId);
				}
				if (pose->mFlags & 0x80) {
					pose->mFlags &= ~0x80;
					animPoseWorldMatrixEvalOn(entry->poseId);
				}
				animPoseMain(entry->poseId);
			}
		}
		fieldwork->wFlags &= ~2;
	}
}




//buncha disp functions that I need other stuff finished for




void npcInit(void) {
	work.field.npcMaxCount = NPC_FIELD_MAX_COUNT;
	work.field.entries = (NpcEntry*)__memAlloc(HEAP_DEFAULT, sizeof(NpcEntry) * NPC_FIELD_MAX_COUNT); //TODO: change to npcMaxCount when rewriting
	memset(work.field.entries, 0, sizeof(NpcEntry) * work.field.npcMaxCount);

	release_wp = (NpcFiledEntry*)__memAlloc(HEAP_DEFAULT, sizeof(NpcFiledEntry) * work.field.npcMaxCount);

	work.battle.npcMaxCount = NPC_BATTLE_MAX_COUNT;
	work.battle.entries = (NpcEntry*)__memAlloc(HEAP_DEFAULT, sizeof(NpcEntry) * NPC_BATTLE_MAX_COUNT); //TODO: change to npcMaxCount when rewriting
	memset(work.battle.entries, 0, sizeof(NpcEntry) * work.battle.npcMaxCount);

	gp->mpFieldBattleData = (FieldBattleData*)__memAlloc(HEAP_DEFAULT, sizeof(FieldBattleData));
	memset(gp->mpFieldBattleData, 0, sizeof(FieldBattleData));
	gp->mpFieldBattleData->mMode = 0;
	npcMainCount = 0;
}

void npcReset(BOOL isBattle) {
	NpcWork* npcwork;

	npcwork = &work.field;
	if (isBattle) {
		npcwork = &work.battle;
	}
	memset(npcwork->entries, 0, sizeof(NpcEntry) * npcwork->npcMaxCount);
	npcwork->npcCount = 0;
	if (!isBattle) {
		gp->mpFieldBattleData->mMode = 0;
	}
	npcMainCount = 0;
}

u32 npcGetReactionOfLivingBody(BOOL isBattle) {
	NpcWork* npcwork;

	npcwork = &work.field;
	if (isBattle) {
		npcwork = &work.battle;
	}
	return npcwork->npcCount;
}

s32 npcEntry(const char* a1, const char* animName) {
	NpcWork* npcwork;
	NpcEntry* entry;
	s32 i;

	npcwork = npcGetWorkPtr(); //inlined

	// unused ------------------------------------------------
	for (i = 0; i < npcwork->npcMaxCount; i++) {
		entry = &npcwork->entries[i];
		if (entry->mFlags & 1 && !strcmp(entry->field_0x8, a1)) {
			break; //found an entry, unused
		}
	}
	// unused ------------------------------------------------

	if (npcwork->npcMaxCount > 0) {
		for (i = 0; i < npcwork->npcMaxCount; i++) {
			entry = &npcwork->entries[i];
			if (!(entry->mFlags & 1)) {
				break; //unused entry
			}
		}
	}
	memset(entry, 0, sizeof(NpcEntry));
	entry->mFlags = 3;
	strcpy(entry->field_0x8, a1);
	entry->poseId = animPoseEntry(animName, gp->isBattleInit != 0);
	if (entry->poseId == -2) {
		entry->poseId = animPoseEntry("hoshi", gp->isBattleInit != 0);
	}
	animPosePeraOn(entry->poseId);
	entry->position.x = 0.0f;
	entry->position.y = 0.0f;
	entry->position.z = 0.0f;
	entry->prevPosition.x = entry->position.x;
	entry->prevPosition.y = entry->position.y;
	entry->prevPosition.z = entry->position.z;
	entry->width = animPoseGetHeight(entry->poseId);
	entry->height = animPoseGetRadius(entry->poseId);
	entry->scale.x = 1.0f;
	entry->scale.y = 1.0f;
	entry->scale.z = 1.0f;
	entry->field_0x154 = 1.0f;
	entry->field_0x108 = 0;
	entry->field_0x10C = "M_I_2";
	entry->field_0x198 = gp->mAnimationTimeInclBattle;
	entry->field_0x188 = 0;
	entry->field_0x1CC = 1.0f;
	entry->field_0x1D0 = 0.0f;
	entry->field_0x1C8 = 1.0f;
	entry->color = (GXColor){ 0xFF, 0xFF, 0xFF, 0xFF };
	entry->cameraId = kCam3d;
	entry->field_0x2F8 = 0;
	entry->field_0x317 = 1;
	if (animPoseGetVivianType(entry->poseId)) {
		entry->mFlags |= 0x4000000;
	}
	if (!strcmp(animName, "c_luigi")) {
		entry->mFlags |= 0x800000u;
	}
	animPoseSetMaterialLightFlagOn(entry->poseId, 2);
	npcwork->npcCount++;
	return i;
}














void npcMain(void) {
	NpcWork* npcwork;
	NpcEntry* entry;
	int i;
	
	npcwork = gp->isBattleInit ? &work.battle : &work.field;
	//npcwork = npcGetWorkPtr();
	marioGetPtr()->field_0x1D8 = 0;
	npcMainCount++;

	for (i = 0; i < npcwork->npcMaxCount; i++) {
		entry = &npcwork->entries[i];
		if (entry->mFlags & 1 && entry->mFlags & 2) {
			if (entry->mFlags & 0x8000) {
				animPoseGetAnimPosePtr(entry->poseId)->mFlags |= 0x100;
			}
			else {
				animPoseGetAnimPosePtr(entry->poseId)->mFlags &= ~0x100;
			}

			if (!(entry->mFlags & 0x100000)) {
				entry->wJumpFlags |= 0x10;
			}
			if (entry->wJumpFlags & 0x10000) {
				entry->wJumpFlags &= ~0x10;
			}

			if (entry->wFbatHitCheckRelated) {
				entry->wFbatHitCheckRelated--;
			}

			if (entry->wJumpFlags & 0x10) {
				entry->field_0x190 = gp->mAnimationTimeInclBattle - entry->field_0x198;
				if (OSTicksToMilliseconds(entry->field_0x190) > 500) {
					entry->field_0x190 = OSMillisecondsToTicks(16);
				}
				if (entry->mFlags & 0x20000) {
					entry->field_0x178 += entry->field_0x190;
				}
				else {
					entry->field_0x178 = 0;
				}
				entry->field_0x188 += entry->field_0x190;
				entry->field_0x198 = gp->mAnimationTimeInclBattle;
				entry->field_0x1A0 = (f32)(OSTicksToMicroseconds(entry->field_0x188)) / 1000000.0f;
				entry->field_0x180 = (f32)(OSTicksToMicroseconds(entry->field_0x178)) / 1000000.0f;
				entry->field_0x1A4 = (f32)(OSTicksToMicroseconds(entry->field_0x190)) / 1000000.0f;
				entry->field_0x1A8 = (f32)(OSTicksToMicroseconds(entry->field_0x198)) / 1000000.0f;









			}
			else {
				entry->field_0x198 = gp->mAnimationTimeInclBattle;
			}
		}
	}
}














FieldBattleData* fbatGetPointer(void) {
	return gp->mpFieldBattleData;
}
