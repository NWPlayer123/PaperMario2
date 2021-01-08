#pragma once

#include "evtmgr_cmd.h"
#include "mariost.h"
#include <dolphin/os.h>

typedef u32 EvtScriptCode;

struct EvtEntry {
	OSTime timeSinceStart; //0x0
	u8 flags; //0x8
	u8 paramCount; //0x9
	EvtOpcode opcode; //0xA
	u8 priority; //0xB
	u8 typeMask; //0xC
	u8 sleeping; //0xD, bool
	u8 loopStackIndex; //0xE
	u8 switchStackIndex; //0xF
	u8 wNpcEventType; //0x10
	u8 pad_11[3]; //0x11
	EvtScriptCode* nextCmdPtr; //0x14
	EvtScriptCode* currCmdArgs; //0x18
	u8 labelIdTable[16]; //0x1C
	void* labelAddressTable[16]; //0x2C
	EvtEntry* waitingEvt; //0x6C
	EvtEntry* waitingOnEvt; //0x70
	EvtEntry* parentEvt; //0x74, TODO prevBrotherEvt?
	u8 userData[12]; //0x78, TODO check
	u32 wInterpolationStartedNpcFlag; //0x84
	OSTime wInterpRelatedTime; //0x88
	u32 field_0x90[2]; //0x90, unknown
	u32 currentSleepingFuncPtr; //0x98, TODO rename/retype
	s32 lwData[16]; //0x9C
	u8 lfData[12]; //0xDC
	u32 field_0xE0[2]; //0xE0, unknown
	void* loopStartAddressStack[8]; //0xE8, TODO rename
	u32 loopIterationsLeftStack[8]; //0x108, TODO rename
	u8 switchStateStack[8]; //0x128, TODO retype/rename?
	u32 switchValueStack[8]; //0x130, TODDO retype/rename?
	void* memoryCmdBase; //0x150
	s32* uwBase; //0x154
	u8* ufBase; //0x158
	u32 threadId; //0x15C
	void* wActorThisPtr; //0x160, TODO rename
	f32 speed; //0x164, number of instructions per frame
	OSTick timeScheduledToRun; //0x168, TODO rename/retype
	u32 caseId; //0x16C
	void* wThisPtr; //0x170, TODO rename
	void* wThisObjPtr; //0x174, TODO rename
	u32 wActiveMsgWindowId; //0x178
	u32 field_0x17C[5]; //0x17C
	u32 msgPriority; //0x190
	u32 field_0x194; //0x194
	OSTime wInterpolationStartTime2; //0x198, TODO rename
	void* restartFromLocation; //0x1A0, TODO rename
	char* name; //0x1A4
	void* wCurrentCommandPtr; //0x1A8
	u32 field_0x1AC; //0x1AC
};

typedef struct evtWork {
	s32 entryCount; //0x0
	s32 gwData[32]; //0x4
	u8 gfData[12]; //0x84
	EvtEntry* entries; //0x90
	u32 field_0x94; //0x94
	OSTime currentEvtTime; //0x98
} evtWork;

//u32 test = sizeof(EvtEntry);

evtWork* evtGetWork(void);