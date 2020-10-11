#pragma once

#include <dolphin/types.h>
#include <string.h>
#include "drv/msgdrv.h"

typedef struct PouchPartyData {
	u16 flags;
	s16 max_hp;
	s16 base_max_hp;
	s16 current_hp;
	s16 hp_level;
	s16 attack_level;
	s16 tech_level;
} PouchPartyData;

typedef struct mario_pouch {
	PouchPartyData party_data[8]; //0x0 - 0x70
	u8 pad70[0x31A];
	s16 equipped_badges[200];
	u8 pad51a[0x66];
	u32 e_mail_received[4];
	u32 e_mail_read[4];
	u8 pad5a0[0x20];
	char yoshi_name[16];
	u32 something;
} mario_pouch;

mario_pouch* mpp;