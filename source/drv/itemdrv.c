#include "drv/itemdrv.h"
#include "drv/icondrv.h"
#include "mario_pouch.h"
#include "mariost.h"
#include "memory.h"
#include <string.h>

extern GlobalWork* gp;

//.bss
static ItemWork work[2];

//TODO: inline function? I see nothing in the symbol map
#define itemGetWork() (gp->isBattleInit ? &work[1] : &work[0])

void itemInit(void) {
	work[0].count = 32;
	work[0].entries = __memAlloc(HEAP_DEFAULT, work[0].count * sizeof(ItemEntry));
	memset(work[0].entries, 0, work[0].count * sizeof(ItemEntry));
	work[0].field_0x8 = 0;

	work[1].count = 32;
	work[1].entries = __memAlloc(HEAP_DEFAULT, work[1].count * sizeof(ItemEntry));
	memset(work[1].entries, 0, work[1].count * sizeof(ItemEntry));
	work[1].field_0x8 = 0;
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
					pouchArriveBadge(id);
				}
			}
		}
	}
	entry = wp->entries;
	for (i = 0; i < wp->count; i++, entry++) {
		iconDelete(entry->icon_name);
		entry->flags &= ~1;
		memset(entry->icon_name, 0, sizeof(entry->icon_name));
	}
}

void itemMain(void) {

}