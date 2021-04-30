#include "mario.h"
#include <string.h>
//TODO: put in folder?

mario_work* mp;

void marioInit(void) {

}

void marioMain(void) {

}

void marioItemGetDisable(void) {
	mp->flags |= 0x4000;
	mp->field_0x7A = 0x14;
}

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

void marioSetFamicomMode(s32 mode) {

}

void marioSetCharMode(s32 mode) {

}

s32 marioGetColor(void) {
	return (s32)mp->colorId;
}

mario_work* marioGetPtr(void) {
	return mp;
}