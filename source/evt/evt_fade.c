#include "evt/evt_fade.h"

USERFUNC_DEF(evt_fade_entry) {
	s32* args = evt->args;
	FadeType type;
	s32 duration;
	u8 r, g, b;

	type = evtGetValue(evt, *args++);
	duration = evtGetValue(evt, *args++);
	r = evtGetValue(evt, *args++);
	g = evtGetValue(evt, *args++);
	b = evtGetValue(evt, *args++);
	fadeEntry(type, duration, (GXColor){r, g, b, 0xFF});
	return EVT_RETURN_DONE;
}