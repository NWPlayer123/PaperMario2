#pragma once

#include "battle/battle.h"

s32 BattleAcrobatStart(BattleWork* work, s32 unitId, s32 windowStartFrame,
						s32 windowEndFrame, s32 endFrame, s32 earlyFrames);
EvtStatus BattleAcrobatMain(BattleWork* work);
EvtStatus BattleAcrobatGetResult(BattleWork* work, s32* result, s32* curFrame);