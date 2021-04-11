#include "battle/battle_event_cmd.h"
#include "battle/battle_acrobat.h"
#include "battle/battle_sub.h"

extern BattleWork* _battleWorkPointer;

EvtStatus btlevtcmd_ACRStart(EvtEntry* evt, BOOL isFirstCall) {
    s32 unitId, windowStartFrame, windowEndFrame, endFrame, earlyFrames;

    s32* args = evt->currCmdArgs;
    if (isFirstCall) {
        unitId = BattleTransID(evt, evtGetValue(evt, args[0]));
        windowStartFrame = evtGetValue(evt, args[1]);
        windowEndFrame = evtGetValue(evt, args[2]);
        endFrame = evtGetValue(evt, args[3]);
        earlyFrames = evtGetValue(evt, args[4]);
        BattleAcrobatStart(_battleWorkPointer, unitId, windowStartFrame, windowEndFrame, endFrame, earlyFrames);
    }
    return BattleAcrobatMain(_battleWorkPointer);
}