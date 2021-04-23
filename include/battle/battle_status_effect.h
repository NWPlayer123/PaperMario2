#pragma once

#include <dolphin/types.h>
#include "battle/battle_unit.h"

void BattleStatusEffectInit(BattleWorkUnit* unit);
void BattleStatusEffectMain(BattleWorkUnit* unit);
void BSE_TurnFirstProcessEffectEntry(BattleWorkUnit* unit, s16 active);
BOOL BSE_TurnFirstProcessEffectMain(BattleWorkUnit* unit);
void BattleStatusChangeInfoWorkInit(BattleWorkUnit* unit);








void BattleStatusChangeMsgWorkInit(void);