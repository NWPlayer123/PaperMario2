#include "battle/battle_stage_object.h"
#include "battle/battle.h"
#include <string.h>

extern BattleWork* _battleWorkPointer;

void BattleStageObjectMain(void) {

}

BattleWorkStageObject* BattleSearchObjectPtr(char* name) {
	BattleWorkStageObject* object;
	int i;

	for (i = 0; i < 32; i++) {
		object = &_battleWorkPointer->mStageObjectWork[i];
		if (object->mId && !strcmp(object->mBaseObjectData->mName, name)) {
			return object;
		}
	}
	return NULL; //if name not found
}

BattleWorkStageObject* BattleGetObjectPtr(u32 id) {
	int i;
	
	for (i = 0; i < 32; i++) {
		if (_battleWorkPointer->mStageObjectWork[i].mId == id) {
			return &_battleWorkPointer->mStageObjectWork[i];
		}
	}
	return NULL; //if none of the IDs match
}