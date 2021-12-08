#pragma once

#include <dolphin/mtx.h>
#include <dolphin/os.h>
#include "mario/mario_pouch.h"

#define NPC_FIELD_MAX_COUNT 36
#define NPC_BATTLE_MAX_COUNT 16

typedef enum NpcTerritoryType {
	TERRITORY_NONE = 0,
	TERRITORY_CIRCLE = 1,
	TERRITORY_SQUARE = 2
} NpcTerritoryType;

typedef struct NpcEntry NpcEntry;

typedef struct NpcBattleInfo {
	u8 field_0x0[0x5C - 0x0]; //0x0
	ItemType stolenItems[8]; //0x5C
	ItemType recoveredItems[8]; //0x7C
	u8 field_0x9C[0xC8 - 0x9C]; //0x9C
} NpcBattleInfo;

typedef struct NpcTribe {
	const char* nameJp; //0x0
	const char* modelName; //0x4
	const char* initialAnimation; //0x8, TODO double check
	const char* stopAnimation; //0xC
	const char* stayAnimation; //0x10
	const char* talkAnimation; //0x14
	const char* walkAnimation; //0x18
	const char* runAnimation; //0x1C
	const char* damageAnimation; //0x20
	const char* confuseAnimation; //0x24
	f32 width; //0x28, TODO double check
	f32 height; //0x2C
	f32 shadowOffsetX; //0x30
	f32 shadowOffsetY; //0x34
	f32 shadowOffsetZ; //0x38
	f32 runStartSpeed; //0x3C
	const char* moveLeftSfxName; //0x40
	const char* moveRightSfxName; //0x44
	u16 field_0x48; //0x48, something with MusyX
	//pad two bytes
	const char* jumpSfxName; //0x4C
	const char* landingSfxName; //0x50
} NpcTribe;

struct NpcEntry {
	u32 flags; //0x0
	u32 reactionFlags; //0x4
	char description[32]; //0x8
	NpcTribe* tribe; //0x28
	char currentAnimation[32]; //0x2C
	char stayAnimation[32]; //0x4C
	char talkAnimation[32]; //0x6C
	Vec position; //0x8C
	Vec prevPosition; //0x98
	Vec positionHistory[5]; //0xA4
	Vec scale; //0xE0
	Vec rotation; //0xEC
	Vec rotationOffset; //0xF8
	s32 poseId; //0x104
	s32 field_0x108; //0x108
	const char* field_0x10C; //0x10C
	s32 field_0x110; //0x110
	GXColor color; //0x114
	s32 initEvtId; //0x118
	s32 regularEvtId; //0x11C
	void* initEvt; //0x120
	void* regularEvt; //0x124
	void* talkEvt; //0x128
	void* deadEvt; //0x12C
	void* findEvt; //0x130
	void* lostEvt; //0x134
	void* returnEvt; //0x138
	void* blowEvt; //0x13C
	u8 field_0x140[0x14C - 0x140]; //0x140
	f32 width; //0x14C
	f32 height; //0x150
	f32 field_0x154; //0x154
	Vec jumpStartPos; //0x158
	Vec jumpTargetPos; //0x164
	u8 field_0x170[0x178 - 0x170]; //0x170
	OSTime field_0x178; //0x178
	f32 field_0x180; //0180
	u8 field_0x184[0x188 - 0x184]; //0x184
	OSTime field_0x188; //0x188
	OSTime field_0x190; //0x190
	OSTime field_0x198; //0x198
	f32 field_0x1A0; //0x1A0
	f32 field_0x1A4; //0x1A4
	f32 field_0x1A8; //0x1A8
	u8 field_0x1AC[0x1C8 - 0x1AC]; //0x1AC
	f32 field_0x1C8; //0x1C8
	f32 field_0x1CC; //0x1CC
	f32 field_0x1D0; //0x1D0
	s32 wJumpFlags; //0x1D4, TODO verify
	u8 field_0x1D8[0x1F4 - 0x1D8]; //0x1D8
	s32 cameraId; //0x1F4, CameraId
	NpcTerritoryType territoryType; //0x1F8
	Vec territoryBase; //0x1FC
	Vec territoryLoiter; //0x208
	Vec territoryHoming; //0x214, TODO: double check naming
	f32 searchRange; //0x220
	f32 searchAngle; //0x224
	f32 homingRange; //0x228
	f32 homingAngle; //0x22C
	NpcBattleInfo battleInfo; //0x230
	s32 field_0x2F8; //0x2F8
	u8 field_0x2FC[0x314 - 0x2FC]; //0x2FC
	u8 field_0x314; //0x314
	u8 field_0x315; //0x315
	u8 field_0x316; //0x316
	u8 field_0x317; //0x317
	u8 field_0x318[0x320 - 0x318]; //0x318
	s16 wFbatHitCheckRelated; //0x320
	u8 pad_322[2]; //0x322
	NpcEntry* prev; //0x324
	NpcEntry* next; //0x328
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
	s32 npcCount; //0x0
	u32 npcMaxCount; //0x4
	u32 wFlags; //0x8
	NpcEntry* entries; //0xC
	NpcEntry* wTalkCheckRelatedNpc; //0x10
} NpcWork;

typedef struct FirstStrikeInfo {
	BOOL enabled; //0x0
	u32 field_0x4; //0x4
	u32 type; //0x8
	u32 field_0xC; //0xC
	f32 field_0x10; //0x10
	f32 field_0x14; //0x14
	const char* msg_tag; //0x18
	GXColor color; //0x1C
} FirstStrikeInfo;

typedef struct FieldBattleData {
	u16 mode; //0x0
	u8 field_0x2[0x550 - 0x2]; //0x2
	FirstStrikeInfo firstStrike; //0x550
	u8 field_0x570[0x580 - 0x570]; //0x570
} FieldBattleData;


NpcWork* npcGetWorkPtr(void);
void npcReleaseFiledNpc(void);
void npcRecoveryFiledNpc(void);
void npcInit(void);
void npcReset(BOOL inBattle);
s32 npcGetReactionOfLivingBody(BOOL inBattle);
s32 npcEntry(const char* a1, const char* animName);
NpcTribe* npcGetTribe(const char* tribeName);
void npcDelete(NpcEntry* entry);
void npcDeleteGroup(NpcEntry* entry);
void npcMain(void);
NpcEntry* npcNameToPtr(const char* name);
NpcEntry* npcNameToPtr_NoAssert(const char* name);






FieldBattleData* fbatGetPointer(void);





void npcSetSlave(NpcEntry* npc, NpcEntry* slave, s32 id);
void npcSetBattleInfo(NpcEntry* npc, s32 battleInfoId);
void fbatChangeMode(u16 mode);
