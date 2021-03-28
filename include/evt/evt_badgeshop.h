#pragma once
#include <dolphin/types.h>

typedef struct badgeshopw {
	u8 special_table[0x19]; //0x00
	u8 bargain_table[0x19]; //0x19
	u8 starmaniac_table[0x19]; //0x32
	u8 bottakuru_table[0x55]; //0x4B
	u8 bteresa_table[0x55]; //0xA0
	u8 align[3];
	u32 field_0xF8[5]; //0xF8-0x10C, assumption since unused(?)
	u32 field_0x10C; //0x10C
	u32 field_0x110; //0x110
	u16 field_0x114; //0x114
	u16 field_0x116; //0x116
	u32 field_0x118; //0x118
	u32 field_0x11C; //0x11C
	u32 field_0x120; //0x120
} badgeshopw;

//u32 test = sizeof(badgeshopw);

void badgeShop_init(void);

u8 badgeShop_get(u8* table, u16 id);
u8 badgeShop_set(u8* table, u16 id, u8 val);
s32 badgeShop_add(u8* table, s16 id, s32 val);

u32 getBadgeBteresaTableMaxCount(void);
u32 getBadgeStarmaniacTableMaxCount(void);
u32 getBadgeBottakuru100TableMaxCount(void);

void badgeShop_bargainGeneration(void);
void badgeShop_bottakuruGeneration(void);