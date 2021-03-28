#include "evt/evt_badgeshop.h"
#include "mgr/evtmgr_cmd.h"
#include "system.h"
#include <string.h> //for memset

//TODO: double check these need trailing zero on compilation
//.data
u32 badge_starmaniac_table[] = {
	0x13E, 0x140, 0x12D, 0x124, 0x126, 0x125, 0x13A, 0x137, 0x138, 0x135, 0x134, 0x119, 0x11A, 0x104, 0x105, 0
};

u32 badge_bottakuru_table[] = {
	0x82, 0x85, 0x86, 0x88, 0x8C, 0x96, 0x9C, 0x98, 0x120, 0x121, 0x122, 0x13D, 0x143, 0x10A, 0x136, 0x148, 0
};

//.bss
badgeshopw badgeshopwork;

//.sdata
u32 badge_special_table[] = {
	0xFA, 0xF0, 0x144, 0x145, 0x129, 0x110, 0x111, 0x126, 0x102, 0
};

u32 badge_special_table_stage_1_clear[] = {
	0x12B, 0x12C, 0xF5, 0
};

u32 badge_special_table_stage_2_clear[] = {
	0x141, 0x12E, 0x10C, 0
};

u32 badge_special_table_stage_3_clear[] = {
	0xF4, 0xFD, 0x10D, 0
};

u32 badge_special_table_stage_4_clear[] = {
	0xF6, 0xFB, 0x114, 0
};

u32 badge_special_table_stage_5_clear[] = {
	0x103, 0x115, 0xFC, 0
};

u32 badge_special_table_stage_6_clear[] = {
	0x144, 0x145, 0x13F, 0
};

u32 badge_bottakuru100_table[] = {
	0x94, 0x95, 0x9A, 0x9B, 0x84, 0x81, 0
};

u32 badge_bteresa_table[] = {
	0x95, 0xAC, 0x102, 0x131, 0
};

u32 badge_bteresa_table_card_special[] = {
	0x9B, 0xF0, 0x116, 0x118, 0
};

u32 badge_bteresa_table_card_silver[] = {
	0x7F, 0xF7, 0x117, 0
};

u32 badge_bteresa_table_card_gold[] = {
	0xF9, 0x10C, 0xF3, 0xF1, 0x10D, 0
};

u32 badge_bteresa_table_card_platinum[] = {
	0xF8, 0x139, 0x96, 0x9C, 0
};

//.sbss
badgeshopw* bdsw;

s32 bottakuruComp(void* entry1, void* entry2);

//re-typed get/set/add to unsigned, would have to set back to signed for 1:1
s32 badgeShop_add(u8* table, s16 id, s32 val) {
	s32 bound;
	u16 index;
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

void badgeShop_bargainGeneration(void) {

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
				evtSetValue(0, i + GSW(118), j);
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
	bdsw->field_0x114 = 1;

	for (ptr = badge_special_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->special_table, (s16)*ptr, 1);
	}
	for (ptr = badge_starmaniac_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->starmaniac_table, (s16)*ptr, 1);
	}
	for (ptr = badge_bottakuru_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->bottakuru_table, (s16)*ptr, 1);
	}

	badgeShop_bottakuruGeneration();

	for (ptr = badge_bteresa_table; *ptr; ++ptr) {
		badgeShop_add(bdsw->bteresa_table, (s16)*ptr, 1);
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