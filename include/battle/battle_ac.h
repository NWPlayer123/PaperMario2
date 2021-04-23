#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"

void BattleActionCommandManagerInit(BattleWork* work);
void BattleActionCommandManager(BattleWork* work);
s32 BattleActionCommandResult(BattleWork* work);
void BattleActionCommandDeclareACResult(BattleWork* work, BattleWeapon* weapon, s32 result);

void BattleActionCommandStart(BattleWork* work);
void BattleActionCommandStop(BattleWork* work);
s32 BattleActionCommandGetDefenceResult(void);
void BattleActionCommandResetDefenceResult(void);
s8 BattleActionCommandGetDifficulty(BattleWork* work);




