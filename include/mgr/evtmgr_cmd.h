#pragma once

#include <dolphin/types.h>

typedef struct EvtEntry EvtEntry;

#pragma enumsalwaysint off
typedef enum EvtOpcode {
	OP_InternalFetch = 0,
	OP_ScriptEnd,
	OP_Return,
	OP_Label
} EvtOpcode;
#pragma enumsalwaysint on

// Data types
#define EVTDAT_ADDR_MAX -270 * 1000000
#define EVTDAT_FLOAT_MAX -220 * 1000000
#define EVTDAT_UF_MAX -200 * 1000000
#define EVTDAT_UW_MAX -180 * 1000000
#define EVTDAT_GSW_MAX -160 * 1000000
#define EVTDAT_LSW_MAX -140 * 1000000
#define EVTDAT_GSWF_MAX -120 * 1000000
#define EVTDAT_LSWF_MAX -100 * 1000000
#define EVTDAT_GF_MAX -80 * 1000000
#define EVTDAT_LF_MAX -60 * 1000000
#define EVTDAT_GW_MAX -40 * 1000000
#define EVTDAT_LW_MAX -20 * 1000000

#define EVTDAT_FLOAT_BASE 230 * 1000000
#define EVTDAT_UF_BASE 210 * 1000000
#define EVTDAT_UW_BASE 190 * 1000000
#define EVTDAT_GSW_BASE 170 * 1000000
#define EVTDAT_LSW_BASE 150 * 1000000
#define EVTDAT_GSWF_BASE 130 * 1000000
#define EVTDAT_LSWF_BASE 110 * 1000000
#define EVTDAT_GF_BASE 90 * 1000000
#define EVTDAT_LF_BASE 70 * 1000000
#define EVTDAT_GW_BASE 50 * 1000000
#define EVTDAT_LW_BASE 30 * 1000000

#define UF(id) (id - EVTDAT_UF_BASE)
#define UW(id) (id - EVTDAT_UW_BASE)
#define GSW(id) (id - EVTDAT_GSW_BASE)
#define LSW(id) (id - EVTDAT_LSW_BASE)
#define GSWF(id) (id - EVTDAT_GSWF_BASE)
#define LSWF(id) (id - EVTDAT_LSWF_BASE)
#define GF(id) (id - EVTDAT_GF_BASE)
#define LF(id) (id - EVTDAT_LF_BASE)
#define GW(id) (id - EVTDAT_GW_BASE)
#define LW(id) (id - EVTDAT_LW_BASE)

s32 evtGetValue(EvtEntry* entry, s32 index);
s32 evtGetNumber(EvtEntry* entry, s32 index);
s32 evtSetValue(EvtEntry* entry, s32 index, s32 value);
f32 evtGetFloat(EvtEntry* entry, s32 index);
f32 evtSetFloat(EvtEntry* entry, s32 index, f32 value);