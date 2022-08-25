#include "aji_00.h"
#include "evt/evt_snd.h"

EVT_BEGIN(aji_00_init_evt)
IF_LESS(GSW(0), 0x169)
USER_FUNC(evt_snd_bgmon, 0x200, STRING("BGM_STG7_DUN1"))
EVT_END()