#include "mario.h"
#include "party/party_vivian.h"
#include <string.h>
//TODO: put in folder?

//.sbss
MarioWork* mp;

MarioWork* marioGetPtr(void) {
	return mp;
}

s32 marioGetColor(void) {
	return mp->colorId;
}

void marioSetCharMode(s32 mode) {

}

void marioSetFamicomMode(s32 mode) {

}

void marioSetSpec(void) {
	if (mp->characterId == 1) {
		(mp->wPlayerCollisionBox).x = 20.0f;
		(mp->wPlayerCollisionBox).y = 47.0f;
		mp->mBaseWalkSpeed = 1.0f;
		mp->mBaseDashSpeed = 2.0f;
	}
	else if (mp->characterId == 2) {
		mp->mBaseWalkSpeed = 1.0f;
		mp->mBaseDashSpeed = 2.25f;
	}
	else {
		if (mp->flags & 0x2000000) {
			(mp->wPlayerCollisionBox).x = 10.0f;
			(mp->wPlayerCollisionBox).y = 25.9f;
		}
		else {
			(mp->wPlayerCollisionBox).x = 20.0f;
			(mp->wPlayerCollisionBox).y = 37.0f;
		}
		mp->mBaseWalkSpeed = 1.0f;
		mp->mBaseDashSpeed = 2.25f;
	}
}

//TODO: finish
void marioEntry(void) {
	/*
	mp->flags |= 1;
	if (!(mp->dispFlags & 2)) {
		if (!mp->animName || strcmp(mp->animName, "M_S_1")) {

		}
	}
	if (mp->flags & 0x2000000) {
		if (mp->currMotionId == kSlit) {
			mp->flags |= 0x100000;
			marioSlitForceCancel();
		}
	}
	else {
		switch (mp->currMotionId) {
			case kShip:
				marioReInit_ship();
				break;
			case kRoll:
				marioReInit_roll();
				break;
			case kSlit:
				marioReInit_slit();
				break;
			case kYoshi:
				marioRideYoshi();
				break;
			default:
				marioChgMot(kStay);

		}
	}
	*/
}

void marioItemGetDisable(void) {
	mp->flags |= 0x4000;
	mp->field_0x7A = 0x14;
}

void marioItemGetChk(void) {
	MarioWork* work;
	s32 var;

	work = mp;
	var = mp->field_0x7A;
	if (var > 0) {
		var--;
		work->field_0x7A = (s16)var;
		if ((s16)var <= 0) {
			work->field_0x7A = 0;
			mp->flags &= ~0x4000u;
		}
	}
}

BOOL marioItemGetOk(void) {
	return (mp->flags & 0x4000) == 0;
}

BOOL marioCaseEventValidChk(void) {
	if (mp->currMotionId == kPlane) {
		return FALSE;
	}
	else {
		return vivianGetStatus() == 0;
	}
}

BOOL mario8005BB80(void) {
	if (mp->currMotionId == kRoll || mp->flags & 0x100000) {
		return FALSE;
	}
	return TRUE;
}

BOOL marioCheckMenuDisable(void) {
	return TRUE;
}




void marioInit(void) {

}

void marioMain(void) {

}