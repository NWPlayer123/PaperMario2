#include "drv/npcdrv.h"
#include "drv/animdrv.h"
#include "drv/dispdrv.h"
#include "drv/msgdrv.h"
#include "mario.h"
#include "mariost.h"
#include "memory.h"
#include "system.h"
#include <string.h>

extern GlobalWork* gp;
extern NpcTribe npcTribe[];

typedef struct NpcWork2 {
	NpcWork field; //0x0
	NpcWork battle; //0x14
} NpcWork2;

//.sbss
BOOL dbg_battleskip;
u32 npcMainCount;
NpcFiledEntry* release_wp;
static NpcWork2 work;



//local prototypes
void mtx_setup(NpcEntry* entry, Mtx mtx, s32 history);
void npcDisp_xlu(CameraId cameraId, void* param);
void npcDisp(CameraId cameraId, void* param);
void npcDisp_offscreen_xlu(CameraId cameraId, void* param);
void npcDisp_offscreen(CameraId cameraId, void* param);
void npcDisp_blur(CameraId cameraId, void* param);
void _npcDeleteGroup(NpcEntry* entry);

#define npcGetWorkPtr2(inBattle) ((inBattle) ? &work.battle : &work.field)
//TODO: change with npcGetWorkPtr once inlining works right?
#define npcGetWorkPtrInline() (gp->isBattleInit ? &work.battle : &work.field)

//usually inlined
NpcWork* npcGetWorkPtr(void) { //1:1
	return gp->isBattleInit ? &work.battle : &work.field;
}

void npcReleaseFiledNpc(void) { //1:1
	NpcWork* wp;
	NpcFiledEntry* field;
	NpcEntry* entry;
	AnimPose* pose;
	int i;

	wp = &work.field;
	field = release_wp;
	if (wp->wFlags & 2) {
		entry = wp->entries;
		for (i = 0; i < wp->npcMaxCount; i++, entry++) {
			//entry = &wp->entries[i];
			if (entry->mFlags & 1 && entry->mFlags & 2 &&
				animPoseGetAnimPosePtr(entry->poseId)->mEffectPoseIdx == -1) {
				if (entry->poseId >= 0) {
					pose = animPoseGetAnimPosePtr(entry->poseId);
					field->mFlags = pose->mFlags;
					field->wCurrentRenderRotationMinus90Y = pose->field_0x70;
					field->wRotationY = pose->mRotationY;
					field->wScaleZ_flip = pose->field_0x78;
					field->wInitToZero4 = pose->field_0x7C;
					strcpy(field->mModelName, animPoseGetAnimBaseDataPtr(entry->poseId)->mFileName);
					strcpy(field->mAnimName, animPoseGetCurrentAnim(entry->poseId)->mAnimName);
					animPoseRelease(entry->poseId);
					field++; //next release_wp entry
					entry->poseId = -1;
				}
			}
		}
	}
}

void npcRecoveryFiledNpc(void) { //1:1
	NpcWork* wp;
	NpcFiledEntry* field;
	NpcEntry* entry;
	AnimPose* pose;
	int i;

	wp = &work.field;
	field = release_wp;
	if (wp->wFlags & 2) {
		entry = wp->entries;
		for (i = 0; i < wp->npcMaxCount; i++, entry++) {
			if (entry->mFlags & 1 && entry->mFlags & 2 && entry->poseId == -1) {
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
				field++; //next release_wp entry
			}
		}
		wp->wFlags &= ~2;
	}
}

void mtx_setup(NpcEntry* entry, Mtx mtx, s32 history) { //1:1 once "!= 0.0f" checks are fixed
	f32 rotationAngle;
	f32 scaleSign;
	cameraObj* camera;
	f32 targetAngle;
	f32 positionAngle;
	f32 v15;
	Mtx mtxX, mtxY, mtxZ;

	rotationAngle = 0.0f;
	scaleSign = 1.0f;
	if (!(entry->mFlags & 0x2000000) && !(entry->mFlags & 0x8000000)) {
		camera = camGetCurPtr();
		targetAngle = angleABf(camera->mCameraPos.x, camera->mCameraPos.z, camera->mTarget.x, camera->mTarget.z);
		positionAngle = angleABf(camera->mCameraPos.x, camera->mCameraPos.z, entry->position.x, entry->position.z);
		rotationAngle = reviseAngle(targetAngle - positionAngle);
	}
	v15 = reviseAngle(reviseAngle(entry->rotation.y));
	if (v15 >= 90.0f && v15 <= 270.0f) {
		scaleSign *= -1.0f;
	}
	MTXIdentity(mtx);
	if (entry->scale.x != 1.0f || entry->scale.y != 1.0f || entry->scale.z != 1.0f || scaleSign != 1.0f) {
		MTXScaleApply(mtx, mtx, entry->scale.x, entry->scale.y, entry->scale.z * scaleSign);
	}
	if (entry->rotationOffset.x != 0.0f || entry->rotationOffset.y != 0.0f || entry->rotationOffset.z != 0.0f) {
		MTXTransApply(mtx, mtx, -entry->rotationOffset.x, -entry->rotationOffset.y, -entry->rotationOffset.z);
	}
	if (entry->rotation.z != 0.0f) {
		MTXRotRad(mtxZ, 'z', entry->rotation.z * 0.017453292f);
		MTXConcat(mtxZ, mtx, mtx);
	}
	if (entry->rotation.x != 0.0f) {
		MTXRotRad(mtxX, 'x', entry->rotation.x * 0.017453292f);
		MTXConcat(mtxX, mtx, mtx);
	}
	if (entry->rotation.y != 0.0f || rotationAngle != 0.0f) {
		MTXRotRad(mtxY, 'y', (entry->rotation.y + rotationAngle) * 0.017453292f);
		MTXConcat(mtxY, mtx, mtx);
	}
	if (entry->rotationOffset.x != 0.0f || entry->rotationOffset.y != 0.0f || entry->rotationOffset.z != 0.0f) {
		MTXTransApply(mtx, mtx, entry->rotationOffset.x, entry->rotationOffset.y, entry->rotationOffset.z);
	}
	if (history == -1) {
		MTXTransApply(mtx, mtx, entry->position.x, entry->position.y, entry->position.z);
	}
	else {
		MTXTransApply(mtx, mtx,
			entry->positionHistory[history].x,
			entry->positionHistory[history].y,
			entry->positionHistory[history].z);
	}
}

void npcDisp_xlu(CameraId cameraId, void* param) {
	NpcEntry* entry = param; //cast to proper type


}

void npcDisp(CameraId cameraId, void* param) {
	NpcEntry* entry = param; //cast to proper type


}

void npcDisp_offscreen_xlu(CameraId cameraId, void* param) {
	NpcEntry* entry = param; //cast to proper type


}

void npcDisp_offscreen(CameraId cameraId, void* param) {
	NpcEntry* entry = param; //cast to proper type


}

void npcDisp_blur(CameraId cameraId, void* param) {
	NpcEntry* entry = param; //cast to proper type


}

void npcInit(void) { //1:1
	FieldBattleData* data;
	NpcWork* wp;

	wp = &work.field;
	wp->npcMaxCount = NPC_FIELD_MAX_COUNT;
	wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(NpcEntry) * wp->npcMaxCount);
	memset(wp->entries, 0, sizeof(NpcEntry) * wp->npcMaxCount);

	release_wp = __memAlloc(HEAP_DEFAULT, sizeof(NpcFiledEntry) * wp->npcMaxCount);

	wp = &work.battle;
	wp->npcMaxCount = NPC_BATTLE_MAX_COUNT;
	wp->entries = __memAlloc(HEAP_DEFAULT, sizeof(NpcEntry) * wp->npcMaxCount);
	memset(wp->entries, 0, sizeof(NpcEntry) * wp->npcMaxCount);

	data = __memAlloc(HEAP_DEFAULT, sizeof(FieldBattleData));
	gp->mpFieldBattleData = data;
	memset(gp->mpFieldBattleData, 0, sizeof(FieldBattleData));
	data->mode = 0;
	npcMainCount = 0;
}

void npcReset(BOOL inBattle) {
	NpcWork* wp = npcGetWorkPtr2(inBattle);
	memset(wp->entries, 0, sizeof(NpcEntry) * wp->npcMaxCount);
	wp->npcCount = 0;
	if (!inBattle) {
		gp->mpFieldBattleData->mode = 0;
	}
	npcMainCount = 0;
}

u32 npcGetReactionOfLivingBody(BOOL inBattle) {
	return npcGetWorkPtr2(inBattle)->npcCount;
}

//TODO: fix entry->position delayed set-Vec with r6/r30
s32 npcEntry(const char* a1, const char* animName) {
	NpcWork* wp;
	NpcEntry* entry;
	s32 i;

	wp = gp->isBattleInit ? &work.battle : &work.field;
	//wp = npcGetWorkPtr(); //inlined

	// unused ------------------------------------------------
	for (i = 0; i < wp->npcMaxCount; i++) {
		entry = &wp->entries[i];
		if (entry->mFlags & 1 && !strcmp(entry->field_0x8, a1)) {
			break; //found an entry, unused
		}
	}
	// unused ------------------------------------------------

	if (wp->npcMaxCount > 0) {
		for (i = 0; i < wp->npcMaxCount; i++) {
			entry = &wp->entries[i];
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
	wp->npcCount++;
	return i;
}

NpcTribe* npcGetTribe(const char* tribeName) { //1:1
	NpcTribe* tribe;
	
	for (tribe = npcTribe; tribe->nameJp; tribe++) {
		if (!strcmp(tribe->nameJp, tribeName)) {
			return tribe;
		}
	}
	return NULL;
}

void npcDelete(NpcEntry* entry) {
	if (entry->slaves[0]) {
		entry->slaves[0]->master = NULL;
	}
	entry->slaves[0] = NULL;
	if (entry->slaves[1]) {
		entry->slaves[1]->master = NULL;
	}
	entry->slaves[0] = NULL;
	if (entry->slaves[2]) {
		entry->slaves[2]->master = NULL;
	}
	entry->slaves[0] = NULL;
	if (entry->slaves[3]) {
		entry->slaves[3]->master = NULL;
	}
	entry->slaves[0] = NULL;

	if (entry->prev) {
		if (entry->next) {
			entry->prev->next = entry->next;
			entry->next->prev = entry->prev;
		}
		else {
			entry->prev->next = NULL;
		}
	}
	else {
		if (entry->next) {
			entry->next->prev = NULL;
		}
	}
	entry->mFlags &= ~2;
	if (entry->poseId >= 0) {
		animPoseRelease(entry->poseId);
	}
	entry->poseId = -1;
	entry->mFlags &= ~1;
	npcGetWorkPtrInline()->npcCount--;
}

void _npcDeleteGroup(NpcEntry* entry) { //recursively inlined, warning
	int i;

	if (entry->next) {
		npcDeleteGroup(entry->next);
	}

	for (i = 0; i < 4; i++) {
		if (entry->slaves[i]) {
			npcDelete(entry->slaves[i]);
			entry->slaves[i] = NULL;
		}
	}
	npcDelete(entry);
}

void npcDeleteGroup(NpcEntry* entry) {
	if (entry->master) {
		entry = entry->master;
	}
	while (entry->prev) {
		entry = entry->prev;
	}
	_npcDeleteGroup(entry);
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

NpcEntry* npcNameToPtr(const char* name) {
	NpcWork* wp = npcGetWorkPtrInline();
	NpcEntry* entry;
	int i;

	for (i = 0; i < wp->npcMaxCount; i++) {
		entry = &wp->entries[i];
		if (entry->mFlags & 1 && !strcmp(entry->field_0x8, name)) {
			break;
		}
	}
	return entry;
}












FieldBattleData* fbatGetPointer(void) {
	return gp->mpFieldBattleData;
}

void fbatChangeMode(u16 mode) {
	gp->mpFieldBattleData->mode = mode;
}

void fbatSetAttackAnnounce(s32 a1) {
	FirstStrikeInfo* firstStrike = &gp->mpFieldBattleData->firstStrike;
	
	firstStrike->enabled = FALSE;
	firstStrike->field_0x4 = a1;
	firstStrike->field_0xC = 0;
	switch (firstStrike->field_0x4) {
		case 0x20000:
		case 0x40000:
		case 0x80000:
		case 0x100000:
		case 0x200000:
		case 0x400000:
		case 0x800000:
		case 0x1000000:
			firstStrike->msg_tag = "fb_sensei_shita"; //You struck first!
			firstStrike->type = 2;
			firstStrike->color = (GXColor){0xFF, 0x80, 0x80, 0xFF};
			break;
		case 0x10000000:
			firstStrike->msg_tag = "fb_sensei_sareta"; //Your foe struck first!
			firstStrike->type = 1;
			firstStrike->color = (GXColor){0x80, 0x80, 0xFF, 0xFF};
			break;
		default:
			firstStrike->type = 0;
			break;
	}
}

void fbatSetAttackAnnounceEnable(void) {
	gp->mpFieldBattleData->firstStrike.enabled = TRUE;
}

void _fbatFirstAttackAnnounceDisp(CameraId cameraId, void* param) {
	FirstStrikeInfo* firstStrike = param;
	const char* message;

	message = msgSearch(firstStrike->msg_tag);
}


