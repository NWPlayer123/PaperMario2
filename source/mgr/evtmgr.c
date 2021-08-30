#include "mgr/evtmgr.h"
#include "evt/evt_msg.h"
#include "memory.h"
#include <string.h>

extern GlobalWork* gp;
extern s32 _mariostSystemLevel;

//.bss
s32 priIDTbl[0x100];
s32 priTbl[0x100];
static evtWork work[2];

//.sbss
BOOL runMainF;
s32 priTblNum;
s32 evtMax;

//.sdata
s32 evtID = 1;
f32 evtSpd = 1.0f;

//local definitions
void make_pri_table(void);
void evtEntryRunCheck(void);

#define evtGetWorkInline() (gp->isBattleInit ? &work[1] : &work[0])
#define evtGetWork2() (gp->isBattleInit ? &work[1] : &work[0])

evtWork* evtGetWork(void) {
	return gp->isBattleInit ? &work[1] : &work[0];
}

void make_pri_table(void) {
	evtWork* wp = evtGetWork2();
	EvtEntry* entry;
	s32 *currTbl, *currId, *nextTbl, *nextId, tempTbl, tempId;
	int i, j, num;

	currTbl = priTbl;
	currId = priIDTbl;
	entry = wp->entries;
	num = 0;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if (entry->flags & 1) {
			*currTbl++ = i;
			num++;
			*currId++ = entry->evtNum;
		}
	}
	priTblNum = num;

	currTbl = priTbl;
	currId = priIDTbl;
	for (i = 0; i < num - 1; i++, currTbl++, currId++) {
		nextTbl = &priTbl[i + 1];
		nextId = &priIDTbl[i + 1];
		if (i + 1 < num) {
			for (j = 0; j < num - (i + 1); j++, nextTbl++, nextId++) {
				if (wp->entries[*currTbl].priority < wp->entries[*nextTbl].priority) {
					//swap the two entries
					tempTbl = *currTbl;
					tempId = *currId;
					*currTbl = *nextTbl;
					*currId = *nextId;
					*nextTbl = tempTbl;
					*nextId = tempId;
				}
			}
		}
	}
}

void evtmgrInit(void) {
	work[0].entryCount = 256;
	work[0].entries = __memAlloc(HEAP_DEFAULT, sizeof(EvtEntry) * work[0].entryCount);
	work[0].currentEvtTime = gp->mAnimationTimeInclBattle;
	memset(work[0].entries, 0, sizeof(EvtEntry) * work[0].entryCount);
	memset(work[0].gwData, 0, sizeof(work[0].gwData));
	memset(work[0].gfData, 0, sizeof(work[0].gfData));

	work[1].entryCount = 256;
	work[1].entries = __memAlloc(HEAP_DEFAULT, sizeof(EvtEntry) * work[1].entryCount);
	work[1].currentEvtTime = gp->mAnimationTimeInclBattle;
	memset(work[1].entries, 0, sizeof(EvtEntry) * work[1].entryCount);
	memset(work[1].gwData, 0, sizeof(work[1].gwData));
	memset(work[1].gfData, 0, sizeof(work[1].gfData));

	evtMax = 0;
	priTblNum = 0;
	runMainF = 0;
	evt_msg_init();
}

void evtmgrReInit(void) {
	evtWork* wp = evtGetWork2();
	wp->currentEvtTime = gp->mAnimationTimeInclBattle;
	memset(wp->entries, 0, sizeof(EvtEntry) * wp->entryCount);
	evtMax = 0;
	runMainF = 0;
	evt_msg_init();
}

void evtEntryRunCheck(void) {
	switch (_mariostSystemLevel) {
		case 1:
			evtStopAll(0x1);
			break;
		case 2:
			evtStopAll(0x2);
			break;
		case 3:
			evtStopAll(0x10);
			break;
		case 4:
			evtStopAll(0xEF);
			break;
	}
}

EvtEntry* evtEntry(void* evtCode, u8 priority, u8 flags) {
	evtWork* wp = evtGetWork2();
	EvtEntry* entry;
	void* data;
	s32 id;
	int i;
	
	//search until we find one not-in-use
	entry = wp->entries;
	for (id = 0, i = 0; i < wp->entryCount; i++, id++, entry++) {
		if (!(entry->flags & 1)) {
			break;
		}
	}
	evtMax++;
	memset(entry, 0, sizeof(EvtEntry));
	entry->flags = flags | 1;
	entry->wNextCmdPtr = evtCode;
	entry->restartFromLocation = evtCode;
	entry->wCurrentCmdPtr = evtCode;
	entry->opcode = OP_InternalFetch;
	entry->waitingEvt = NULL;
	entry->waitingOnEvt = NULL;
	entry->parentEvt = NULL;
	entry->priority = priority;
	entry->evtNum = evtID++;
	entry->field_0x160 = -1;
	entry->loopDepth = -1;
	entry->switchStackIndex = -1;
	entry->typeMask = 0xEF;
	entry->name = NULL;
	entry->speed = evtSpd;
	entry->timeScheduledToRun = 0.0f;
	entry->caseId = -1;
	entry->wThisPtr = NULL;
	entry->timeSinceStart = 0;
	for (i = 0; i < 16; i++) {
		entry->lwData[i] = 0;
	}
	for (i = 0; i < 3; i++) {
		entry->lfData[i] = 0;
	}
	for (i = 0; i < 16; i++) {
		entry->labelIdTable[i] = -1;
		entry->labelAddressTable[i] = 0;
	}

	data = entry->wNextCmdPtr;



	//TODO: finish
	return entry;
}

EvtEntry* evtEntryType(void* evtCode, u8 priority, u8 flags, u8 typeMask) {
	evtWork* wp;
	s32 id;
	EvtEntry* entry;

	int i;

	wp = evtGetWorkInline();
	//very close to evtEntry
	return NULL;
}














/*
EvtEntry* evtRestart(EvtEntry* evt) {
	s32* code;
	s16 param_count;
	u16 opcode;
	u32 label;
	int i;

	evt->wNextCmdPtr = evt->restartFromLocation;
	evt->wCurrentCmdPtr = evt->restartFromLocation;
	evt->opcode = OP_InternalFetch;
	evt->speed = 1.0f;
	evt->timeScheduledToRun = 0.0f;
	evt->switchStackIndex = -1;
	evt->switchStackIndex = -1;

	evt->speed = 1.0f;
	evt->timeScheduledToRun = 0.0f;

	for (i = 0; i < 16; i++) {
		evt->labelIdTable[i] = -1;
		evt->labelAddressTable[i] = NULL;
	}

	code = evt->wNextCmdPtr;
	i = 0; //for label tables
	do {
		opcode = code->opcode;
		param_count = code->param_count;
		label = code->label;
		code = (EvtScriptCode*)((u32)code + 8); //lwzu 4
		code = (EvtScriptCode*)((u32)code + (param_count * 4)); //add param_count
		switch (opcode) {
			case OP_ScriptEnd:
				break; //will exit loop
			case OP_Label:
				evt->labelIdTable[i] = (s8)label;
				evt->labelAddressTable[i] = code;
				i++;
				//no break
			default:
				//some math that doesn't look like it gets used
				break;
		}
	} while (opcode != OP_ScriptEnd);
	//evtEntryRunCheck(evt);
	return evt;
}*/

void evtmgrMain(void) {
	OSTime delta;
	EvtEntry* entry;
	int i, ret, ret2;

	evtWork* wp = evtGetWorkInline();
	runMainF = TRUE; //we're in evtmgrMain

	delta = gp->mAnimationTimeInclBattle - wp->currentEvtTime;
	if (delta < 0) {
		delta = 0;
	}

	if (OSTicksToMilliseconds(delta) > 500) {
		delta = OSMillisecondsToTicks(1000 / gp->mFPS);
	}

	wp->currentEvtTime = gp->mAnimationTimeInclBattle;
	make_pri_table();
	for (i = 0; i < priTblNum; i++) {
		entry = &wp->entries[priTbl[i]];
		if ((entry->flags & 1) && (entry->evtNum == priIDTbl[i]) && !(entry->flags & 0x92)) {
			entry->timeSinceStart += delta;
			entry->timeScheduledToRun += entry->speed; //update num_instructions
			while (entry->timeScheduledToRun >= 1.0f) { //now execute however many commands
				entry->timeScheduledToRun -= 1.0f;
				ret = evtmgrCmd(entry);
				if (ret == 1) {
					ret2 = TRUE;
					break;
				}
				if (ret == -1) break;
			}
			if (ret2) break;
		}
	}
	runMainF = FALSE; //we're done with evtmgrMain for this frame
}

void evtDelete(EvtEntry* evt) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	if (evt->flags & 1) {
		if (evt->waitingOnEvt) {
			evtDelete(evt->waitingOnEvt);
		}

		entry = wp->entries;
		for (i = 0; i < wp->entryCount; i++, entry++) {
			if ((entry->flags & 1) && (entry->parentEvt == evt)) {
				evtDelete(entry);
			}
		}

		entry = evt->waitingEvt;
		if (entry != NULL) {
			entry->flags &= ~0x10;
			entry->waitingOnEvt = NULL;
			for (i = 0; i < 16; i++) {
				entry->lwData[i] = evt->lwData[i];
			}
			for (i = 0; i < 3; i++) {
				entry->lfData[i] = evt->lfData[i];
			}
			entry->field_0x17C = evt->field_0x17C;
			entry->wActiveMsgWindowId = evt->wActiveMsgWindowId;
			entry->field_0x180 = evt->field_0x180;
			entry->field_0x184 = evt->field_0x184;
			entry->field_0x188 = evt->field_0x188;
			entry->field_0x18C = evt->field_0x18C;
			entry->msgPriority = evt->msgPriority;
		}
		evt->flags &= ~1;
		memset(evt, 0, sizeof(EvtEntry));
		evtMax--;
	}
}

void evtDeleteID(s32 evtNum) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry->evtNum == evtNum)) {
			evtDelete(entry);
		}
	}
}

BOOL evtCheckID(s32 evtNum) {
	int i;

	evtWork* wp = evtGetWorkInline();
	EvtEntry* entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry->evtNum == evtNum)) {
			return TRUE;
		}
	}
	return FALSE;
}

void evtSetPri(EvtEntry* evt, u8 priority) {
	evt->priority = priority;
}

void evtSetSpeed(EvtEntry* evt, f32 speed) {
	evt->speed = speed * evtSpd;
}

void evtSetType(EvtEntry* evt, u8 typeMask) {
	evt->typeMask = typeMask;
}

//this entire function gets recursively inlined twice
void evtStop(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	if (evt->waitingOnEvt) {
		evtStop(evt->waitingOnEvt, typeMask);
	}

	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry->parentEvt == evt)) {
			evtStop(entry, typeMask);
		}
	}

	if (evt->typeMask & typeMask) {
		evt->flags |= 2;
	}
}

//this entire function gets recursively inlined twice
void evtStart(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	if (evt->waitingOnEvt) {
		evtStart(evt->waitingOnEvt, typeMask);
	}

	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry->parentEvt == evt)) {
			evtStart(entry, typeMask);
		}
	}

	if (evt->typeMask & typeMask) {
		evt->flags &= ~2;
	}
}

void evtStopID(s32 evtNum) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry->evtNum == evtNum)) {
			evtStop(entry, 0xEF);
		}
	}
}

void evtStartID(s32 evtNum) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry->evtNum == evtNum)) {
			evtStart(entry, 0xEF);
		}
	}
}

void evtStopAll(u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if (entry->flags & 1) {
			evtStop(entry, typeMask);
		}
	}
}

void evtStartAll(u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if (entry->flags & 1) {
			evtStart(entry, typeMask);
		}
	}
}

void evtStopOther(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry != evt)) {
			evtStop(entry, typeMask);
		}
	}
}

void evtStartOther(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry != evt)) {
			evtStart(entry, typeMask);
		}
	}
}

EvtEntry* evtGetPtr(s32 index) {
	evtWork* wp = evtGetWorkInline();
	EvtEntry* entry = &wp->entries[index];
	if (entry->flags & 1) {
		return entry;
	}
	return NULL;
}

EvtEntry* evtGetPtrID(s32 evtNum) {
	int i;

	evtWork* wp = evtGetWorkInline();
	EvtEntry* entry = wp->entries;
	for (i = 0; i < wp->entryCount; i++, entry++) {
		if ((entry->flags & 1) && (entry->evtNum == evtNum)) {
			return entry;
		}
	}
	return NULL;
}
