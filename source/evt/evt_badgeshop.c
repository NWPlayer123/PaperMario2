#include "evt/evt_badgeshop.h"

void badgeShop_init(void);
u32 badgeShop_add(u8* table, s16 num, s16 smth);
void badgeShop_bottakuruGeneration(void);
void bottakuruComp(void);

u32 badgeShop_add(u8* table, s16 num, s16 smth) { //params are good, r5 is 1/-1 (0xFFFF)
	u8 mask, table_val, r0;
	u32 minval, shift;
	if ((table == bdsw->bottakuru_table) || (table == bdsw->bteresa_table)) {
		minval = 1;
	}
	else {
		minval = 0xF0;
	}
	if ((num < minval) || (num >= 0x153)) return 0;
	num -= minval;
	switch (num & 3) {
		case 0:
			mask = 3;
			shift = 0;
			break;
		case 1:
			mask = 0xC;
			shift = 2;
			break;
		case 2:
			mask = 0x30;
			shift = 4;
			break;
		case 3:
			mask = 0xC0;
			shift = 6;
			break;
	}
	table_val = table[num / 4];
	smth += ((table_val & mask) >> shift);
	if (smth < 0) {
		smth = 0;
	}
	if (smth > 3) {
		smth = 3;
	}
	table[num / 4] = (table_val & ~mask) + (smth << shift);
	return smth;
}

/*void badgeShop_bargainGeneration(void) {
	u32 i;
	u8 table[0x19];
	if ((evtGetValue(0, 0xF5DE0180) >= 0x37) && !evtGetValue(0, 0xF8405C6E)) {
		evtSetValue(0, 0xF8405C6E, 1);
		i = 0;
		while (badge_special_table_stage_1_clear[i] != 0) {
			badgeShop_add(bdsw->special_table, (s16)badge_special_table_stage_1_clear[i], 1);
			i++;
		}
	}
	if ((evtGetValue(0, 0xF5DE0180) >= 0x70) && !evtGetValue(0, 0xF8405C6F)) {
		evtSetValue(0, 0xF8405C6F, 1);
		i = 0;
		while (badge_special_table_stage_2_clear[i] != 0) {
			badgeShop_add(bdsw->special_table, (s16)badge_special_table_stage_2_clear[i], 1);
			i++;
		}
	}
	if ((evtGetValue(0, 0xF5DE0180) >= 0xA4) && !evtGetValue(0, 0xF8405C70)) {
		evtSetValue(0, 0xF8405C70, 1);
		i = 0;
		while (badge_special_table_stage_3_clear[i] != 0) {
			badgeShop_add(bdsw->special_table, (s16)badge_special_table_stage_3_clear[i], 1);
			i++;
		}
	}
	if ((evtGetValue(0, 0xF5DE0180) >= 0xD4) && !evtGetValue(0, 0xF8405C71)) {
		evtSetValue(0, 0xF8405C71, 1);
		i = 0;
		while (badge_special_table_stage_4_clear[i] != 0) {
			badgeShop_add(bdsw->special_table, (s16)badge_special_table_stage_4_clear[i], 1);
			i++;
		}
	}
	if ((evtGetValue(0, 0xF5DE0180) >= 0xFD) && !evtGetValue(0, 0xF8405C72)) {
		evtSetValue(0, 0xF8405C72, 1);
		i = 0;
		while (badge_special_table_stage_5_clear[i] != 0) {
			badgeShop_add(bdsw->special_table, (s16)badge_special_table_stage_5_clear[i], 1);
			i++;
		}
	}
	if ((evtGetValue(0, 0xF5DE0180) >= 0x152) && !evtGetValue(0, 0xF8405C73)) {
		evtSetValue(0, 0xF8405C73, 1);
		i = 0;
		while (badge_special_table_stage_6_clear[i] != 0) {
			badgeShop_add(bdsw->special_table, (s16)badge_special_table_stage_6_clear[i], 1);
			i++;
		}
	}
	if ((evtGetValue(0, 0xF5DE0180) >= 0x175) && !evtGetValue(0, 0xF8405C74)) {
		evtSetValue(0, 0xF8405C74, 1);
		//badges missing here?
	}
	if ((evtGetValue(0, 0xF5DE0180) >= 0x191) && !evtGetValue(0, 0xF8405C75)) {
		evtSetValue(0, 0xF8405C75, 1);
		//badges missing here?
	}
	memset(table, 0, sizeof(table));
	bdsw->field_0xF8 = 0;
	bdsw->field_0xFC = 0;
	bdsw->field_0x100 = 0;
	bdsw->field_0x104 = 0;
	bdsw->field_0x108 = 0;
	bdsw->field_0x10C = 0;
	bdsw->field_0x110 = 0;
}

void badgeShop_bteresaGeneration(void) {
	u32 i;
	//TODO: check what these pouch items are, and unhash the evt values
	if (pouchCheckItem(0x5B) && !evtGetValue(0, 0xF8405C76)) {
		evtSetValue(0, 0xF8405C76, 1);
		i = 0;
		while (badge_bteresa_table_card_special[i] != 0) {
			badgeShop_add(bdsw->bteresa_table, (s16)badge_bteresa_table_card_special[i], 1);
			i++;
		}
	}

	if (pouchCheckItem(0x5E) && !evtGetValue(0, 0xF8405C77)) {
		evtSetValue(0, 0xF8405C77, 1);
		i = 0;
		while (badge_bteresa_table_card_silver[i] != 0) {
			badgeShop_add(bdsw->bteresa_table, (s16)badge_bteresa_table_card_silver[i], 1);
			i++;
		}
	}

	if (pouchCheckItem(0x5D) && !evtGetValue(0, 0xF8405C78)) {
		evtSetValue(0, 0xF8405C78, 1);
		i = 0;
		while (badge_bteresa_table_card_gold[i] != 0) {
			badgeShop_add(bdsw->bteresa_table, (s16)badge_bteresa_table_card_gold[i], 1);
			i++;
		}
	}

	if (pouchCheckItem(0x5C) && !evtGetValue(0, 0xF8405C79)) {
		evtSetValue(0, 0xF8405C79, 1);
		i = 0;
		for (i = 0; badge_bteresa_table_card_platinum[i]; i++) {
			badgeShop_add(bdsw->bteresa_table, (s16)badge_bteresa_table_card_platinum[i], 1);
			i++;
		}
	}
}*/

//TODO: probably fix pseudo-RNG, will prob never be exact to console but w/e I've given up, it's a quirk tm
void badgeShop_bottakuruGeneration(void) {
	u32 i, j, l, m;
	u8 mask, shift;
	//int i;
	s16 k;
	u64 table[0x11];

	for (i = 0; i < 0x11; i++) {
		table[i] = 200Ull;
	}
	l = 0;
	m = 0;
	for (i = 0; i < 0x152; i++) {
		//always_true conditional here, r3 = 1 || 0xF0
		k = i + 1;
		if ((k < 1) || (k >= 0x153)) {
			k = 0;
		}
		else {
			k--;
			switch (k & 3) {
				case 0:
					mask = 3;
					shift = 0;
					break;
				case 1:
					mask = 0xC;
					shift = 2;
					break;
				case 2:
					mask = 0x30;
					shift = 4;
					break;
				case 3:
					mask = 0xC0;
					shift = 6;
					break;
			}
			k >>= 2;
			k = bdsw->bottakuru_table[k] & mask;
			k >>= shift;
		}
		j = 0;
		while (j++ < k) {
			*(u32*)((u32)table + (j * 8) + 4) = irand(100);
			m++;
			*(u32*)((u32)table + (l * 8)) = i + 1;
			l++;
			if (m >= 0x10) break;
		}
		if (m >= 0x10) break;
	}
	/*qqsort(table, 0x11, 8, bottakuruComp);
	k = 0;
	l = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 0x11; j++) {
			if (*(u32*)((u32)table + (k * 8)) == badge_bottakuru_table[l]) {
				evtSetValue(0, i + 0x0A21FE0A, j);
			}
			l++;
		}
		k++;
	}*/
}

#ifndef __INTELLISENSE__
asm void bottakuruComp(void) {
	lwz     r5, 4(r3)
	lwz     r0, 4(r4)
	cmpw    r5, r0
	bge     @0
	li      r3, -1
	blr
	@0:
	xor     r0, r5, r0
	srawi   r3, r0, 1
	and     r0, r0, r5
	subf    r0, r0, r3
	srwi    r3, r0, 31
	blr
}
#endif


void badgeShop_init(void) {
	u32 i;
	bdsw = &badgeshopwork;
	memset(bdsw->special_table, 0, sizeof(bdsw->special_table));
	memset(bdsw->bargain_table, 0, sizeof(bdsw->bargain_table));
	memset(bdsw->starmaniac_table, 0, sizeof(bdsw->starmaniac_table));
	memset(bdsw->bottakuru_table, 0, sizeof(bdsw->bottakuru_table));
	memset(bdsw->bteresa_table, 0, sizeof(bdsw->bteresa_table));
	bdsw->field_0x114 = 1; //u16
	i = 0;
	while (badge_special_table[i] != 0) {
		badgeShop_add(bdsw->special_table, (s16)badge_special_table[i], 1);
		i++;
	}
	i = 0;
	while (badge_starmaniac_table[i] != 0) {
		badgeShop_add(bdsw->starmaniac_table, (s16)badge_starmaniac_table[i], 1);
		i++;
	}
	i = 0;
	while (badge_bottakuru_table[i] != 0) {
		badgeShop_add(bdsw->bottakuru_table, (s16)badge_bottakuru_table[i], 1);
		i++;
	}
	badgeShop_bottakuruGeneration();
	i = 0;
	while (badge_bteresa_table[i] != 0) {
		badgeShop_add(bdsw->bteresa_table, (s16)badge_bteresa_table[i], 1);
		i++;
	}
}

//TODO: un-hardcode all these?
u32 getBadgeBteresaTableMaxCount(void) {
	return 0x14;
}

u32 getBadgeStarmaniacTableMaxCount(void) {
	return 0xF;
}

u32 getBadgeBottakuru100TableMaxCount(void) {
	return 6;
}