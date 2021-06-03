#include "mgr/evtmgr.h"
#include <string.h> //memset

//.bss
s32 priIDTbl[0x100];
s32 priTbl[0x100];
evtWork work[2];

//.sbss
BOOL runMainF;
s32 priTblNum;
u32 evtMax; //TODO: sort these once they're all added

extern GlobalWork* gp;

//local definitions
void make_pri_table(void);




evtWork* evtGetWork(void) {
	return &work[gp->isBattleInit];
}

void make_pri_table(void) {
	evtWork* work;
	int i, j, num;

	work = evtGetWork(); //inline
	for (i = 0, num = 0; i < work->entryCount; i++) {
		if (work->entries[i].flags & 1) {
			priTbl[i] = i;
			priIDTbl[i] = work->entries[i].threadId;
			num++;
		}
	}
	priTblNum = num;
	for (i = 0; i < num - 1; i++) {
		if (i + 1 < num) {
			for (j = 0; j < num - (i + 1); j++) {

			}
		}
	}
}

void evtmgrInit(void) {

}









EvtEntry* evtGetPtrID(s32 threadId) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry->threadId == threadId)) {
			return entry;
		}
	}
	return NULL;
}

EvtEntry* evtGetPtr(s32 index) {
	EvtEntry* entry;

	evtWork* work = evtGetWork(); //inline
	entry = &work->entries[index];
	if (entry->flags & 1) {
		return entry;
	}
	return NULL;
}

void evtStartOther(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry != evt)) {
			evtStart(entry, typeMask);
		}
	}
}

void evtStopOther(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry != evt)) {
			evtStop(entry, typeMask);
		}
	}
}

void evtStartAll(u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if (entry->flags & 1) {
			evtStart(entry, typeMask);
		}
	}
}

void evtStopAll(u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if (entry->flags & 1) {
			evtStop(entry, typeMask);
		}
	}
}

void evtStartID(s32 threadId) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry->threadId == threadId)) {
			evtStart(entry, 0xEF);
		}
	}
}

void evtStopID(s32 threadId) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry->threadId == threadId)) {
			evtStop(entry, 0xEF);
		}
	}
}

//this entire function gets recursively inlined twice
void evtStart(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	if (evt->waitingOnEvt) {
		evtStart(evt->waitingOnEvt, typeMask);
	}

	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry->parentEvt == evt)) {
			evtStart(entry, typeMask);
		}
	}

	if (evt->typeMask & typeMask) {
		evt->flags &= 0xFD;
	}
}

//this entire function gets recursively inlined twice
void evtStop(EvtEntry* evt, u8 typeMask) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	if (evt->waitingOnEvt) {
		evtStop(evt->waitingOnEvt, typeMask);
	}

	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry->parentEvt == evt)) {
			evtStop(entry, typeMask);
		}
	}

	if (evt->typeMask & typeMask) {
		evt->flags |= 2;
	}
}

void evtSetType(EvtEntry* evt, u8 typeMask) {
	evt->typeMask = typeMask;
}

void evtSetSpeed(EvtEntry* evt, f32 speed) {
	evt->speed = speed * 1.0f; //TODO: probably unnecessary multiply
}

void evtSetPri(EvtEntry* evt, u8 priority) {
	evt->priority = priority;
}

BOOL evtCheckID(s32 threadId) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry->threadId == threadId)) {
			return TRUE;
		}
	}
	return FALSE;
}

void evtDeleteID(s32 threadId) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	for (i = 0; i < work->entryCount; i++) {
		entry = &work->entries[i];
		if ((entry->flags & 1) && (entry->threadId == threadId)) {
			evtDelete(entry);
		}
	}
}

void evtDelete(EvtEntry* evt) {
	EvtEntry* entry;
	int i;

	evtWork* work = evtGetWork(); //inline
	if (evt->flags & 1) {
		if (evt->waitingOnEvt) {
			evtDelete(evt->waitingOnEvt);
		}
		for (i = 0; i < work->entryCount; i++) {
			entry = &work->entries[i];
			if ((entry->flags & 1) && (entry->parentEvt == evt)) {
				evtDelete(entry);
			}
		}
		entry = evt->waitingEvt;
		if (entry != NULL) {
			entry->flags &= 0xEF;
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
		evt->flags &= 0xFE;
		memset(evt, 0, sizeof(EvtEntry));
		evtMax--;
	}
}

void evtmgrMain(void) {
	OSTime delta;
	EvtEntry* entry;
	int i, ret, ret2;

	evtWork* work = evtGetWork(); //inline
	runMainF = TRUE; //we're in evtmgrMain

	delta = gp->mAnimationTimeInclBattle - work->currentEvtTime;
	if (delta < 0) {
		delta = 0;
	}

	if (OSTicksToMilliseconds(delta) > 500) {
		delta = OSMillisecondsToTicks(1000 / gp->mFPS);
	}

	work->currentEvtTime = gp->mAnimationTimeInclBattle;
	make_pri_table();
	for (i = 0; i < priTblNum; i++) {
		entry = &work->entries[priTbl[i]];
		if ((entry->flags & 1) && (entry->threadId == priIDTbl[i]) && !(entry->flags & 0x92)) {
			entry->timeSinceStart += delta;
			entry->timeScheduledToRun += entry->speed; //update num_instructions
			while (entry->timeScheduledToRun > 1.0f) { //now execute however many commands
				entry->timeScheduledToRun -= 1.0f;
				ret = evtmgrCmd(entry);
				if (ret == 1) {
					ret2 = TRUE;
					break;
				}
				if (ret == -1) break;
			}
		}
		if (ret2) break;
	}
	runMainF = FALSE; //we're done with evtmgrMain for this frame
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
	evt->loopStackIndex = -1;
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





