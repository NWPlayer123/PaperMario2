#include "evt/evt_npc.h"
#include "drv/animdrv.h"
#include "mariost.h"
#include <string.h>

extern GlobalWork* gp;
extern int sprintf(char* str, const char* format, ...);

//scripts
EVT_BEGIN(npc_init_evt)
USER_FUNC(evt_npc_flag_onoff, 1, STRING("me"), 0x600)
RETURN()
EVT_END()

//const char* npcDesc, const char* npcName
USERFUNC_DEF(evt_npc_entry) {
	s32* args = evt->currCmdArgs;
	const char *desc, *name;

	desc = (const char*)evtGetValue(evt, args[0]);
	name = (const char*)evtGetValue(evt, args[1]);
	if (!animGroupBaseAsync(name, gp->isBattleInit != 0, 0)) {
		return EVT_RETURN_BLOCK;
	}
	npcEntry(desc, name);
	return EVT_RETURN_DONE;
}

//const char* lookupName, s32 slaveId, const char* animName, s32 evtIndex, void* deadEvt
USERFUNC_DEF(evt_npc_slave_entry) {
	s32* args = evt->currCmdArgs;
	const char *name, *animName;
	NpcEntry *npc, *slave;
	s32 id, index;
	void* deadEvt;
	u8 v20[184]; //TODO: find right size, some unused struct
	char slaveName[32];
	char ident[4];

	name = (const char*)evtGetValue(evt, args[0]);
	npc = evtNpcNameToPtr(evt, name);
	id = evtGetValue(evt, args[1]);
	animName = (const char*)evtGetValue(evt, args[2]);
	index = args[3];
	deadEvt = (void*)evtGetValue(evt, args[4]);
	memset(v20, 0, sizeof(v20));
	sprintf(ident, "_s%d", id);
	strcpy(slaveName, npc->description);
	strcat(slaveName, ident);
	npcEntry(slaveName, animName);
	slave = npcNameToPtr(slaveName);
	slave->flags |= 0x120018;
	slave->deadEvt = deadEvt;
	slave->battleInfo = npc->battleInfo;
	npcSetSlave(npc, slave, id);
	evtSetValue(evt, index, (s32)&slave->description);
	return EVT_RETURN_DONE;
}

//const char* lookupName
USERFUNC_DEF(evt_npc_delete) {
	const char* name;
	NpcEntry* npc;

	name = (const char*)evtGetValue(evt, evt->currCmdArgs[0]);
	npc = evtNpcNameToPtr(evt, name);
	npcDeleteGroup(npc);
	return EVT_RETURN_DONE;
}

//const char* lookupName
USERFUNC_DEF(evt_npc_check_delete) {
	const char* name;
	NpcEntry* npc;

	name = (const char*)evtGetValue(evt, evt->currCmdArgs[0]);
	npc = evtNpcNameToPtr_NoAssert(evt, name);
	if (npc) {
		npcDeleteGroup(npc);
	}
	return EVT_RETURN_DONE;
}

//BOOL inBattle, s32 evtIndex
USERFUNC_DEF(evt_npc_get_ReactionOfLivingBody) {
	s32* args = evt->currCmdArgs;
	s32 index, value;
	BOOL inBattle;

	inBattle = evtGetValue(evt, args[0]);
	index = args[1];
	value = npcGetReactionOfLivingBody(inBattle);
	evtSetValue(evt, index, value);
	return EVT_RETURN_DONE;
}

//NpcSetupInfo* setuplist
USERFUNC_DEF(evt_npc_setup) {
	NpcSetupInfo *setuplist, *setup;
	EvtEntry* newEvt;
	NpcTribe* tribe;
	NpcEntry* npc;
	s32 evtId;

	setuplist = (NpcSetupInfo*)evtGetValue(evt, evt->currCmdArgs[0]);
	fbatGetPointer(); //unused
	if (isFirstCall) {
		for (setup = setuplist; setup->name; setup++) {
			npc = npcNameToPtr_NoAssert(setup->name);
			if (npc) {
				tribe = npc->tribe;
				npc->flags |= setup->flags;
				npc->reactionFlags |= setup->reactionFlags;
				if (setup->initEvt) {
					evtId = npc->initEvtId;
					if (evtId) {
						evtDeleteID(evtId);
					}
					newEvt = evtEntry(setup->initEvt, 0, 0);
					newEvt->wNpcEventType = 0;
					newEvt->wThisPtr = npc;
					npc->initEvtId = newEvt->evtNum;
				}
				else {
					npc->initEvtId = 0;
				}

				npc->talkEvt = setup->talkEvt;
				npc->deadEvt = setup->deadEvt;
				npc->regularEvt = setup->regularEvt;
				npc->findEvt = setup->findEvt;
				npc->lostEvt = setup->lostEvt;
				npc->returnEvt = setup->returnEvt;
				npc->blowEvt = setup->blowEvt;
				npc->territoryType = setup->territoryType;
				npc->territoryBase = setup->territoryBase;
				npc->territoryLoiter = setup->territoryLoiter;
				npc->territoryHoming = setup->territoryLoiter;
				npc->searchRange = setup->searchRange;
				npc->searchAngle = setup->searchAngle;
				npc->homingRange = setup->homingRange;
				npc->homingAngle = setup->homingAngle;
				npcSetBattleInfo(npc, setup->battleInfoId);
				if (tribe->stayAnimation) {
					strcpy(npc->stayAnimation, tribe->stayAnimation);
				}
				if (tribe->talkAnimation) {
					strcpy(npc->talkAnimation, tribe->talkAnimation);
				}
				if (tribe->initialAnimation) {
					strcpy(npc->currentAnimation, tribe->initialAnimation);
					animPoseSetAnim(npc->poseId, npc->currentAnimation, 1);
				}
			}
		}
		fbatChangeMode(1);
		return EVT_RETURN_BLOCK; //call this function again
	}
	else {
		for (setup = setuplist; setup->name; setup++) {
			evtId = npcNameToPtr(setup->name)->initEvtId;
			if (evtId && evtCheckID(evtId)) {
				return EVT_RETURN_BLOCK; //call this function again
			}
		}
		for (setup = setuplist; setup->name; setup++) {
			npc = npcNameToPtr(setup->name);
			if (npc->regularEvt) {
				evtId = npc->regularEvtId;
				if (evtId) {
					evtDeleteID(evtId);
				}
				newEvt = evtEntry(npc->regularEvt, 0, 0x20);
				newEvt->wNpcEventType = 1;
				newEvt->wThisPtr = npc;
				npc->regularEvtId = newEvt->evtNum;
			}
		}
		return EVT_RETURN_DONE;
	}
}

//TODO: double check _savefpr_29/_restfpr_29 were generated
//const char* lookupName, f32 x, f32 y, f32 z
USERFUNC_DEF(evt_npc_set_position) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	f32 x, y, z;
	Vec temp;

	name = (const char*)evtGetValue(evt, args[0]);
	x = evtGetFloat(evt, args[1]);
	y = evtGetFloat(evt, args[2]);
	z = evtGetFloat(evt, args[3]);
	npc = evtNpcNameToPtr(evt, name);
	temp = (Vec){x, y, z};
	npc->prevPosition = temp;
	npc->position = npc->prevPosition;
	return EVT_RETURN_DONE;
}

//const char* lookupName, f32 width
USERFUNC_DEF(evt_npc_set_width) {
	s32* args = evt->currCmdArgs;
	const char* name;
	f32 width;

	name = (const char*)evtGetValue(evt, args[0]);
	width = evtGetFloat(evt, args[1]);
	evtNpcNameToPtr(evt, name)->width = width;
	return EVT_RETURN_DONE;
}

//const char* lookupName, f32 width
USERFUNC_DEF(evt_npc_set_height) {
	s32* args = evt->currCmdArgs;
	const char* name;
	f32 height;

	name = (const char*)evtGetValue(evt, args[0]);
	height = evtGetFloat(evt, args[1]);
	evtNpcNameToPtr(evt, name)->height = height;
	return EVT_RETURN_DONE;
}

//const char* lookupName, s32 evtStoreIndex
USERFUNC_DEF(evt_npc_get_height) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	s32 index;

	name = (const char*)evtGetValue(evt, args[0]);
	index = args[1];
	npc = evtNpcNameToPtr(evt, name);
	evtSetFloat(evt, index, npc->height);
	return EVT_RETURN_DONE;
}

//const char* lookupName, f32 x, f32 y, f32 z
USERFUNC_DEF(evt_npc_set_scale) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	f32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = evtGetFloat(evt, args[1]);
	y = evtGetFloat(evt, args[2]);
	z = evtGetFloat(evt, args[3]);
	npc = evtNpcNameToPtr(evt, name);
	npc->scale = (Vec){x, y, z};
	return EVT_RETURN_DONE;
}

//const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USERFUNC_DEF(evt_npc_get_scale) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	s32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = args[1];
	y = args[2];
	z = args[3];
	npc = evtNpcNameToPtr(evt, name);
	evtSetFloat(evt, x, npc->scale.x);
	evtSetFloat(evt, y, npc->scale.y);
	evtSetFloat(evt, z, npc->scale.z);
	return EVT_RETURN_DONE;
}

//const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USERFUNC_DEF(evt_npc_get_position) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	s32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = args[1];
	y = args[2];
	z = args[3];
	npc = evtNpcNameToPtr(evt, name);
	evtSetFloat(evt, x, npc->position.x);
	evtSetFloat(evt, y, npc->position.y);
	evtSetFloat(evt, z, npc->position.z);
	return EVT_RETURN_DONE;
}

//const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USERFUNC_DEF(evt_npc_get_home_position) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	s32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = args[1];
	y = args[2];
	z = args[3];
	npc = evtNpcNameToPtr(evt, name);
	evtSetFloat(evt, x, npc->territoryBase.x);
	evtSetFloat(evt, y, npc->territoryBase.y);
	evtSetFloat(evt, z, npc->territoryBase.z);
	return EVT_RETURN_DONE;
}

//devs forgot to change s32 to f32 when copypasting, fix in rewrite
//const char* lookupName, f32 x, f32 y, f32 z
USERFUNC_DEF(evt_npc_set_home_position) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	s32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = (s32)evtGetFloat(evt, args[1]);
	y = (s32)evtGetFloat(evt, args[2]);
	z = (s32)evtGetFloat(evt, args[3]);
	npc = evtNpcNameToPtr(evt, name);
	npc->territoryBase = (Vec){(f32)x, (f32)y, (f32)z};
	return EVT_RETURN_DONE;
}

//const char* lookupName, s32 xStoreIndex, s32 yStoreIndex, s32 zStoreIndex
USERFUNC_DEF(evt_npc_get_rotate) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	s32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = args[1];
	y = args[2];
	z = args[3];
	npc = evtNpcNameToPtr(evt, name);
	evtSetFloat(evt, x, npc->rotation.x);
	evtSetFloat(evt, y, npc->rotation.y);
	evtSetFloat(evt, z, npc->rotation.z);
	return EVT_RETURN_DONE;
}

//devs forgot to use evtGetFloat when copypasting, fix in rewrite
//const char* lookupName, f32 x, f32 y, f32 z
USERFUNC_DEF(evt_npc_set_rotate) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	f32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = (f32)evtGetValue(evt, args[1]);
	y = (f32)evtGetValue(evt, args[2]);
	z = (f32)evtGetValue(evt, args[3]);
	npc = evtNpcNameToPtr(evt, name);
	npc->rotation = (Vec){x, y, z};
	return EVT_RETURN_DONE;
}

//devs forgot to use evtGetFloat when copypasting, fix in rewrite
//const char* lookupName, f32 x, f32 y, f32 z
USERFUNC_DEF(evt_npc_add_rotate) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	f32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = (f32)evtGetValue(evt, args[1]);
	y = (f32)evtGetValue(evt, args[2]);
	z = (f32)evtGetValue(evt, args[3]);
	npc = evtNpcNameToPtr(evt, name);
	npc->rotation.x += x;
	npc->rotation.y += y;
	npc->rotation.z += z;
	return EVT_RETURN_DONE;
}

//devs forgot to use evtGetFloat when copypasting, fix in rewrite
//const char* lookupName, f32 x, f32 y, f32 z
USERFUNC_DEF(evt_npc_set_rotate_offset) {
	s32* args = evt->currCmdArgs;
	const char* name;
	NpcEntry* npc;
	f32 x, y, z;

	name = (const char*)evtGetValue(evt, args[0]);
	x = (f32)evtGetValue(evt, args[1]);
	y = (f32)evtGetValue(evt, args[2]);
	z = (f32)evtGetValue(evt, args[3]);
	npc = evtNpcNameToPtr(evt, name);
	npc->rotationOffset = (Vec){x, y, z};
	return EVT_RETURN_DONE;
}

USERFUNC_DEF(evt_npc_move_position) {
	s32* args = evt->currCmdArgs;
	const char* name;

	name = (const char*)evtGetValue(evt, args[0]);
	
	
	return EVT_RETURN_DONE; //TODO: finish
}

USERFUNC_DEF(evt_npc_jump_position) {

}










NpcEntry* evtNpcNameToPtr(EvtEntry* evt, const char* name) {
	NpcEntry* entry;

	entry = (NpcEntry*)evt->wThisPtr;
	if (!strcmp(name, "me")) {
		return entry;
	}
	if (!strcmp(name, "party")) {
		return (NpcEntry*)-1;
	}
	if (!strcmp(name, "(^x^)party")) {
		return (NpcEntry*)-1;
	}
	if (!strcmp(name, "extparty")) {
		return (NpcEntry*)-2;
	}
	if (!strcmp(name, "mario")) {
		return (NpcEntry*)-3;
	}
	if (!strcmp(name, "master")) {
		return entry->master;
	}
	if (!strcmp(name, "slave_0")) {
		return entry->slaves[0];
	}
	if (!strcmp(name, "slave_1")) {
		return entry->slaves[1];
	}
	if (!strcmp(name, "slave_2")) {
		return entry->slaves[2];
	}
	if (!strcmp(name, "slave_3")) {
		return entry->slaves[3];
	}
	return npcNameToPtr(name);
}

NpcEntry* evtNpcNameToPtr_NoAssert(EvtEntry* evt, const char* name) {
	NpcEntry* entry;

	entry = (NpcEntry*)evt->wThisPtr;
	if (!strcmp(name, "me")) {
		return entry;
	}
	if (!strcmp(name, "party")) {
		return (NpcEntry*)-1;
	}
	if (!strcmp(name, "(^x^)party")) {
		return (NpcEntry*)-1;
	}
	if (!strcmp(name, "extparty")) {
		return (NpcEntry*)-2;
	}
	if (!strcmp(name, "mario")) {
		return (NpcEntry*)-3;
	}
	if (!strcmp(name, "master")) {
		return entry->master;
	}
	if (!strcmp(name, "slave_0")) {
		return entry->slaves[0];
	}
	if (!strcmp(name, "slave_1")) {
		return entry->slaves[1];
	}
	if (!strcmp(name, "slave_2")) {
		return entry->slaves[2];
	}
	if (!strcmp(name, "slave_3")) {
		return entry->slaves[3];
	}
	return npcNameToPtr_NoAssert(name);
}

//s32 type (off/on), const char* npcName, s32 mask
USERFUNC_DEF(evt_npc_flag_onoff) {
	s32* args = evt->currCmdArgs;
	s32 type, mask;
	const char* name;
	NpcEntry* npc;

	type = evtGetValue(evt, args[0]);
	name = (const char*)evtGetValue(evt, args[1]);
	mask = evtGetValue(evt, args[2]);
	npc = evtNpcNameToPtr(evt, name);
	switch (type) {
		case 0:
			npc->flags &= ~mask;
			break;
		case 1:
			npc->flags |= mask;
	}
	return EVT_RETURN_DONE;
}