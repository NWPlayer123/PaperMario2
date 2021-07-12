#pragma once

#include <dolphin/types.h>
#include "battle/battle.h"

void BattleInformationSetMode(FieldBattleInfo* info, u32 mode);
void BattleInformationSetParty(FieldBattleInfo* info, BattleUnitType party);
void BattleInformationSetFirstAttack(FieldBattleInfo* info, u32 partyId);
void BattleInformationInit(FieldBattleInfo* info);
void BattleInfomationSetBattleSetupInfo(FieldBattleInfo* info, NpcBattleInfo* setup);
void BattleInformationSetResult(FieldBattleInfo* info, u32 result);
u32 BattleInformationGetResult(FieldBattleInfo* info);


