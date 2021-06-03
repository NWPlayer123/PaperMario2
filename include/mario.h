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
typedef struct MarioWork {
	u32 flags; //0x0
	u32 dispFlags; //0x4
	u32 field_0x8; //0x8
	u32 trigFlags; //0xC
	u8 field_0xC[0x18 - 0xC]; //0xC
	char* animName; //0x18
	u8 field_0x1C[0x2E - 0x1C]; //0x1C
	MarioMotion currMotionId; //0x2E
	MarioMotion prevMotionId; //0x30, TODO check
	u8 field_0x32[0x3C - 0x32]; //0x32
	s8 characterId; //0x3C
	s8 colorId; //0x3D, verified
	u8 field_0x3E[0x44 - 0x3E]; //0x3E
	u32 currSubMotionId; //0x44
	u8 field_0x48[0x7A - 0x48]; //0x48
	s16 field_0x7A; //0x7A
	u8 field_0x7C[0x8C - 0x7C]; //0x7C
	Vec position; //0x8C
	u8 field_0x98[0x134 - 0x98]; //0x98
	s32 camId; //0x134
	u8 field_0x138[0x184 - 0x138]; //0x138
	f32 mBaseWalkSpeed; //0x184
	f32 mBaseDashSpeed; //0x188, same on US
	u8 field_0x18C[0x1B4 - 0x18C]; //0x18C
	Vec wPlayerCollisionBox; //0x1B4 JP, 0x1B8 US
	u8 field_0x1C0[0x1D8 - 0x1C0]; //0x1C0
	s32 field_0x1D8; //0x1D8
	u8 field_0x1DC[0x288 - 0x1DC]; //0x1DC
	void* motStruct; //0x288, motion userdata, TODO rename?
	u8 field_0x28C[0x2A8 - 0x28C]; //0x28C
	f32 wCamVal1; //0x2A8, TODO: double check
	u8 field_0x2AC[0x2E0 - 0x2AC]; //0x2AC
} MarioWork; //size 0x2E0 on Japan

MarioWork* marioGetPtr(void);
s32 marioGetColor(void);
void marioSetCharMode(s32 mode);
void marioSetFamicomMode(s32 mode);
void marioSetSpec(void);
void marioEntry(void);
void marioItemGetDisable(void);
void marioItemGetChk(void);
BOOL marioItemGetOk(void);
BOOL marioCaseEventValidChk(void);
BOOL mario8005BB80(void);
BOOL marioCheckMenuDisable(void);


void marioInit(void);
void marioMain(void);