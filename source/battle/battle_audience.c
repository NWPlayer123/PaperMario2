#include "battle/battle_audience.h"
#include "battle/battle.h"
#include "drv/dispdrv.h"
#include "system.h"
#include <string.h>

extern BattleWork* _battleWorkPointer;
extern int sprintf(char* str, const char* format, ...);

//local prototypes
fileObj* tplRead(const char* path);
BOOL check_exe_phase_evt_status(s32 index, AudienceMemberType member);
void BattleAudienceSettingAudience(void);
void BattleAudienceGuestTPLRead(s32 index, AudienceMemberType member, const char* tplName);
void BattleAudienceCtrlProcess(void);
BOOL BattleAudienceItemOn(s32 index, u16 itemId, s32 numItems);
void BattleAudienceSetThrowItemMax(void);
BattleWorkUnit* BattleAudienceDetectTargetAll(void);
BattleWorkUnit* BattleAudienceDetectTargetPlayer(void);
void BattleAudienceItemCtrlProcess(void);
void BattleAudienceApSrcCtrlProcess(void);
BOOL BattleAudienceApSrcEntry(void);
void BattleAudienceWinCtrlProcess(void);
void BattleAudienceGXInit(void);
void BattleAudienceAnimProcess(void);
void BattleAudienceDispAudience(void);
void BattleAudienceDispItem(void);
void BattleAudienceDispApSrc(void);
void BattleAudienceDispWin(CameraId cameraId, void* param);
void BattleAudience_Entry_Sub(s32 index, u8 type, u8 status);
BOOL BattleAudience_GetAnimEnd(s32 index);








void BattleAudienceSoundNoiseAlways(void);
void BattleAudienceSoundSetVol(s32 index, u8 a2, s32 a3);
void BattleAudienceNumToTarget(void);
void BattleAudience_WinSetActive(u8 active);






fileObj* tplRead(const char* path) {
	return fileAllocf(4, "%s/%s", getMarioStDvdRoot(), path);
}

BattleWorkAudience* BattleAudienceBaseGetPtr(void) {
	return &_battleWorkPointer->mAudienceWork;
}

BattleWorkAudienceMember* BattleAudienceGetPtr(s32 id) {
	return &BattleAudienceBaseGetPtr()->mMembers[id];
}

BattleWorkAudienceItem* BattleAudienceItemGetPtr(s32 id) {
	return &BattleAudienceBaseGetPtr()->mItems[id];
}

BattleWorkAudienceWin* BattleAudienceWinGetPtr(void) {
	return &BattleAudienceBaseGetPtr()->mWindowWork;
}

BattleWorkAudienceSound* BattleAudienceSoundGetPtr(s32 id) {
	return &BattleAudienceBaseGetPtr()->mSounds[id];
}

void BattleAudience_Init(void) {
	BattleWorkAudience* audience;
	BattleWorkAudienceWin* window;
	int i, j, index;
	u8 status;

	audience = BattleAudienceBaseGetPtr();
	window = BattleAudienceWinGetPtr();
	pouchGetPtr(); //useless call
	memset(audience, 0, sizeof(BattleWorkAudience));
	BattleAudienceSettingAudience();
	audience->mNormalAudienceTpl = tplRead("battle/audience/audience_normal.tpl");
	BattleAudienceGuestTPLRead(0, kAudienceLuigi, "audience_luigi.tpl");
	BattleAudienceSetThrowItemMax();

	audience->mTurnEndPhaseEventChance = 0;

	audience->mCurrentAudienceIntCountLeft = 0;
	audience->mCurrentAudienceIntCountRight = 0;
	audience->mCurrentAudienceIntCount = 0;

	for (i = 0, index = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			index = (i * 20) + 10 + j;
			if ((u8)BattleAudience_GetExist(index)) {
				status = BattleAudienceGetPtr(index)->mStatus;
				if (status != 12 && status != 11) {
					audience->mCurrentAudienceIntCountRight++;
				}
			}
		}
	}

	for (i = 0, index = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			index = (i * 20) + j;
			if ((u8)BattleAudience_GetExist(index)) {
				status = BattleAudienceGetPtr(index)->mStatus;
				if (status != 12 && status != 11) {
					audience->mCurrentAudienceIntCountLeft++;
				}
			}
		}
	}

	audience->mCurrentAudienceIntCount = audience->mCurrentAudienceIntCountRight + audience->mCurrentAudienceIntCountLeft;
	window->mAudienceCountDisp = (f32)audience->mCurrentAudienceIntCount;
	BattleAudienceSoundNoiseAlways();
	BattleAudienceSoundSetVol(2, 60, 180);
	audience->mAudienceExcited = 0;
	BattleAudienceNumToTarget();
}

void BattleAudience_Main(void) {
	BattleWork* work;
	BattleWorkAudience* audience;

	work = _battleWorkPointer;
	audience = BattleAudienceBaseGetPtr();
	if (!(audience->mFlags & 0x10000) && work->mBattleFlags & 0x80) {
		BattleAudience_WinSetActive(1);
	}
}

void BattleAudience_ActInit(void) {

}

void BattleAudience_PerAct(void) {

}

BOOL BattleAudience_CheckReaction(void) {
	return FALSE;
}

BOOL check_exe_phase_evt_status(s32 index, AudienceMemberType member) {
	return FALSE;
}

void BattleAudience_PerPhase(u32 value) {

}

BOOL BattleAudience_CheckReactionPerPhase(void) {
	return FALSE;
}

void BattleAudience_Disp(void) {

}

void BattleAudience_End(void) {

}

void BattleAudienceSettingAudience(void) {

}

void BattleAudienceGuestTPLRead(s32 index, AudienceMemberType member, const char* tplName) {
	char path[264]; //TODO: double check

	BattleWorkAudience* audience = BattleAudienceBaseGetPtr();
	if (index < 2) {
		sprintf(path, "battle/audience/%s", tplName);
		audience->mGuestAudienceTpls[index] = tplRead(path);
		audience->mGuestAudienceKinds[index] = member;
	}
}

void BattleAudienceCtrlProcess(void) {

}

BOOL BattleAudienceItemOn(s32 index, u16 itemId, s32 numItems) {
	return FALSE;
}

void BattleAudienceSetThrowItemMax(void) {

}

BattleWorkUnit* BattleAudienceDetectTargetAll(void) {
	return NULL;
}

BattleWorkUnit* BattleAudienceDetectTargetPlayer(void) {
	return NULL;
}

void BattleAudienceItemCtrlProcess(void) {

}

void BattleAudienceApSrcCtrlProcess(void) {

}

BOOL BattleAudienceApSrcEntry(void) {
	return FALSE;
}

void BattleAudienceWinCtrlProcess(void) {

}

void BattleAudienceGXInit(void) {

}

void BattleAudienceAnimProcess(void) {

}

void BattleAudienceDispAudience(void) {

}

void BattleAudienceDispItem(void) {

}

void BattleAudienceDispApSrc(void) {

}

void BattleAudienceDispWin(CameraId cameraId, void* param) {

}

BOOL BattleAudience_Entry(s32 index, AudienceMemberType type, u8 status) {
	return FALSE;
}

void BattleAudience_Entry_Sub(s32 index, u8 type, u8 status) {

}

void BattleAudience_Delete(s32 index) {

}

void BattleAudience_SetAnim(s32 index, int a2, int a3) {

}

BOOL BattleAudience_GetAnimEnd(s32 index) {
	return FALSE;
}

void BattleAudience_GetPosition(s32 index, f32* x, f32* y, f32* z) {
	BattleWorkAudienceMember* member;

	member = BattleAudienceGetPtr(index);
	*x = member->mPosition.x;
	*y = member->mPosition.y;
	*z = member->mPosition.z;
}

void BattleAudience_GetHomePosition(s32 index, f32* x, f32* y, f32* z) {
	BattleWorkAudienceMember* member;

	member = BattleAudienceGetPtr(index);
	*x = member->mHomePosition.x;
	*y = member->mHomePosition.y;
	*z = member->mHomePosition.z;
}

void BattleAudience_GetRotate(s32 index, f32* x, f32* y, f32* z) {
	BattleWorkAudienceMember* member;

	member = BattleAudienceGetPtr(index);
	*x = member->mRotation.x;
	*y = member->mRotation.y;
	*z = member->mRotation.z;
}

void BattleAudience_SetPosition(s32 index, f32 x, f32 y, f32 z) {
	BattleWorkAudienceMember* member;

	member = BattleAudienceGetPtr(index);
	member->mPosition.x = x;
	member->mPosition.y = y;
	member->mPosition.z = z;
}

void BattleAudience_SetRotate(s32 index, f32 x, f32 y, f32 z) {
	BattleWorkAudienceMember* member;

	member = BattleAudienceGetPtr(index);
	member->mRotation.x = x;
	member->mRotation.y = y;
	member->mRotation.z = z;
}

void BattleAudience_SetRotateOffset(s32 index, f32 x, f32 y, f32 z) {
	BattleWorkAudienceMember* member;

	member = BattleAudienceGetPtr(index);
	member->mRotationOffset.x = x;
	member->mRotationOffset.y = y;
	member->mRotationOffset.z = z;
}

void BattleAudience_GetItemOn(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType) {
	BattleWorkAudience* audience;
	BattleWorkAudienceMember* member;
	BattleWorkAudienceItem* item;
	int i;

	audience = BattleAudienceBaseGetPtr();
	for (i = 0; i < 200; i++) {
		member = BattleAudienceGetPtr(i); //TODO: get (0) out of loop and ++?
		//TODO: invert this to multiple break; checks?
		if ((member->mFlags & 1) && !(member->mFlags & 0x100) && !(member->mFlags & 2)) {
			if (member->mItemId != -1) {
				item = BattleAudienceItemGetPtr(member->mItemId);
				if (item->mState <= 4 || item->mState == 7) {
					break; //TODO: better way to handle this compare?
				}
				if (memberId) {
					audience->mItemOnMemberId = i;
					*memberId = i;
				}
				audience->mItemOnMemberPos.x = member->mPosition.x;
				audience->mItemOnMemberPos.y = member->mPosition.y;
				audience->mItemOnMemberPos.z = member->mPosition.z + 3.0f;

				if (x) {
					*x = audience->mItemOnMemberPos.x;
				}
				if (y) {
					*y = audience->mItemOnMemberPos.y;
				}
				if (z) {
					*z = audience->mItemOnMemberPos.z;
				}

				if (itemType) {
					if (member->mItemId >= 0) {
						*itemType = audience->mItems[member->mItemId].mItemType;
					}
					else {
						*itemType = kNullItem;
					}
				}
				//has to pass the 4 checks up top, otherwise we keep looking at all 200 members
				return;
			}
		}
	}
}

//might be inlined, 1 above has additional assignments
void BattleAudience_GetItemOn2(s32* memberId, f32* x, f32* y, f32* z, ItemType* itemType) {
	BattleWorkAudience* audience;
	BattleWorkAudienceMember* member;

	audience = BattleAudienceBaseGetPtr();
	member = BattleAudienceGetPtr(audience->mItemOnMemberId);
	if (memberId) {
		*memberId = audience->mItemOnMemberId;
	}

	if (x) {
		*x = audience->mItemOnMemberPos.x;
	}
	if (y) {
		*y = audience->mItemOnMemberPos.y;
	}
	if (z) {
		*z = audience->mItemOnMemberPos.z;
	}

	if (itemType) {
		if (member->mItemId >= 0) {
			*itemType = audience->mItems[member->mItemId].mItemType;
		}
		else {
			*itemType = kNullItem;
		}
	}
}

void BattleAudience_SetTarget(s32 index) {

}















BOOL BattleAudience_GetExist(s32 id) {
	return FALSE;
}

void BattleAudienceSoundNoiseAlways(void) {

}

void BattleAudienceSoundSetVol(s32 index, u8 a2, s32 a3) {
	BattleWorkAudienceSound* sound;

	sound = BattleAudienceSoundGetPtr(index);
	sound->mFadeStartVolumeMultiplier = sound->mVolumeMultiplier;
	sound->mFadeEndVolumeMultiplier = a2;
	sound->mForceFadeTimer = 0;
	sound->mForceFadeLength = a3;
	sound->mFadeoutTime = 0;
}

void BattleAudienceNumToTarget(void) {

}

void BattleAudience_WinSetActive(u8 active) {
	BattleAudienceWinGetPtr()->mActive = active;
}