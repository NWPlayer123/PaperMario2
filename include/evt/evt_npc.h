#pragma once

#include <dolphin/types.h>
#include "drv/npcdrv.h"
#include "mgr/evtmgr.h"

NpcEntry* evtNpcNameToPtr(EvtEntry* evt, char* name);