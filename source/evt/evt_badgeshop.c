#include "evt/evt_badgeshop.h"
#include "system.h"
#include <string.h> //for memset

badgeshopw badgeshopwork;
badgeshopw* bdsw;

void badgeShop_bottakuruGeneration(void);
s32 bottakuruComp(void* entry1, void* entry2);

//re-typed get/set/add to unsigned, would have to set back to signed for 1:1
u8 badgeShop_add(u8* table, u16 id, u8 val) {
	u16 bound, index;
	u8 shift;

	if (table == bdsw->bottakuru_table || table == bdsw->bteresa_table) {
		bound = 1;
	}
	else {
		bound = 0xF0;
	}
	if (id < bound || id >= 0x153) return 0;
	index = (u16)(id - bound);
	shift = (u8)((index & 3) << 1);
	val = (u8)(((table[index / 4] & (3 << shift)) >> shift) + val); //TODO: badgeShop_get?
	val &= 3; //can only set two bits, retail is signed
	table[index / 4] = (u8)((table[index / 4] & ~(3 << shift)) + (val << shift));
	return val;
}

u8 badgeShop_get(u8* table, u16 id) {
	u16 bound, index;
	u8 shift;

	if (table == bdsw->bottakuru_table || table == bdsw->bteresa_table) {
		bound = 1;
	} else {
		bound = 0xF0;
	}
	if (id < bound || id >= 0x153) return 0;
	index = (u16)(id - bound);
	shift = (u8)((index & 3) << 1);
	return (u8)((table[index / 4] & (3 << shift)) >> shift);
}

u8 badgeShop_set(u8* table, u16 id, u8 val) {
	u16 bound, index;
	u8 shift;

	if (table == bdsw->bottakuru_table || table == bdsw->bteresa_table) {
		bound = 1;
	} else {
		bound = 0xF0;
	}
	if (id < bound || id >= 0x153) return 0;
	index = (u16)(id - bound);
	shift = (u8)((index & 3) << 1);
	val &= 3; //can only set two bits, retail is signed
	table[index / 4] = (u8)((table[index / 4] & ~(3 << shift)) + (val << shift));
	return val;
}

void badgeShop_bottakuruGeneration(void) {
	s32 table[0x11][2]; //stack+8
	int i, j, dobreak;

	for (i = 0; i < 0x11; i++) { //unrolled in retail
		table[i][1] = 200;
		table[i][0] = 0;
	}

	dobreak = 0;
	for (i = 0; i < 0x152; i++) {
		for (j = 0; j < badgeShop_get(bdsw->bottakuru_table, (u16)(i+1)); j++) {
			table[i][1] = irand(100);
			table[i][0] = i + 1;
			dobreak += 1;
			if (dobreak > 0x11) break;
		}
		if (dobreak > 0x11) break;
	}

	qqsort(table, 0x11, 8, bottakuruComp);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 0x11; j++) {
			if (table[i][0] == badge_bottakuru_table[j]) {
				//evtSetValue(0, i + (-170000000 + 118), j); //169999882
			}
		}
	}
}

s32 bottakuruComp(void* entry1, void* entry2) { //TODO: cleanup?
	s32 val1, val2;
	val1 = *(s32*)((u32)entry1 + 4);
	val2 = *(s32*)((u32)entry2 + 4);
	if (val1 < val2) {
		return -1;
	}
	else {
		return (((val1 ^ val2) >> 1) - ((val1 ^ val2) & val1)) >> 31;
	}
}

void badgeShop_init(void) {
	u32* ptr;

	bdsw = &badgeshopwork;
	memset(bdsw->special_table, 0, sizeof(bdsw->special_table));
	memset(bdsw->bargain_table, 0, sizeof(bdsw->bargain_table));
	memset(bdsw->starmaniac_table, 0, sizeof(bdsw->starmaniac_table));
	memset(bdsw->bottakuru_table, 0, sizeof(bdsw->bottakuru_table));
	memset(bdsw->bteresa_table, 0, sizeof(bdsw->bteresa_table));
	badgeshopwork.field_0x114 = 1;

	for (ptr = badge_special_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->special_table, (u16)*ptr, 1);
	}
	for (ptr = badge_starmaniac_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->starmaniac_table, (u16)*ptr, 1);
	}
	for (ptr = badge_bottakuru_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->bottakuru_table, (u16)*ptr, 1);
	}

	badgeShop_bottakuruGeneration();

	for (ptr = badge_bteresa_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->bteresa_table, (u16)*ptr, 1);
	}
}

u32 getBadgeBteresaTableMaxCount(void) {
	return 20;
}

u32 getBadgeStarmaniacTableMaxCount(void) {
	return 15;
}

u32 getBadgeBottakuru100TableMaxCount(void) {
	return 6;
}