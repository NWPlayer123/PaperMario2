#include "drv/hitdrv.h"
#include <string.h>

void hitInit(void) {
	; //stubbed in retail
}

void hitReInit(void) {
	HitEntry* entry;
	MapEntry* map;
	s32 i, j;

	map = mapGetWork()->entries;
	for (i = 0; i < map->mapCount; i++, map++) {
		entry = map->hitObjects;
		if (entry) {
			for (j = 0; j < map->hitNumJoints; j++) {

			}
		}
	}

}

void hitMain(void) {

}



HitEntry* hitNametoPtr(const char* name) {
	MapEntry* map;
	HitEntry* hit;
	s32 i, j;

	map = mapGetWork()->entries;
	if (!name) {
		return NULL;
	}
	for (i = 0; i < map->mapCount; i++, map++) {
		hit = map->hitObjects;
		for (j = 0; j < map->hitNumJoints; j++, hit++) {
			if (!(hit->flags & 0x80) && !strcmp(hit->joint->name, name)) {
				return hit;
			}
		}
	}
	return NULL;
}





HitEntry* hitEntry(MapFileJoint* joint, Mtx mtx, BOOL group) {
	MapWork* wp;
	MapEntry* entry;
	u32 size;

	wp = mapGetWork();
	entry = &wp->entries[group];
	entry->hitNumJoints = mapGetJoints(joint);
	size = OSRoundUp32B(sizeof(HitEntry) * (entry->hitNumJoints + 128));
	memset(entry->hitObjects, 0, size);
	//return hitEntrySub(joint, 0, mtx, 1, group);
}