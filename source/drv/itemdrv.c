#include "drv/itemdrv.h"
#include "drv/icondrv.h"
#include "drv/swdrv.h"
#include "mgr/evtmgr_cmd.h"
#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include "memory.h"
#include <string.h>

extern GlobalWork* gp;
extern int sprintf(char* str, const char* format, ...);

//.bss
static ItemWork work[2];

//TODO: inline function? I see nothing in the symbol map
#define itemGetWork() (gp->isBattleInit ? &work[1] : &work[0])

void itemInit(void) {
	work[0].count = 32;
	work[0].entries = __memAlloc(HEAP_DEFAULT, work[0].count * sizeof(ItemEntry));
	memset(work[0].entries, 0, work[0].count * sizeof(ItemEntry));
	work[0].currId = 0;

	work[1].count = 32;
	work[1].entries = __memAlloc(HEAP_DEFAULT, work[1].count * sizeof(ItemEntry));
	memset(work[1].entries, 0, work[1].count * sizeof(ItemEntry));
	work[1].currId = 0;
}

void itemReInit(void) {
	ItemWork* wp = itemGetWork();
	ItemEntry* entry;
	s32 i, id;

	if (!gp->isBattleInit) {
		entry = wp->entries;
		for (i = 0; i < wp->count; i++, entry++) {
			if (entry->flags & 1 && !gp->isBattleInit) {
				id = entry->itemId;
				if (id >= BADGE_MIN && id < BADGE_MAX + 1 && entry->flags & 0x100) {
					pouchArriveBadge((s16)id); //TODO: remove cast?
				}
			}
		}
	}

	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		iconDelete(entry->name);
		entry->flags &= ~1;
		memset(entry->name, 0, sizeof(entry->name));
	}
}






ItemEntry* itemEntry(const char* name, ItemType type, s32 a3, s32 evtId, s32 a5, f32 a6, f32 a7, f32 a8) {
	ItemWork* wp = itemGetWork();
	ItemEntry* entry;
	s32 evtVar;
	int i;
	char itemName[64];


	evtVar = evtId;
	if (evtId != -1) {
		if (evtId >= EVTDAT_GSWF_MIN && evtId <= EVTDAT_GSWF_MAX) {
			evtVar = evtId + EVTDAT_GSWF_BASE;
			if (swGet(evtId + EVTDAT_GSWF_BASE)) {
				return NULL;
			}
		}
		else {
			if (evtId >= EVTDAT_LSWF_MIN && evtId <= EVTDAT_LSWF_MAX) {
				evtVar = evtId + EVTDAT_LSWF_BASE;
				if (_swGet(evtId + EVTDAT_LSWF_BASE)) {
					return NULL;
				}
			}
		}
	}

	if (name == NULL) {
		if (++wp->currId >= 0xFFFFF) { //fix overflow
			wp->currId = 1;
		}
		sprintf(itemName, "_i%05x", wp->currId);
		name = itemName;
	}

	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		if (entry->flags & 1 && !strcmp(entry->name, name)) {
			break;
		}
	}

	entry = wp->entries;
	if (wp->count > 0) { //TODO: redundant check?
		for (i = 0; i < wp->count; i++, entry++) {
			if (!(entry->flags & 1)) {
				break;
			}
		}
	}
}


void itemMain(void) {

}