#pragma once

#include <dolphin/types.h>

#define NPC_FIELD_MAX_COUNT 36
#define NPC_BATTLE_MAX_COUNT 16

typedef struct NpcEntry NpcEntry;

struct NpcEntry {
	u32 mFlags; //0x0
	u8 field_0x4[0x104 - 0x4]; //0x4
	s32 poseId; //0x104
	u8 field_0x108[0x32C - 0x108]; //0x108
	NpcEntry* master; //0x32C
	NpcEntry* slaves[4]; //0x330
};

typedef struct NpcFiledEntry {
	char mModelName[64]; //0x0
	char mAnimName[64]; //0x40
	u32 mFlags; //0x80
	f32 wCurrentRenderRotationMinus90Y; //0x84, TODO better name
	f32 wRotationY; //0x88, TODO better name
	f32 wScaleZ_flip; //0x8C, TODO better name
	f32 wInitToZero4; //0x90, TODO better name
} NpcFiledEntry;

typedef struct NpcWork {
	u32 npcCount; //0x0
	u32 npcMaxCount; //0x4
	u32 wFlags; //0x8
	NpcEntry* entries; //0xC
	NpcEntry* wTalkCheckRelatedNpc; //0x10
} NpcWork;

typedef struct FieldBattleInfo {
	u8 field_0x0[0x1C - 0]; //0x0
} FieldBattleInfo;

typedef struct FieldBattleData {
	u16 mMode; //0x0
	u8 field_0x2[0x580 - 0x2]; //0x2
} FieldBattleData;

FieldBattleData* fbatGetPointer(void);