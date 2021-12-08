#pragma once

#include <dolphin/types.h>
#include "evt/evt_cmd.h"

typedef union BeroINFO {
	const char* name;
	s32 value;
} BeroINFO;

USERFUNC_DEF(evt_bero_set_reset_position);
USERFUNC_DEF(evt_bero_set_reset_position_current);
s32 bero_get_BeroEXEC(void);
s32 bero_get_BeroNUM(void);
f32 bero_get_BeroSX(void);
f32 bero_get_BeroSY(void);
f32 bero_get_BeroSZ(void);
f32 bero_get_BeroEX(void);
f32 bero_get_BeroEY(void);
f32 bero_get_BeroEZ(void);
BeroINFO** bero_get_ptr(void);
void bero_clear_Offset(void);
BeroINFO bero_id_filter(BeroINFO id);
USERFUNC_DEF(evt_bero_mapchange);













