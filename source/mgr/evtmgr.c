#include "mgr/evtmgr.h"
#include "evt/evt_msg.h"
#include "memory.h"
#include <string.h>

extern GlobalWork* gp;
extern s32 _mariostSystemLevel;

//.bss
static EventWork work[2];
s32 priTbl[0x100];
s32 priIDTbl[0x100];

//.sbss
s32 evtMax;
s32 priTblNum;
BOOL runMainF;

//.sdata
f32 evtSpd = 1.0f;
s32 evtID = 1;

//local definitions
void make_pri_table(void);
void evtEntryRunCheck(EventEntry* evt);

#define evtGetWorkInline() (gp->inBattle ? &work[1] : &work[0])
#define evtGetWork2() (gp->inBattle ? &work[1] : &work[0])

EventWork* evtGetWork(void) {
	return gp->inBattle ? &work[1] : &work[0];
}

void make_pri_table(void) {
	EventWork* wp = evtGetWorkInline();
	EventEntry* event;
	s32 *currTbl, *currId, *nextTbl, *nextId;
	s32 tempTbl, tempId;
	int i, j, num;

	currTbl = priTbl;
	currId = priIDTbl;
	event = wp->entries;
	num = 0;
	for (i = 0; i < wp->count; i++, event++) {
		if (event->flags & 1) {
			*currTbl++ = i;
			num++;
			*currId++ = event->eventId;
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

inline void make_jump_table(EventEntry* evt) {
	s32 header, label, params, opcode;
	int i, n;
	s32* cmd;

	for (i = 0; i < 16; i++) {
		evt->labelIdTable[i] = -1;
		evt->labelAddressTable[i] = 0;
	}

	n = 0;
	cmd = evt->nextCommand;
	while (1) {
		header = *cmd++;
		label = *cmd;
		opcode = header & 0xFFFF;
		params = header >> 16;
		cmd += params;

		switch (opcode) {
			case OP_ScriptEnd:
				return; //exit loop
			case OP_Label:
				evt->labelIdTable[n] = (s8)label;
				evt->labelAddressTable[n] = (s32*)cmd;
				n++;
		}
		if (n >= 16) { //some assert, TODO?
			"unused";
		}
	}
}

void evtmgrInit(void) {
	work[0].count = 256;
	work[0].entries = __memAlloc(HEAP_DEFAULT, sizeof(EventEntry) * work[0].count);
	work[0].lastUpdate = gp->renderTime;
	memset(work[0].entries, 0, sizeof(EventEntry) * work[0].count);
	memset(work[0].gwData, 0, sizeof(work[0].gwData));
	memset(work[0].gfData, 0, sizeof(work[0].gfData));

	work[1].count = 256;
	work[1].entries = __memAlloc(HEAP_DEFAULT, sizeof(EventEntry) * work[1].count);
	work[1].lastUpdate = gp->renderTime;
	memset(work[1].entries, 0, sizeof(EventEntry) * work[1].count);
	memset(work[1].gwData, 0, sizeof(work[1].gwData));
	memset(work[1].gfData, 0, sizeof(work[1].gfData));

	evtMax = 0;
	priTblNum = 0;
	runMainF = FALSE;
	evt_msg_init();
}

void evtmgrReInit(void) {
	EventWork* wp = evtGetWorkInline();
	wp->lastUpdate = gp->renderTime;
	memset(wp->entries, 0, sizeof(EventEntry) * wp->count);
	evtMax = 0;
	runMainF = FALSE;
	evt_msg_init();
}

void evtEntryRunCheck(EventEntry* evt) {
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

//minor regalloc in make_jump_table, 99% 1:1
EventEntry* evtEntry(void* evtCode, u8 priority, u8 flags) {
	EventWork* wp = evtGetWorkInline();
	EventEntry* entry;
	s32 priId, priNext;
	int i, id;
	
	//search until we find one not-in-use
	entry = wp->entries;
	for (i = 0, id = 0; i < wp->count; i++, id++, entry++) {
		if (!(entry->flags & 1)) {
			break;
		}
	}

	evtMax++;
	memset(entry, 0, sizeof(EventEntry));
	entry->flags = (u8)(flags | 1);
	entry->nextCommand = evtCode;
	entry->restartFrom = evtCode;
	entry->currCommand = evtCode;
	entry->opcode = OP_InternalFetch;
	entry->waitingEvent = NULL;
	entry->waitingOnEvent = NULL;
	entry->prevBrotherEvent = NULL;
	entry->priority = priority;
	entry->eventId = evtID++;
	entry->unitId = -1;
	entry->loopDepth = -1;
	entry->switchStackIndex = -1;
	entry->typeMask = 0xEF;
	entry->name = NULL;
	entry->timescale = evtSpd;
	entry->commandsLeft = 0.0f;
	entry->caseId = -1;
	entry->thisNpc = NULL;
	entry->timeSinceStart = 0;
	for (i = 0; i < 16; i++) {
		entry->lwData[i] = 0;
	}
	for (i = 0; i < 3; i++) {
		entry->lfData[i] = 0;
	}

	make_jump_table(entry);

	if (runMainF && entry->flags & 0x20) {
		priId = priTblNum;
		priNext = priTblNum + 1;
		priTbl[priId] = id;
		priIDTbl[priId] = entry->eventId;
		priTblNum = priNext;
	}

	evtEntryRunCheck(entry);

	if (!evtID) {
		evtID = 1;
	}

	return entry;
}

EventEntry* evtEntryType(void* evtCode, u8 priority, u8 flags, u8 typeMask) {
	EventWork* wp = evtGetWorkInline();
	EventEntry* entry;
	s32 priId, priNext;
	int i, id;

	//search until we find one not-in-use
	entry = wp->entries;
	for (i = 0, id = 0; i < wp->count; i++, id++, entry++) {
		if (!(entry->flags & 1)) {
			break;
		}
	}

	evtMax++;
	memset(entry, 0, sizeof(EventEntry));

	entry->flags = (u8)(flags | 1);
	entry->nextCommand = evtCode;
	entry->restartFrom = evtCode;
	entry->currCommand = evtCode;
	entry->opcode = OP_InternalFetch;
	entry->waitingEvent = NULL;
	entry->waitingOnEvent = NULL;
	entry->prevBrotherEvent = NULL;
	entry->priority = priority;
	entry->eventId = evtID++;
	entry->unitId = -1;
	entry->loopDepth = -1;
	entry->switchStackIndex = -1;
	entry->typeMask = typeMask;
	entry->name = NULL;
	entry->timescale = evtSpd;
	entry->commandsLeft = 0.0f;
	entry->caseId = -1;
	entry->thisNpc = NULL;
	entry->timeSinceStart = 0;
	for (i = 0; i < 16; i++) {
		entry->lwData[i] = 0;
	}
	for (i = 0; i < 3; i++) {
		entry->lfData[i] = 0;
	}

	make_jump_table(entry);

	if (runMainF && entry->flags & 0x20) {
		priId = priTblNum;
		priNext = priTblNum + 1;
		priTbl[priId] = id;
		priIDTbl[priId] = entry->eventId;
		priTblNum = priNext;
	}

	evtEntryRunCheck(entry);

	if (!evtID) {
		evtID = 1;
	}

	return entry;
}

EventEntry* evtChildEntry(EventEntry* parent, void* evtCode, u8 flags) {
	EventWork* wp = evtGetWorkInline();
	EventEntry* entry;
	s32 priId, priNext;
	int i, id;

	//search until we find one not-in-use
	entry = wp->entries;
	for (i = 0, id = 0; i < wp->count; i++, id++, entry++) {
		if (!(entry->flags & 1)) {
			break;
		}
	}

	evtMax++;
	parent->waitingOnEvent = entry;
	parent->flags |= 0x10;
	memset(entry, 0, sizeof(EventEntry));

	entry->flags = (u8)(flags | 1);
	entry->nextCommand = evtCode;
	entry->restartFrom = evtCode;
	entry->currCommand = evtCode;
	entry->opcode = OP_InternalFetch;
	entry->waitingEvent = parent;
	entry->waitingOnEvent = NULL;
	entry->prevBrotherEvent = NULL;
	entry->priority = parent->priority + 1;
	entry->eventId = evtID++;
	entry->unitId = parent->unitId;
	entry->loopDepth = -1;
	entry->switchStackIndex = -1;
	entry->typeMask = parent->typeMask;
	entry->name = NULL;
	entry->uwBase = parent->uwBase;
	entry->ufBase = parent->ufBase;
	entry->timescale = evtSpd;
	entry->commandsLeft = 0.0f;
	entry->caseId = -1;
	entry->thisNpc = parent->thisNpc;
	entry->timeSinceStart = 0;
}













/*
EventEntry* evtRestart(EventEntry* evt) {
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
	EventEntry* entry;
	int i, ret, ret2;

	EventWork* wp = evtGetWorkInline();
	runMainF = TRUE; //we're in evtmgrMain

	delta = gp->renderTime - wp->lastUpdate;
	if (delta < 0) {
		delta = 0;
	}

	if (OSTicksToMilliseconds(delta) > 500) {
		delta = OSMillisecondsToTicks(1000 / gp->framerate);
	}

	wp->lastUpdate = gp->renderTime;
	make_pri_table();
	for (i = 0; i < priTblNum; i++) {
		entry = &wp->entries[priTbl[i]];
		if ((entry->flags & 1) && (entry->eventId == priIDTbl[i]) && !(entry->flags & 0x92)) {
			entry->timeSinceStart += delta;
			entry->commandsLeft += entry->timescale; //update num_instructions
			while (entry->commandsLeft >= 1.0f) { //now execute however many commands
				entry->commandsLeft -= 1.0f;
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

void evtDelete(EventEntry* evt) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	if (evt->flags & 1) {
		if (evt->waitingOnEvent) {
			evtDelete(evt->waitingOnEvent);
		}

		entry = wp->entries;
		for (i = 0; i < wp->count; i++, entry++) {
			if ((entry->flags & 1) && (entry->prevBrotherEvent == evt)) {
				evtDelete(entry);
			}
		}

		entry = evt->waitingEvent;
		if (entry != NULL) {
			entry->flags &= ~0x10;
			entry->waitingOnEvent = NULL;
			for (i = 0; i < 16; i++) {
				entry->lwData[i] = evt->lwData[i];
			}
			for (i = 0; i < 3; i++) {
				entry->lfData[i] = evt->lfData[i];
			}
			entry->selectWindowId = evt->selectWindowId;
			entry->printWindowId = evt->printWindowId;
			entry->printWindowFlags = evt->printWindowFlags;
			entry->field_0x184 = evt->field_0x184;
			entry->field_0x188 = evt->field_0x188;
			entry->field_0x18C = evt->field_0x18C;
			entry->msgPriority = evt->msgPriority;
		}
		evt->flags &= ~1;
		memset(evt, 0, sizeof(EventEntry));
		evtMax--;
	}
}

void evtDeleteID(s32 evtNum) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry->eventId == evtNum)) {
			evtDelete(entry);
		}
	}
}

BOOL evtCheckID(s32 evtNum) {
	int i;

	EventWork* wp = evtGetWorkInline();
	EventEntry* entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry->eventId == evtNum)) {
			return TRUE;
		}
	}
	return FALSE;
}

void evtSetPri(EventEntry* evt, u8 priority) {
	evt->priority = priority;
}

void evtSetSpeed(EventEntry* evt, f32 speed) {
	evt->timescale = speed * evtSpd;
}

void evtSetType(EventEntry* evt, u8 typeMask) {
	evt->typeMask = typeMask;
}

//this entire function gets recursively inlined twice
void evtStop(EventEntry* evt, u8 typeMask) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	if (evt->waitingOnEvent) {
		evtStop(evt->waitingOnEvent, typeMask);
	}

	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry->prevBrotherEvent == evt)) {
			evtStop(entry, typeMask);
		}
	}

	if (evt->typeMask & typeMask) {
		evt->flags |= 2;
	}
}

//this entire function gets recursively inlined twice
void evtStart(EventEntry* evt, u8 typeMask) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	if (evt->waitingOnEvent) {
		evtStart(evt->waitingOnEvent, typeMask);
	}

	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry->prevBrotherEvent == evt)) {
			evtStart(entry, typeMask);
		}
	}

	if (evt->typeMask & typeMask) {
		evt->flags &= ~2;
	}
}

void evtStopID(s32 evtNum) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry->eventId == evtNum)) {
			evtStop(entry, 0xEF);
		}
	}
}

void evtStartID(s32 evtNum) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry->eventId == evtNum)) {
			evtStart(entry, 0xEF);
		}
	}
}

void evtStopAll(u8 typeMask) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if (entry->flags & 1) {
			evtStop(entry, typeMask);
		}
	}
}

void evtStartAll(u8 typeMask) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if (entry->flags & 1) {
			evtStart(entry, typeMask);
		}
	}
}

void evtStopOther(EventEntry* evt, u8 typeMask) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry != evt)) {
			evtStop(entry, typeMask);
		}
	}
}

void evtStartOther(EventEntry* evt, u8 typeMask) {
	EventEntry* entry;
	int i;

	EventWork* wp = evtGetWorkInline();
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry != evt)) {
			evtStart(entry, typeMask);
		}
	}
}

EventEntry* evtGetPtr(s32 index) {
	EventWork* wp = evtGetWorkInline();
	EventEntry* entry = &wp->entries[index];
	if (entry->flags & 1) {
		return entry;
	}
	return NULL;
}

EventEntry* evtGetPtrID(s32 evtNum) {
	int i;

	EventWork* wp = evtGetWorkInline();
	EventEntry* entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if ((entry->flags & 1) && (entry->eventId == evtNum)) {
			return entry;
		}
	}
	return NULL;
}
