#pragma once

#include "mgr/evtmgr_cmd.h"
#include "mariost.h"
#include <dolphin/os.h>

//TODO: s32, see evtRestart
typedef struct EvtScriptCode {
	s16 param_count;
	u16 opcode;
	u32 label;
} EvtScriptCode;

typedef EvtStatus (*UserFunction)(EvtEntry* evt, BOOL blocked);

struct EvtEntry {
	OSTime timeSinceStart; //0x0
	u8 flags; //0x8, TODO mFlags?
	u8 paramCount; //0x9
	EvtOpcode opcode; //0xA
	u8 priority; //0xB
	u8 typeMask; //0xC
	u8 blocked; //0xD, TODO re-type "bool" not BOOL
	s8 loopDepth; //0xE
	s8 switchStackIndex; //0xF
	u8 wNpcEventType; //0x10
	u8 pad_11[3]; //0x11
	s32* wNextCmdPtr; //0x14
	s32* currCmdArgs; //0x18, TODO retype?
	s8 labelIdTable[16]; //0x1C
	s32* labelAddressTable[16]; //0x2C
	EvtEntry* waitingEvt; //0x6C
	EvtEntry* waitingOnEvt; //0x70
	EvtEntry* parentEvt; //0x74, TODO prevBrotherEvt?
	s32 userData[3]; //0x78, TODO check
	u32 wInterpolationStartedNpcFlag; //0x84
	OSTime wInterpRelatedTime; //0x88
	u32 field_0x90[2]; //0x90, unknown
	UserFunction user_func; //0x98
	s32 lwData[16]; //0x9C
	u32 lfData[3]; //0xDC, TODO double check u32[3] vs 0xE0, 0xE4
	//u32 field_0xE0[2]; //0xE0, unknown
	void* loopStartTable[8]; //0xE8, TODO rename
	s32 loopCounterTable[8]; //0x108
	u8 switchStateStack[8]; //0x128, TODO retype/rename?
	u32 switchValueStack[8]; //0x130, TODDO retype/rename?
	void* memoryCmdBase; //0x150
	s32* uwBase; //0x154
	s32* ufBase; //0x158
	s32 threadId; //0x15C
	void* wActorThisPtr; //0x160, TODO rename
	f32 speed; //0x164, number of instructions per frame
	f32 timeScheduledToRun; //0x168, TODO rename/retype
	u32 caseId; //0x16C
	void* wThisPtr; //0x170, TODO rename
	void* wThisObjPtr; //0x174, TODO rename
	u32 wActiveMsgWindowId; //0x178
	u32 field_0x17C; //0x17C
	u32 field_0x180; //0x180
	u32 field_0x184; //0x184
	u32 field_0x188; //0x188
	u32 field_0x18C; //0x18C
	u32 msgPriority; //0x190
	u32 field_0x194; //0x194
	OSTime wInterpolationStartTime2; //0x198, TODO rename
	void* restartFromLocation; //0x1A0, TODO rename
	const char* name; //0x1A4
	void* wCurrentCmdPtr; //0x1A8
	u32 field_0x1AC; //0x1AC
};

typedef struct evtWork {
	s32 entryCount; //0x0
	s32 gwData[32]; //0x4
	u32 gfData[3]; //0x84
	EvtEntry* entries; //0x90
	u32 field_0x94; //0x94
	OSTime currentEvtTime; //0x98
} evtWork;

//u32 test = sizeof(EvtEntry);

EvtEntry* evtGetPtrID(s32 threadId);
EvtEntry* evtGetPtr(s32 index);
void evtStartOther(EvtEntry* evt, u8 typeMask);
void evtStopOther(EvtEntry* evt, u8 typeMask);
void evtStartAll(u8 typeMask);
void evtStopAll(u8 typeMask);
void evtStartID(s32 threadId);
void evtStopID(s32 threadId);
void evtStart(EvtEntry* evt, u8 typeMask);
void evtStop(EvtEntry* evt, u8 typeMask);
void evtSetType(EvtEntry* evt, u8 typeMask);
void evtSetSpeed(EvtEntry* evt, f32 speed);
void evtSetPri(EvtEntry* evt, u8 priority);
BOOL evtCheckID(s32 threadId);
void evtDeleteID(s32 threadId);
void evtDelete(EvtEntry* evt);
void evtmgrMain(void);
EvtEntry* evtRestart(EvtEntry* evt);

evtWork* evtGetWork(void);