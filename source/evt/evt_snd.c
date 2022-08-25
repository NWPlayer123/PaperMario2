#include "evt/evt_snd.h"
#include "pmario_sound.h"

USERFUNC_DEF(evt_snd_bgmon) {
    //psndBGMOn((u32)evt->args[0], (const char*)evt->args[1]);
    return EVT_RETURN_DONE;
}