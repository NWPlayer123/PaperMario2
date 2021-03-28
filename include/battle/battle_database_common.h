#pragma once

#include <dolphin/types.h>

typedef struct BattleWeapon {
	u8 field_0x0[0xB0 - 0x0]; //0x0
	void* mAttackEvtScript; //0xB0
	u8 field_0xB4[0xC0 - 0xB4]; //0xB4
} BattleWeapon;