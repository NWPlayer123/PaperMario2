#include "evt/evt_npc.h"
#include <string.h>

NpcEntry* evtNpcNameToPtr(EvtEntry* evt, char* name) {
	NpcEntry* entry;

	entry = (NpcEntry*)evt->wThisPtr;
	if (!strcmp(name, "me")) {
		return entry;
	}
	else if (!strcmp(name, "party")) {
		return (NpcEntry*)-1;
	}
	else if (!strcmp(name, "(^x^)party")) {
		return (NpcEntry*)-1;
	}
	else if (!strcmp(name, "extparty")) {
		return (NpcEntry*)-2;
	}
	else if (!strcmp(name, "mario")) {
		return (NpcEntry*)-3;
	}
	else if (!strcmp(name, "master")) {
		return entry->master;
	}
	else if (!strcmp(name, "slave_0")) {
		return entry->slaves[0];
	}
	else if (!strcmp(name, "slave_1")) {
		return entry->slaves[1];
	}
	else if (!strcmp(name, "slave_2")) {
		return entry->slaves[2];
	}
	else if (!strcmp(name, "slave_3")) {
		return entry->slaves[3];
	}
	else {
		return NULL;// npcNameToPtr(name);
	}
}