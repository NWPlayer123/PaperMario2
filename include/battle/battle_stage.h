#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"
#include "battle/battle_stage_object.h"

typedef struct BattleStageData {
	char* mGlobalStageDataDir; //0x0
	char* mSpecificStageDataDir; //0x4
	u32 mNumObjects; //0x8
	BattleStageObjectData* mObjects; //0xC, TODO extern?
} BattleStageData;




void BattleStageInit(void);
void BattleStageMain(void);
void BattleStageEnd(void);



