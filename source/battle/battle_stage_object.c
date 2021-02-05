#include "battle/battle_stage_object.h"
#include "battle/battle.h"
#include "system.h"
#include <string.h>

extern BattleWork* _battleWorkPointer;

//local prototypes
void BattleObjectConfig(void);



EvtStatus _set_mobj_shake_init(EvtEntry* evt) {
	BattleWorkStageObject* obj;
	int i, layer;
	u8 position;

	//get the layer this object is on
	layer = evtGetValue(evt, evt->currCmdArgs[0]);

	for (i = 0; i < 32; i++) {
		obj = &_battleWorkPointer->mStageObjectWork[i];
		if (obj->mId > 0 && obj->mBaseObjectData->mLayer == layer) {
			obj->field_0x72 = 0;
			obj->mShakePeriodLength = (u8)(irand(10) + 60);
			if (irand(100) & 1) {
				position = 0;
			}
			else {
				position = (u8)(obj->mShakePeriodLength / 2);
			}
			obj->mShakePeriodPosition = position;
			obj->mShakeAngle = 0.0f;
		}
	}
	return EVT_RETURN_DONE2;
}







void BattleObjectConfig(void) {
	//stubbed in retail
}

void BattleStageObjectMain(void) {

}

void BattleStageObjectInit(void) {

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