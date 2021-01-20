#include "drv/npcdrv.h"
#include "drv/animdrv.h"
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


FieldBattleData* fbatGetPointer(void) {
	return gp->mpFieldBattleData;
}

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



void npcReleaseFiledNpc(void) {
	AnimPose* pose;
	NpcEntry* entry;
	int i;

	if (work.field.wFlags & 2) {
		for (i = 0; i < work.field.npcMaxCount; i++) {
			entry = &work.field.entries[i];
			if (entry->mFlags & 1 &&
				entry->mFlags & 2 &&
				animPoseGetAnimPosePtr(entry->poseId)->mEffectPoseIdx == -1) {
				if (entry->poseId >= 0) {
					pose = animPoseGetAnimPosePtr(entry->poseId);
					release_wp[i].mFlags = pose->mFlags;
					release_wp[i].wCurrentRenderRotationMinus90Y = pose->field_0x70;
					release_wp[i].wRotationY = pose->mRotationY;
					release_wp[i].wScaleZ_flip = pose->field_0x78;
					release_wp[i].wInitToZero4 = pose->field_0x7C;
					strcpy(release_wp[i].mModelName, animPoseGetAnimBaseDataPtr(entry->poseId)->mFileName);
					strcpy(release_wp[i].mAnimName, animPoseGetCurrentAnim(entry->poseId)->mAnimName);
					//animPoseRelease(entry->poseId);
					entry->poseId = -1;
				}
			}
		}
	}
}

//usually inlined
NpcWork* npcGetWorkPtr(void) {
	return gp->isBattleInit ? &work.battle : &work.field;
}