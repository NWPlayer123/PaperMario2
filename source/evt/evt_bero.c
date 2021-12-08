#include "evt/evt_bero.h"
#include "evt/evt_cmd.h"
#include "mario/mario.h"
#include "mario/mario_sbr.h"
#include "mgr/evtmgr.h"
#include <string.h>

//.bss
s32 beroSW[16];
BeroINFO* BeroINFOARR[16];

//.sbss, NOTE: all are reversed because of -inline deferred
s32 BeroEXEC;
s32 BeroNUM, BeroNOWNUM;
s32 BeroCAM; //only seems to be set in scripts
f32 BeroOX, BeroOY, BeroOZ; //offset position
f32 BeroSX, BeroSY, BeroSZ; //start position
f32 BeroEX, BeroEY, BeroEZ; //end position
Vec BeroPT, BeroAT;
BOOL BeroMarioGO;

//new in retail
USERFUNC_DEF(evt_bero_set_reset_position) {
	s32* args = evt->currCmdArgs;
	s32 x, y, z;

	x = evtGetValue(evt, args[0]);
	y = evtGetValue(evt, args[1]);
	z = evtGetValue(evt, args[2]);
	marioSetBottomlessResetPosition((f32)x, (f32)y, (f32)z);
	return EVT_RETURN_DONE;
}

//new in retail
USERFUNC_DEF(evt_bero_set_reset_position_current) {
	MarioWork* wp = marioGetPtr();
	marioSetBottomlessResetPosition(wp->position.x, wp->position.y, wp->position.z);
	return EVT_RETURN_DONE;
}

s32 bero_get_BeroEXEC(void) {
	return BeroEXEC;
}

s32 bero_get_BeroNUM(void) {
	return BeroNUM;
}

f32 bero_get_BeroSX(void) {
	return BeroSX;
}

f32 bero_get_BeroSY(void) {
	return BeroSY;
}

f32 bero_get_BeroSZ(void) {
	return BeroSZ;
}

f32 bero_get_BeroEX(void) {
	return BeroEX;
}

f32 bero_get_BeroEY(void) {
	return BeroEY;
}

f32 bero_get_BeroEZ(void) {
	return BeroEZ;
}

//TODO: re-type?
BeroINFO** bero_get_ptr(void) {
	return BeroINFOARR;
}

void bero_clear_Offset(void) {
	BeroOX = 0.0f;
	BeroOY = 0.0f;
	BeroOZ = 0.0f;
}

//close to 1:1, needs some tweaks
BeroINFO bero_id_filter(BeroINFO id) {
	BeroINFO** table = BeroINFOARR;
	const char* string;
	BeroINFO newId = id; //stack + 8

	if ((s32)&id < 0 || (s32)&id >= 16) {
		if (&newId != NULL) {
			newId.value = 0;
		}
		while (1) {
			string = (**table).name;
			if (!string || !strcmp(string, (const char*)&id)) {
				break;
			}
			table++;
			if (&newId != NULL) {
				newId.value++;
			}
		}
	}
	return newId;
}

USERFUNC_DEF(evt_bero_mapchange) {

}































