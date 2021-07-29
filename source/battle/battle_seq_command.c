#include "battle/battle_seq_command.h"
#include "battle/battle.h"
#include "battle/battle_unit.h"
#include "drv/camdrv.h"
#include "drv/dispdrv.h"

//TODO: move into battle/seq/?

//local prototypes
void BattleDrawEnemyHP(CameraId cameraId, void* param);

void BattleDrawEnemyHP(CameraId cameraId, void* param) {
	BattleWork* wp = param;
	BattleWorkUnit* unit;
	int i;

	camGetPtr(kCam2d); //unused, probably just stored

	for (i = 0; i < 64; i++) {
		unit = wp->mUnits[i];
		if (unit) {
			if (!BtlUnit_CheckStatus(unit, kStatusInstantKill)) {

			}
		}
	}
}