#include "battle/battle_camera.h"
#include "battle/battle.h"
#include <string.h>

extern BattleWork* _battleWorkPointer;

void battleCameraInit(void) {
	BattleWorkCamera* camera;

	camera = &_battleWorkPointer->mCameraWork;
	memset(camera, 0, sizeof(BattleWorkCamera));
	camera->mFlags = 0;
	switch (_battleWorkPointer->mFieldBattleInfo->partyId) {
		case 0:
			camera->mMode = 14;
			break;
		//TODO: figure out ... with compiler
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			camera->mMode = 15;
		case 9:
			camera->mMode = 16;
			break;
	}
	camera->mPriLimit = 0;
	camera->mZoom = 0.0f;
	camera->mMoveSpeedLv = 1;
	camera->mZoomSpeedLv = 1;
	camera->mPosOffset.x = 0.0f;
	camera->mPosOffset.y = 0.0f;
	camera->mPosOffset.z = 0.0f;
	camera->field_0x4 = 0;
}