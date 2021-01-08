#pragma once

#include <dolphin/mtx.h>

#pragma enumsalwaysint off
typedef enum MarioMotion {
	kStay, //0x0
	kWalk, //0x1
	kDash, //0x2
	kJump, //0x3
	kJumpNPC, //0x4
	kJumpSw, //0x5
	kJumpStand, //0x6
	kJump2, //0x7
	kJump3, //0x8
	kJumpSmall, //0x9
	kFall, //0xA
	kFall2, //0xB
	kUpstairs, //0xC
	kLand, //0xD
	kTalk, //0xE
	kGetItem, //0xF
	kHip, //0x10
	kHip2, //0x11
	kHammer, //0x12
	kHammer2, //0x13
	kJabara, //0x14
	kSlit, //0x15
	kRoll, //0x16
	kKaze, //0x17
	kPlane, //0x18
	kShip, //0x19
	kYoshi, //0x1A
	kCloud, //0x1B
	kVivian, //0x1C
	kDokan, //0x1D
	kGrasp, //0x1E
	kDamage, //0x1F
	kDamageToge, //0x20
	kBottomless, //0x21
	kForceReset, //0x22
	kShadow, //0x23
	kPartyUse, //0x24
	kKpaSwim, //0x25
	kKpaPowUp, //0x26
	kKpaPowDown, //0x27
	kDummy, //0x28
	kMotionMax = 0xFFFF
} MarioMotion;
#pragma enumsalwaysint on

//TODO: US struct is bigger, 0x2F8 vs 0x2E0
typedef struct mario_work { //TODO: rename?
	u32 flags; //0x0
	u32 dispFlags; //0x4
	u8 field_0x8[0x18 - 0x8]; //0x8
	char* animName; //0x18
	u8 field_0x1C[0x2E - 0x1C]; //0x1C
	MarioMotion currMotionId; //0x2E
	MarioMotion prevMotionId; //0x30, TODO check
	u8 field_0x32[0x3C - 0x32]; //0x32
	s8 characterId; //0x3C
	s8 colorId; //0x3D
	u8 field_0x3E[0x7A - 0x3E]; //0x3E
	u16 field_0x7A; //0x7A
	u8 field_0x7C[0x184 - 0x7C]; //0x7C
	f32 mBaseWalkSpeed; //0x184
	f32 mBaseDashSpeed; //0x188, TODO check
	u8 field_0x18C[0x1B4 - 0x18C]; //0x18C
	Vec wPlayerCollisionBox; //0x1B4, TODO check, 0x1B8 in US
	u8 field_0x1C0[0x2E0 - 0x1C0]; //0x1C0
} mario_work; //size 0x2E0 on Japan

void marioInit(void);
void marioEntry(void);

mario_work* marioGetPtr(void);
s32 marioGetColor(void);
void marioSetCharMode(s32 mode);
void marioSetFamicomMode(s32 mode);
void marioSetSpec(void);
void marioItemGetDisable(void);