#include "battle/battle_seq.h"
#include "battle/battle.h"

extern BattleWork* _battleWorkPointer;

//local prototypes
void BattleCheckAllPinchStatus(BattleWork* work, BOOL a2);
void btlseqInit(BattleWork* work);

void BattleSequenceManager(void) {
	BattleWork* wp = _battleWorkPointer;
	switch (BattleGetSeq(wp, SEQ_UNKNOWN)) {
		case 0:
			btlseqInit(wp);
			break;
	}
}

void BattleCheckAllPinchStatus(BattleWork* work, BOOL a2) {

}

void btlseqInit(BattleWork* work) {

}