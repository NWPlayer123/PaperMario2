#include "drv/hitdrv.h"
#include <string.h>

void hitInit(void) {
	; //stubbed in retail
}

void hitMain(void) {

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