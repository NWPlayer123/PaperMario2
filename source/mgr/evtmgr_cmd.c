#include "mgr/evtmgr_cmd.h"
#include "mgr/evtmgr.h"
#include "drv/swdrv.h"
#include <dolphin/os.h>

extern int sprintf(char* str, const char* fmt, ...);

//.bss
static char str[0x100];

//local prototypes
inline EvtStatus evt_end_evt(EventEntry* evt); // 2
inline EvtStatus evt_lbl(EventEntry* evt); // 3
inline EvtStatus evt_goto(EventEntry* evt); // 4
inline EvtStatus evt_do(EventEntry* evt); // 5
EvtStatus evt_while(EventEntry* evt); // 6
inline EvtStatus evt_do_break(EventEntry* evt); // 7
inline EvtStatus evt_do_continue(EventEntry* evt); // 8
inline EvtStatus evt_wait_frm(EventEntry* evt); // 9
EvtStatus evt_wait_msec(EventEntry* evt); // 10
inline EvtStatus evt_halt(EventEntry* evt); // 11
EvtStatus evt_if_str_equal(EventEntry* evt); // 12
EvtStatus evt_if_str_not_equal(EventEntry* evt); // 13
EvtStatus evt_if_str_small(EventEntry* evt); // 14
EvtStatus evt_if_str_large(EventEntry* evt); // 15
EvtStatus evt_if_str_small_equal(EventEntry* evt); // 16
EvtStatus evt_if_str_large_equal(EventEntry* evt); // 17
EvtStatus evt_iff_equal(EventEntry* evt); // 18
EvtStatus evt_iff_not_equal(EventEntry* evt); // 19
EvtStatus evt_iff_small(EventEntry* evt); // 20
EvtStatus evt_iff_large(EventEntry* evt); // 21
EvtStatus evt_iff_small_equal(EventEntry* evt); // 22
EvtStatus evt_iff_large_equal(EventEntry* evt); // 23
EvtStatus evt_if_equal(EventEntry* evt); // 24
EvtStatus evt_if_not_equal(EventEntry* evt); // 25
EvtStatus evt_if_small(EventEntry* evt); // 26
EvtStatus evt_if_large(EventEntry* evt); // 27
EvtStatus evt_if_small_equal(EventEntry* evt); // 28
EvtStatus evt_if_large_equal(EventEntry* evt); // 29
EvtStatus evt_if_flag(EventEntry* evt); // 30
EvtStatus evt_if_not_flag(EventEntry* evt); // 31
inline EvtStatus evt_else(EventEntry* evt); // 32
inline EvtStatus evt_end_if(EventEntry* evt); // 33
inline EvtStatus evt_switch(EventEntry* evt); // 34
inline EvtStatus evt_switchi(EventEntry* evt); // 35
EvtStatus evt_case_equal(EventEntry* evt); // 36
EvtStatus evt_case_not_equal(EventEntry* evt); // 37
EvtStatus evt_case_small(EventEntry* evt); // 38
EvtStatus evt_case_small_equal(EventEntry* evt); // 39
EvtStatus evt_case_large(EventEntry* evt); // 40
EvtStatus evt_case_large_equal(EventEntry* evt); // 41
EvtStatus evt_case_between(EventEntry* evt); // 42
inline EvtStatus evt_case_etc(EventEntry* evt); // 43
EvtStatus evt_case_flag(EventEntry* evt); // 44
EvtStatus evt_case_or(EventEntry* evt); // 45
EvtStatus evt_case_and(EventEntry* evt); // 46
EvtStatus evt_case_end(EventEntry* evt); // 47
inline EvtStatus evt_switch_break(EventEntry* evt); // 48
inline EvtStatus evt_end_switch(EventEntry* evt); // 49
inline EvtStatus evt_set(EventEntry* evt); // 50
inline EvtStatus evt_seti(EventEntry* evt); // 51
inline EvtStatus evt_setf(EventEntry* evt); // 52
inline EvtStatus evt_add(EventEntry* evt); // 53
inline EvtStatus evt_sub(EventEntry* evt); // 54
inline EvtStatus evt_mul(EventEntry* evt); // 55
inline EvtStatus evt_div(EventEntry* evt); // 56
inline EvtStatus evt_mod(EventEntry* evt); // 57
inline EvtStatus evt_addf(EventEntry* evt); // 58
inline EvtStatus evt_subf(EventEntry* evt); // 59
inline EvtStatus evt_mulf(EventEntry* evt); // 60
inline EvtStatus evt_divf(EventEntry* evt); // 61
inline EvtStatus evt_set_read(EventEntry* evt); // 62
inline EvtStatus evt_set_readf(EventEntry* evt); // 63
inline EvtStatus evt_read(EventEntry* evt); // 64
inline EvtStatus evt_read2(EventEntry* evt); // 65
inline EvtStatus evt_read3(EventEntry* evt); // 66
inline EvtStatus evt_read4(EventEntry* evt); // 67
inline EvtStatus evt_read_n(EventEntry* evt); // 68
inline EvtStatus evt_readf(EventEntry* evt); // 69
inline EvtStatus evt_readf2(EventEntry* evt); // 70
inline EvtStatus evt_readf3(EventEntry* evt); // 71
inline EvtStatus evt_readf4(EventEntry* evt); // 72
inline EvtStatus evt_readf_n(EventEntry* evt); // 73
inline EvtStatus evt_set_user_wrk(EventEntry* evt); // 74
inline EvtStatus evt_set_user_flg(EventEntry* evt); // 75
inline EvtStatus evt_alloc_user_wrk(EventEntry* evt); // 76
inline EvtStatus evt_and(EventEntry* evt); // 77
inline EvtStatus evt_andi(EventEntry* evt); // 78
inline EvtStatus evt_or(EventEntry* evt); // 79
inline EvtStatus evt_ori(EventEntry* evt); // 80
inline EvtStatus evt_set_frame_from_msec(EventEntry* evt); // 81
inline EvtStatus evt_set_msec_from_frame(EventEntry* evt); // 82
inline EvtStatus evt_set_ram(EventEntry* evt); // 83
inline EvtStatus evt_set_ramf(EventEntry* evt); // 84
inline EvtStatus evt_get_ram(EventEntry* evt); // 85
inline EvtStatus evt_get_ramf(EventEntry* evt); // 86
inline EvtStatus evt_setr(EventEntry* evt); // 87
inline EvtStatus evt_setrf(EventEntry* evt); // 88
inline EvtStatus evt_getr(EventEntry* evt); // 89
inline EvtStatus evt_getrf(EventEntry* evt); // 90
inline EvtStatus evt_user_func(EventEntry* evt); // 91
EvtStatus evt_run_evt(EventEntry* evt); // 92
EvtStatus evt_run_evt_id(EventEntry* evt); // 93
inline EvtStatus evt_run_child_evt(EventEntry* evt); // 94
inline EvtStatus evt_restart_evt(EventEntry* evt); // 95
inline EvtStatus evt_delete_evt(EventEntry* evt); // 96
inline EvtStatus evt_set_pri(EventEntry* evt); // 97
inline EvtStatus evt_set_spd(EventEntry* evt); // 98
inline EvtStatus evt_set_type(EventEntry* evt); // 99
inline EvtStatus evt_stop_all(EventEntry* evt); // 100
inline EvtStatus evt_start_all(EventEntry* evt); // 101
inline EvtStatus evt_stop_other(EventEntry* evt); // 102
inline EvtStatus evt_start_other(EventEntry* evt); // 103
inline EvtStatus evt_stop_id(EventEntry* evt); // 104
inline EvtStatus evt_start_id(EventEntry* evt); // 105
inline EvtStatus evt_chk_evt(EventEntry* evt); // 106
EvtStatus evt_inline_evt(EventEntry* evt); // 107
EvtStatus evt_inline_evt_id(EventEntry* evt); // 108
inline EvtStatus evt_end_inline(EventEntry* evt); // 109
EvtStatus evt_brother_evt(EventEntry* evt); // 110
EvtStatus evt_brother_evt_id(EventEntry* evt); // 111
inline EvtStatus evt_end_brother(EventEntry* evt); // 112
inline EvtStatus evt_debug_put_msg(EventEntry* evt); // 113
inline EvtStatus evt_debug_msg_clear(EventEntry* evt); // 114
EvtStatus evt_debug_put_reg(EventEntry* evt); // 115
inline EvtStatus evt_debug_name(EventEntry* evt); // 116
inline EvtStatus evt_debug_rem(EventEntry* evt); // 117
inline EvtStatus evt_debug_bp(EventEntry* evt); // 118

inline s32* evtSearchLabel(EventEntry* evt, s32 id);
inline s32* evtSearchBreakLoop(EventEntry* evt); //TODO: find right name
inline s32* evtSearchContinueLoop(EventEntry* evt);

static inline f32 check_float(s32 val) { // always inlined
	if (val <= EVTDAT_FLOAT_MAX) {
		return (val + EVTDAT_FLOAT_BASE) / 1024.0f;
	}
	else {
		return val;
	}
}

//almost 1:1, missing additional 1024.0 load from check_float
f32 evtSetFloat(EventEntry* entry, s32 index, f32 value) {
	s32 retval;
	s32 shift;

	EventWork* wp = evtGetWork();
	if (index <= EVTDAT_ADDR_MAX) {
		return value;
	}
	else if (index <= EVTDAT_FLOAT_MAX) {
		return value;
	}
	else if (index <= EVTDAT_UW_MAX) {
		index += EVTDAT_UW_BASE;
		retval = entry->uwBase[index];
		entry->uwBase[index] = (s32)(value / 1024.0f) + EVTDAT_FLOAT_BASE;
		return check_float(retval);
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		shift = index % 32;
		if (value != 0.0f) { //inverted check
			wp->gfData[index / 32] &= ~(1 << shift);
		}
		else {
			wp->gfData[index / 32] |= (1 << shift);
		}
		return value;
	}
	else if (index <= EVTDAT_LF_MAX) {
		index += EVTDAT_LF_BASE;
		shift = index % 32;
		if (value != 0.0f) { //inverted check
			entry->lfData[index / 32] &= ~(1 << shift);
		}
		else {
			entry->lfData[index / 32] |= (1 << shift);
		}
		return value;
	}
	else if (index <= EVTDAT_GW_MAX) {
		index += EVTDAT_GW_BASE;
		retval = wp->gwData[index];
		wp->gwData[index] = (s32)(value / 1024.0f) + EVTDAT_FLOAT_BASE;
		return check_float(retval);
	}
	else if (index <= EVTDAT_LW_MAX) {
		index += EVTDAT_LW_BASE;
		retval = entry->lwData[index];
		entry->lwData[index] = (s32)(value / 1024.0f) + EVTDAT_FLOAT_BASE;
		return check_float(retval);
	}
	else {
		return value;
	}
}

//TODO: move below evtSetFloat, -inline deferred
inline s32* evtSearchLabel(EventEntry* evt, s32 id) {
	s32 i, *ptr;

	ptr = evt->currCmdArgs;
	if (id < EVTDAT_ADDR_MAX) {
		return ptr;
	}

	for (i = 0; i < 0x10; i++) {
		if (evt->labelIdTable[i] == id) {
			ptr = evt->labelAddressTable[i];
			break;
		}
	}

	return ptr;
}

//TODO: determine which symbol based on context
inline s32* evtSearchBreakLoop(EventEntry* evt) {
	u32 temp, opcode; //TODO: rename temp
	s32 param_count, *ptr;

	temp = 0;
	ptr = evt->nextCommand;
	while (1) {
		opcode = (u32)(*ptr & 0xFFFF);
		param_count = *ptr >> 16;
		ptr += param_count + 1; //should shift 2 for index
		if (opcode == OP_LoopBegin) {
			temp++;
		}
		else if (opcode == OP_ScriptEnd || opcode == OP_LoopIterate) {
			if (--temp < 0) break; //exit loop
		}
	}
	return ptr;
}

inline s32* evtSearchContinueLoop(EventEntry* evt) {
	u32 temp, opcode; //TODO: rename temp
	s32 param_count, * ptr;

	temp = 0;
	ptr = evt->nextCommand;
	while (1) {
		opcode = (u32)(*ptr & 0xFFFF);
		if (opcode == OP_LoopBegin) {
			temp++;
		}
		else if (opcode == OP_ScriptEnd || opcode == OP_LoopIterate) {
			if (--temp < 0) break; //exit loop
		}
		param_count = *ptr >> 16;
		ptr += param_count + 1; //should shift 2 for index
	}
	return ptr;
}

EvtStatus evt_end_evt(EventEntry* evt) { // 2
	evtDelete(evt);
	return EVT_RETURN_FINISH;
}

EvtStatus evt_lbl(EventEntry* evt) { // 3
	return EVT_RETURN_DONE;
}

//double check after we get status fixed
EvtStatus evt_goto(EventEntry* evt) { // 4
	evt->nextCommand = evtSearchLabel(evt, evtGetValue(evt, *evt->currCmdArgs));
	return EVT_RETURN_DONE;
}

EvtStatus evt_do(EventEntry* evt) { // 5
	s32 var, *args;
	s8 loopDepth;

	args = evt->currCmdArgs;
	var = *args++;
	evt->loopDepth++;
	loopDepth = evt->loopDepth;

	evt->loopStartTable[loopDepth] = args;
	evt->loopCounterTable[loopDepth] = var;
	return EVT_RETURN_DONE;
}

//TODO: condense all returns to final line?
EvtStatus evt_while(EventEntry* evt) { // 6, 1:1, not inlined
	s32 loopDepth = evt->loopDepth;
	s32 loopCounter;

	loopCounter = evt->loopCounterTable[loopDepth];

	if (!loopCounter) {
		evt->nextCommand = evt->loopStartTable[loopDepth];
		return EVT_RETURN_DONE;
	}

	if (loopCounter >= -10 * 1000000) {
		evt->loopCounterTable[loopDepth] = --loopCounter;
	}
	else {
		s32 var = evtGetValue(evt, loopCounter);
		evtSetValue(evt, loopCounter, var - 1);
		loopCounter = var - 1;
	}

	if (loopCounter) {
		evt->nextCommand = evt->loopStartTable[loopDepth];
		return EVT_RETURN_DONE;
	}
	else {
		evt->loopDepth--;
		return EVT_RETURN_DONE;
	}
}

EvtStatus evt_do_break(EventEntry* evt) { // 7

	evt->nextCommand = evtSearchBreakLoop(evt);
	evt->loopDepth--;
	return EVT_RETURN_DONE;
}

EvtStatus evt_do_continue(EventEntry* evt) { // 8

	evt->nextCommand = evtSearchContinueLoop(evt);
	return EVT_RETURN_DONE;
}

EvtStatus evt_wait_frm(EventEntry* evt) { // 9
	if (!evt->blocked) {
		evt->userData[0] = evtGetValue(evt, *evt->currCmdArgs);
		evt->blocked = 1;
	}
	if (!evt->userData[0]) {
		return EVT_RETURN_DONE;
	}

	evt->userData[0]--;
	if (!evt->userData[0]) {
		return EVT_RETURN_BLOCK;
	}
	else {
		return EVT_RETURN_YIELD;
	}
}
typedef union time_cast {
	OSTime time;
	struct {
		OSTick upper;
		OSTick lower;
	};
} time_cast;

EvtStatus evt_wait_msec(EventEntry* evt) { // 10
	u8 blocked;
	s32* args;
	time_cast time;

	blocked = evt->blocked;
	args = evt->currCmdArgs;
	time.time = evt->timeSinceStart;
	if (!blocked) {
		evt->userData[0] = evtGetValue(evt, *args);
		evt->userData[1] = (s32)time.upper;
		evt->userData[2] = (s32)time.lower;
		evt->blocked = 1;
	}
	if (!evt->userData[0]) {
		return EVT_RETURN_DONE;
	}
	return OSTicksToMilliseconds(time.lower - evt->userData[2]) >= evt->userData[0];
	

	/*if (!evt->blocked) {
		evt->userData[0] = evtGetValue(evt, *evt->currCmdArgs);
		evt->userData[1] = evt->timeSinceStart >> 32;
		evt->userData[2] = evt->timeSinceStart;
		evt->blocked = 1;
	}
	if (!evt->userData[0]) {
		return EVT_RETURN_DONE;
	}

	return OSTicksToMilliseconds(*(OSTime*)&evt->userData[2]);

	evt->userData[0]--;
	if (!evt->userData[0]) {
		return EVT_RETURN_BLOCK;
	}
	else {
		return EVT_RETURN_YIELD;
	}*/
	return EVT_RETURN_DONE;
}

EvtStatus evt_halt(EventEntry* evt) { // 11
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_str_equal(EventEntry* evt) { // 12
	s32* args;
	const char *str1, *str2;

	args = evt->currCmdArgs;
	str1 = (const char*)evtGetValue(evt, args[0]);
	str2 = (const char*)evtGetValue(evt, args[1]);
	if (!str1) {
		str1 = "";
	}
	if (!str2) {
		str2 = "";
	}
	/*if (strcmp(str1, str2)) {
		while (1) {
			switch (0) {
			case 0x1:
			case 0x21:
				r5 -= 1;
				continue;
			case 0xC:
			case 0xD:
			case 0xE:
			case 0xF:
			case 0x10:
			case 0x11:
			case 0x12:
			case 0x13:
			case 0x14:
			case 0x15:
			case 0x16:
			case 0x17:
			case 0x18:
			case 0x19:
				r5 += 1;
				continue;
			case 0x20:
				if (!r5) {
					break;
				}
				continue;
		}
	}*/
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_str_not_equal(EventEntry* evt) { // 13
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_str_small(EventEntry* evt) { // 14
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_str_large(EventEntry* evt) { // 15
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_str_small_equal(EventEntry* evt) { // 16
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_str_large_equal(EventEntry* evt) { // 17
	return EVT_RETURN_DONE;
}

EvtStatus evt_iff_equal(EventEntry* evt) { // 18
	return EVT_RETURN_DONE;
}

EvtStatus evt_iff_not_equal(EventEntry* evt) { // 19
	return EVT_RETURN_DONE;
}

EvtStatus evt_iff_small(EventEntry* evt) { // 20
	return EVT_RETURN_DONE;
}

EvtStatus evt_iff_large(EventEntry* evt) { // 21
	return EVT_RETURN_DONE;
}

EvtStatus evt_iff_small_equal(EventEntry* evt) { // 22
	return EVT_RETURN_DONE;
}

EvtStatus evt_iff_large_equal(EventEntry* evt) { // 23
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_equal(EventEntry* evt) { // 24
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_not_equal(EventEntry* evt) { // 25
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_small(EventEntry* evt) { // 26
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_large(EventEntry* evt) { // 27
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_small_equal(EventEntry* evt) { // 28
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_large_equal(EventEntry* evt) { // 29
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_flag(EventEntry* evt) { // 30
	return EVT_RETURN_DONE;
}

EvtStatus evt_if_not_flag(EventEntry* evt) { // 31
	return EVT_RETURN_DONE;
}

EvtStatus evt_else(EventEntry* evt) { // 32
	return EVT_RETURN_DONE;
}

EvtStatus evt_end_if(EventEntry* evt) { // 33
	return EVT_RETURN_DONE;
}

EvtStatus evt_switch(EventEntry* evt) { // 34
	return EVT_RETURN_DONE;
}

EvtStatus evt_switchi(EventEntry* evt) { // 35
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_equal(EventEntry* evt) { // 36
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_not_equal(EventEntry* evt) { // 37
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_small(EventEntry* evt) { // 38
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_small_equal(EventEntry* evt) { // 39
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_large(EventEntry* evt) { // 40
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_large_equal(EventEntry* evt) { // 41
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_between(EventEntry* evt) { // 42
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_etc(EventEntry* evt) { // 43
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_flag(EventEntry* evt) { // 44
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_or(EventEntry* evt) { // 45
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_and(EventEntry* evt) { // 46
	return EVT_RETURN_DONE;
}

EvtStatus evt_case_end(EventEntry* evt) { // 47
	return EVT_RETURN_DONE;
}

EvtStatus evt_switch_break(EventEntry* evt) { // 48
	return EVT_RETURN_DONE;
}

EvtStatus evt_end_switch(EventEntry* evt) { // 49
	return EVT_RETURN_DONE;
}

EvtStatus evt_set(EventEntry* evt) { // 50, 1:1
	s32 index = evt->currCmdArgs[0];
	s32 value = evt->currCmdArgs[1];
	evtSetValue(evt, index, evtGetValue(evt, value));
	return EVT_RETURN_DONE;
}

EvtStatus evt_seti(EventEntry* evt) { // 51
	evtSetValue(evt, evt->currCmdArgs[0], evt->currCmdArgs[1]);
	return EVT_RETURN_DONE;
}

EvtStatus evt_setf(EventEntry* evt) { // 52
	s32 index = evt->currCmdArgs[0];
	s32 value = evt->currCmdArgs[1];
	evtSetFloat(evt, index, evtGetFloat(evt, value));
	return EVT_RETURN_DONE;
}

EvtStatus evt_add(EventEntry* evt) { // 53
	return EVT_RETURN_DONE;
}

EvtStatus evt_sub(EventEntry* evt) { // 54
	return EVT_RETURN_DONE;
}

EvtStatus evt_mul(EventEntry* evt) { // 55
	return EVT_RETURN_DONE;
}

EvtStatus evt_div(EventEntry* evt) { // 56
	return EVT_RETURN_DONE;
}

EvtStatus evt_mod(EventEntry* evt) { // 57
	return EVT_RETURN_DONE;
}

EvtStatus evt_addf(EventEntry* evt) { // 58
	return EVT_RETURN_DONE;
}

EvtStatus evt_subf(EventEntry* evt) { // 59
	return EVT_RETURN_DONE;
}

EvtStatus evt_mulf(EventEntry* evt) { // 60
	return EVT_RETURN_DONE;
}

EvtStatus evt_divf(EventEntry* evt) { // 61
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_read(EventEntry* evt) { // 62
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_readf(EventEntry* evt) { // 63
	return EVT_RETURN_DONE;
}

EvtStatus evt_read(EventEntry* evt) { // 64
	return EVT_RETURN_DONE;
}

EvtStatus evt_read2(EventEntry* evt) { // 65
	return EVT_RETURN_DONE;
}

EvtStatus evt_read3(EventEntry* evt) { // 66
	return EVT_RETURN_DONE;
}

EvtStatus evt_read4(EventEntry* evt) { // 67
	return EVT_RETURN_DONE;
}

EvtStatus evt_read_n(EventEntry* evt) { // 68
	return EVT_RETURN_DONE;
}

EvtStatus evt_readf(EventEntry* evt) { // 69
	return EVT_RETURN_DONE;
}

EvtStatus evt_readf2(EventEntry* evt) { // 70
	return EVT_RETURN_DONE;
}

EvtStatus evt_readf3(EventEntry* evt) { // 71
	return EVT_RETURN_DONE;
}

EvtStatus evt_readf4(EventEntry* evt) { // 72
	return EVT_RETURN_DONE;
}

EvtStatus evt_readf_n(EventEntry* evt) { // 73
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_user_wrk(EventEntry* evt) { // 74
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_user_flg(EventEntry* evt) { // 75
	return EVT_RETURN_DONE;
}

EvtStatus evt_alloc_user_wrk(EventEntry* evt) { // 76
	return EVT_RETURN_DONE;
}

EvtStatus evt_and(EventEntry* evt) { // 77
	return EVT_RETURN_DONE;
}

EvtStatus evt_andi(EventEntry* evt) { // 78
	return EVT_RETURN_DONE;
}

EvtStatus evt_or(EventEntry* evt) { // 79
	return EVT_RETURN_DONE;
}

EvtStatus evt_ori(EventEntry* evt) { // 80
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_frame_from_msec(EventEntry* evt) { // 81
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_msec_from_frame(EventEntry* evt) { // 82
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_ram(EventEntry* evt) { // 83
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_ramf(EventEntry* evt) { // 84
	return EVT_RETURN_DONE;
}

EvtStatus evt_get_ram(EventEntry* evt) { // 85
	return EVT_RETURN_DONE;
}

EvtStatus evt_get_ramf(EventEntry* evt) { // 86
	return EVT_RETURN_DONE;
}

EvtStatus evt_setr(EventEntry* evt) { // 87
	return EVT_RETURN_DONE;
}

EvtStatus evt_setrf(EventEntry* evt) { // 88
	return EVT_RETURN_DONE;
}

EvtStatus evt_getr(EventEntry* evt) { // 89
	return EVT_RETURN_DONE;
}

EvtStatus evt_getrf(EventEntry* evt) { // 90
	return EVT_RETURN_DONE;
}

EvtStatus evt_user_func(EventEntry* evt) { // 91
	if (evt->blocked) {
		return evt->user_func(evt, FALSE);
	}
	else {
		evt->user_func = (UserFunction)evtGetValue(evt, *evt->currCmdArgs);
		evt->paramCount--;
		evt->currCmdArgs++;
		evt->blocked = 1;
		return evt->user_func(evt, TRUE);
	}
}

EvtStatus evt_run_evt(EventEntry* evt) { // 92
	return EVT_RETURN_DONE;
}

EvtStatus evt_run_evt_id(EventEntry* evt) { // 93
	return EVT_RETURN_DONE;
}

EvtStatus evt_run_child_evt(EventEntry* evt) { // 94
	return EVT_RETURN_DONE;
}

EvtStatus evt_restart_evt(EventEntry* evt) { // 95
	return EVT_RETURN_DONE;
}

EvtStatus evt_delete_evt(EventEntry* evt) { // 96
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_pri(EventEntry* evt) { // 97
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_spd(EventEntry* evt) { // 98
	return EVT_RETURN_DONE;
}

EvtStatus evt_set_type(EventEntry* evt) { // 99
	return EVT_RETURN_DONE;
}

EvtStatus evt_stop_all(EventEntry* evt) { // 100
	return EVT_RETURN_DONE;
}

EvtStatus evt_start_all(EventEntry* evt) { // 101
	return EVT_RETURN_DONE;
}

EvtStatus evt_stop_other(EventEntry* evt) { // 102
	return EVT_RETURN_DONE;
}

EvtStatus evt_start_other(EventEntry* evt) { // 103
	return EVT_RETURN_DONE;
}

EvtStatus evt_stop_id(EventEntry* evt) { // 104
	return EVT_RETURN_DONE;
}

EvtStatus evt_start_id(EventEntry* evt) { // 105
	return EVT_RETURN_DONE;
}

EvtStatus evt_chk_evt(EventEntry* evt) { // 106
	return EVT_RETURN_DONE;
}

EvtStatus evt_inline_evt(EventEntry* evt) { // 107
	return EVT_RETURN_DONE;
}

EvtStatus evt_inline_evt_id(EventEntry* evt) { // 108
	return EVT_RETURN_DONE;
}

EvtStatus evt_end_inline(EventEntry* evt) { // 109
	return EVT_RETURN_DONE;
}

EvtStatus evt_brother_evt(EventEntry* evt) { // 110
	return EVT_RETURN_DONE;
}

EvtStatus evt_brother_evt_id(EventEntry* evt) { // 111
	return EVT_RETURN_DONE;
}

EvtStatus evt_end_brother(EventEntry* evt) { // 112
	return EVT_RETURN_DONE;
}

EvtStatus evt_debug_put_msg(EventEntry* evt) { // 113
	return EVT_RETURN_DONE;
}

EvtStatus evt_debug_msg_clear(EventEntry* evt) { // 114
	return EVT_RETURN_DONE;
}

EvtStatus evt_debug_put_reg(EventEntry* evt) { // 115
	EventWork* wp = evtGetWork();
	s32 reg = evt->currCmdArgs[0];
	s32 data, mask, val;

	if (reg <= EVTDAT_ADDR_MAX) {
		sprintf(str, "ADDR     [%08X]", reg);
	}
	else if (reg <= EVTDAT_FLOAT_MAX) {
		sprintf(str, "FLOAT    [%4.2f]", check_float(reg));
	}
	else if (reg <= EVTDAT_UF_MAX) {
		reg += EVTDAT_UF_BASE;

		mask = 1 << (reg % 32);
		data = evt->ufBase[reg / 32];

		sprintf(str, "UF(%3d)  [%d]", reg, mask & data);
	}
	else if (reg <= EVTDAT_UW_MAX) {
		reg += EVTDAT_UW_BASE;

		val = evt->uwBase[reg];
		if (val <= EVTDAT_ADDR_MAX) {
			sprintf(str, "UW(%3d)  [%08X]", val, val);
		}
		else if (val <= EVTDAT_FLOAT_MAX) {
			sprintf(str, "UW(%3d)  [%4.2f]", reg, check_float(val));
		}
		else {
			sprintf(str, "UW(%3d)  [%d]", reg, val);
		}
	}
	else if (reg <= EVTDAT_GSW_MAX) {
		reg += EVTDAT_GSW_BASE;

		val = swByteGet(reg);
		if (val <= EVTDAT_ADDR_MAX) {
			sprintf(str, "GSW(%3d) [%08X]", val, val);
		}
		else if (val <= EVTDAT_FLOAT_MAX) {
			sprintf(str, "GSW(%3d) [%4.2f]", reg, check_float(val));
		}
		else {
			sprintf(str, "GSW(%3d) [%d]", reg, val);
		}
	}
	else if (reg <= EVTDAT_LSW_MAX) {
		reg += EVTDAT_LSW_BASE;

		val = _swByteGet(reg);
		if (val <= EVTDAT_ADDR_MAX) {
			sprintf(str, "LSW(%3d) [%08X]", val, val);
		}
		else if (val <= EVTDAT_FLOAT_MAX) {
			sprintf(str, "LSW(%3d)  [%4.2f]", reg, check_float(val));
		}
		else {
			sprintf(str, "LSW(%3d) [%d]", reg, val);
		}
	}
	else if (reg <= EVTDAT_GSWF_MAX) {
		reg += EVTDAT_GSWF_BASE;

		sprintf(str, "GSWF(%3d)[%d]", reg, swGet(reg));
	}
	else if (reg <= EVTDAT_LSWF_MAX) {
		reg += EVTDAT_LSWF_BASE;

		sprintf(str, "LSWF(%3d)[%d]", reg, _swGet(reg));
	}
	else if (reg <= EVTDAT_GF_MAX) {
		reg += EVTDAT_GF_BASE;

		mask = 1 << (reg % 32);
		data = wp->gfData[reg / 32];

		sprintf(str, "GF(%3d)  [%d]", reg, mask & data);
	}
	else if (reg <= EVTDAT_LF_MAX) {
		reg += EVTDAT_LF_BASE;

		mask = 1 << (reg % 32);
		data = evt->lfData[reg / 32];

		sprintf(str, "LF(%3d)  [%d]", reg, mask & data);
	}
	else if (reg <= EVTDAT_GW_MAX) {
		reg += EVTDAT_GW_BASE;

		val = wp->gwData[reg];
		if (val <= EVTDAT_ADDR_MAX) {
			sprintf(str, "GW(%3d)  [%08X]", reg, val);
		}
		else if (val <= EVTDAT_FLOAT_MAX) {
			sprintf(str, "GW(%3d)  [%4.2f]", reg, check_float(val));
		}
		else {
			sprintf(str, "GW(%3d)  [%d]", reg, val);
		}
	}
	else if (reg <= EVTDAT_LW_MAX) {
		reg += EVTDAT_LW_BASE;

		val = evt->lwData[reg];
		if (val <= EVTDAT_ADDR_MAX) {
			sprintf(str, "LW(%3d)  [%08X]", reg, val);
		}
		else if (val <= EVTDAT_FLOAT_MAX) {
			sprintf(str, "LW(%3d)  [%4.2f]", reg, check_float(val));
		}
		else {
			sprintf(str, "LW(%3d)  [%d]", reg, val);
		}
	}
	else {
		sprintf(str, "         [%d]", reg);
	}
	
	return EVT_RETURN_DONE;
}

EvtStatus evt_debug_name(EventEntry* evt) { // 116
	evt->name = *(const char**)evt->currCmdArgs;
	return EVT_RETURN_DONE;
}

EvtStatus evt_debug_rem(EventEntry* evt) { // 117, 1:1
	return EVT_RETURN_DONE;
}

EvtStatus evt_debug_bp(EventEntry* evt) { // 118
	int i;

	for (i = 0; i < 0x100; i++) {
		if (evtGetPtr(i) == evt) {
			break;
		}
	}
	return EVT_RETURN_YIELD;
}

//TODO: double check OP_LoopBreak(7), OP_LoopContinue(8)
s32 evtmgrCmd(EventEntry* evt) {
	s32* header;
	s32 status;
	s32 param_count;

	while (1) {
		status = EVT_RETURN_DONE;
		switch (evt->opcode) {
		case OP_InternalFetch: //0
			status = EVT_RETURN_REPEAT;
			evt->currCommand = evt->nextCommand;
			header = evt->nextCommand;
			evt->opcode = (u8)*header;
			param_count = (*header++ >> 16);
			evt->paramCount = (u8)param_count;
			evt->currCmdArgs = header;
			evt->nextCommand = &header[param_count];
			evt->blocked = 0;
			break;

		case OP_Return: // 2
			status = evt_end_evt(evt);
			break;

		case OP_Label: // 3
			status = evt_lbl(evt);
			break;

		case OP_Goto: // 4
			status = evt_goto(evt);
			break;

		case OP_LoopBegin: // 5
			status = evt_do(evt);
			break;

		case OP_LoopIterate: // 6
			status = evt_while(evt);
			break;

		case OP_LoopBreak: // 7
			status = evt_do_break(evt);
			break;

		case OP_LoopContinue: // 8
			status = evt_do_continue(evt);
			break;

		case OP_WaitFrames: // 9
			status = evt_wait_frm(evt);
			break;

		case OP_WaitMS: // 10
			status = evt_wait_msec(evt);
			break;

		case OP_WaitUntil: // 11
			status = evt_halt(evt);
			break;

		case OP_IfStringEqual: // 12
			status = evt_if_str_equal(evt);
			break;

		case OP_IfStringNotEqual: // 13
			status = evt_if_str_not_equal(evt);
			break;

		case OP_IfStringLess: // 14
			status = evt_if_str_small(evt);
			break;

		case OP_IfStringGreater: // 15
			status = evt_if_str_large(evt);
			break;

		case OP_IfStringLessEqual: // 16
			status = evt_if_str_small_equal(evt);
			break;

		case OP_IfStringGreaterEqual: // 17
			status = evt_if_str_large_equal(evt);
			break;

		case OP_IfFloatEqual: // 18
			status = evt_iff_equal(evt);
			break;

		case OP_IfFloatNotEqual: // 19
			status = evt_iff_not_equal(evt);
			break;

		case OP_IfFloatLess: // 20
			status = evt_iff_small(evt);
			break;

		case OP_IfFloatGreater: // 21
			status = evt_iff_large(evt);
			break;

		case OP_IfFloatLessEqual: // 22
			status = evt_iff_small_equal(evt);
			break;

		case OP_IfFloatGreaterEqual: // 23
			status = evt_iff_large_equal(evt);
			break;

		case OP_IfIntEqual: // 24
			status = evt_if_equal(evt);
			break;

		case OP_IfIntNotEqual: // 25
			status = evt_if_not_equal(evt);
			break;

		case OP_IfIntLess: // 26
			status = evt_if_small(evt);
			break;

		case OP_IfIntGreater: // 27
			status = evt_if_large(evt);
			break;

		case OP_IfIntLessEqual: // 28
			status = evt_if_small_equal(evt);
			break;

		case OP_IfIntGreaterEqual: // 29
			status = evt_if_large_equal(evt);
			break;

		case OP_IfBitsSet: // 30
			status = evt_if_flag(evt);
			break;

		case OP_IfBitsClear: // 31
			status = evt_if_not_flag(evt);
			break;

		case OP_Else: // 32
			status = evt_else(evt);
			break;

		case OP_EndIf: // 33
			status = evt_end_if(evt);
			break;

		case OP_SwitchExpr: // 34
			status = evt_switch(evt);
			break;

		case OP_SwitchRaw: // 35
			status = evt_switchi(evt);
			break;

		case OP_CaseIntEqual: // 36
			status = evt_case_equal(evt);
			break;

		case OP_CaseIntNotEqual: // 37
			status = evt_case_not_equal(evt);
			break;

		case OP_CaseIntLess: // 38
			status = evt_case_small(evt);
			break;

		case OP_CaseIntGreater: // 39
			status = evt_case_small_equal(evt);
			break;

		case OP_CaseIntLessEqual: // 40
			status = evt_case_large(evt);
			break;

		case OP_CaseIntGreaterEqual: // 41
			status = evt_case_large_equal(evt);
			break;

		case OP_CaseDefault: // 42
			status = evt_case_between(evt);
			break;

		case OP_CaseIntEqualAny: // 43
			status = evt_case_etc(evt);
			break;

		case OP_CaseIntNotEqualAll: // 44
			status = evt_case_flag(evt);
			break;

		case OP_CaseBitsSet: // 45
			status = evt_case_or(evt);
			break;

		case OP_EndMultiCase: // 46
			status = evt_case_and(evt);
			break;

		case OP_CaseIntRange: // 47
			status = evt_case_end(evt);
			break;

		case OP_SwitchBreak: // 48
			status = evt_switch_break(evt);
			break;

		case OP_EndSwitch: // 49
			status = evt_end_switch(evt);
			break;

		case OP_SetExprIntToExprInt: // 50
			status = evt_set(evt);
			break;

		case OP_SetExprIntToRaw: // 51
			status = evt_seti(evt);
			break;

		case OP_SetExprFloatToExprFloat: // 52
			status = evt_setf(evt);
			break;

		case OP_AddInt: // 53
			status = evt_add(evt);
			break;

		case OP_SubtractInt: // 54
			status = evt_sub(evt);
			break;

		case OP_MultiplyInt: // 55
			status = evt_mul(evt);
			break;

		case OP_DivideInt: // 56
			status = evt_div(evt);
			break;

		case OP_ModuloInt: // 57
			status = evt_mod(evt);
			break;

		case OP_AddFloat: // 58
			status = evt_addf(evt);
			break;

		case OP_SubtractFloat: // 59
			status = evt_subf(evt);
			break;

		case OP_MultiplyFloat: // 60
			status = evt_mulf(evt);
			break;

		case OP_DivideFloat: // 61
			status = evt_divf(evt);
			break;

		case OP_MemOpSetBaseInt: // 62
			status = evt_set_read(evt);
			break;

		case OP_MemOpReadInt: // 63
			status = evt_set_readf(evt);
			break;

		case OP_MemOpReadInt2: // 64
			status = evt_read(evt);
			break;

		case OP_MemOpReadInt3: // 65
			status = evt_read2(evt);
			break;

		case OP_MemOpReadInt4: // 66
			status = evt_read3(evt);
			break;

		case OP_MemOpReadIntIndexed: // 67
			status = evt_read4(evt);
			break;

		case OP_MemOpSetBaseFloat: // 68
			status = evt_read_n(evt);
			break;

		case OP_MemOpReadFloat: // 69
			status = evt_readf(evt);
			break;

		case OP_MemOpReadFloat2: // 70
			status = evt_readf2(evt);
			break;

		case OP_MemOpReadFloat3: // 71
			status = evt_readf3(evt);
			break;

		case OP_MemOpReadFloat4: // 72
			status = evt_readf4(evt);
			break;

		case OP_MemOpReadFloatIndexed: // 73
			status = evt_readf_n(evt);
			break;

		case OP_SetUserWordBase: // 74
			status = evt_set_user_wrk(evt);
			break;

		case OP_SetUserFlagBase: // 75
			status = evt_set_user_flg(evt);
			break;

		case OP_AllocateUserWordBase: // 76
			status = evt_alloc_user_wrk(evt);
			break;

		case OP_AndExpr: // 77
			status = evt_and(evt);
			break;

		case OP_AndRaw: // 78
			status = evt_andi(evt);
			break;

		case OP_OrExpr: // 79
			status = evt_or(evt);
			break;

		case OP_OrRaw: // 80
			status = evt_ori(evt);
			break;

		case OP_ConvertMSToFrames: // 81
			status = evt_set_frame_from_msec(evt);
			break;

		case OP_ConvertFramesToMS: // 82
			status = evt_set_msec_from_frame(evt);
			break;

		case OP_StoreIntToPtr: // 83
			status = evt_set_ram(evt);
			break;

		case OP_StoreFloatToPtr: // 84
			status = evt_set_ramf(evt);
			break;

		case OP_LoadIntFromPtr: // 85
			status = evt_get_ram(evt);
			break;

		case OP_LoadFloatFromPtr: // 86
			status = evt_get_ramf(evt);
			break;

		case OP_StoreIntToPtrExpr: // 87
			status = evt_setr(evt);
			break;

		case OP_StoreFloatToPtrExpr: // 88
			status = evt_setrf(evt);
			break;

		case OP_LoadIntFromPtrExpr: // 89
			status = evt_getr(evt);
			break;

		case OP_LoadFloatFromPtrExpr: // 90
			status = evt_getrf(evt);
			break;

		case OP_CallCppSync: // 91
			status = evt_user_func(evt);
			break;

		case OP_CallScriptAsync: // 92
			status = evt_run_evt(evt);
			break;

		case OP_CallScriptAsyncSaveTID: // 93
			status = evt_run_evt_id(evt);
			break;

		case OP_CallScriptSync: // 94
			status = evt_run_child_evt(evt);
			break;

		case OP_TerminateThread: // 95
			status = evt_restart_evt(evt);
			break;

		case OP_Jump: // 96
			status = evt_delete_evt(evt);
			break;

		case OP_SetThreadPriority: // 97
			status = evt_set_pri(evt);
			break;

		case OP_SetThreadTimeQuantum: // 98
			status = evt_set_spd(evt);
			break;

		case OP_SetThreadTypeMask: // 99
			status = evt_set_type(evt);
			break;

		case OP_ThreadSuspendTypes: // 100
			status = evt_stop_all(evt);
			break;

		case OP_ThreadResumeTypes: // 101
			status = evt_start_all(evt);
			break;

		case OP_ThreadSuspendTypesOther: // 102
			status = evt_stop_other(evt);
			break;

		case OP_ThreadResumeTypesOther: // 103
			status = evt_start_other(evt);
			break;

		case OP_ThreadSuspendTID: // 104
			status = evt_stop_id(evt);
			break;

		case OP_ThreadResumeTID: // 105
			status = evt_start_id(evt);
			break;

		case OP_CheckThreadRunning: // 106
			status = evt_chk_evt(evt);
			break;

		case OP_ThreadStart: // 107
			status = evt_inline_evt(evt);
			break;

		case OP_ThreadStartSaveTID: // 108
			status = evt_inline_evt_id(evt);
			break;

		case OP_ThreadEnd: // 109
			status = evt_end_inline(evt);
			break;

		case OP_ThreadChildStart: // 110
			status = evt_brother_evt(evt);
			break;

		case OP_ThreadChildStartSaveTID: // 111
			status = evt_brother_evt_id(evt);
			break;

		case OP_ThreadChildEnd: // 112
			status = evt_end_brother(evt);
			break;

		case OP_DebugOutputString: // 113
			status = evt_debug_put_msg(evt);
			break;

		case OP_DebugUnk1: // 114
			status = evt_debug_msg_clear(evt);
			break;

		case OP_DebugExprToString: // 115
			status = evt_debug_put_reg(evt);
			break;

		case OP_DebugUnk2: // 116
			status = evt_debug_name(evt);
			break;

		case OP_DebugUnk3: // 117
			status = evt_debug_rem(evt);
			break;

		case OP_DebugUnk4: // 118
			status = evt_debug_bp(evt);
			break;
		}

		if (status == EVT_RETURN_REPEAT) {
			continue;
		}
		if (status == EVT_RETURN_FINISH) {
			return -1;
		}
		if (status < 0) {
			return 1;
		}
		if (status) {
			if (status == EVT_RETURN_YIELD) {
				evt->opcode = 0;
			}
			else {
				if (status == EVT_RETURN_DONE) {
					evt->opcode = 0;
				}
				continue;
			}
			
		}
		return 0;
	}
}

//almost 1:1, additional stack variables and delayed addze on UF
s32 evtGetValue(EventEntry* entry, s32 index) {
	s32 value;

	EventWork* wp = evtGetWork();
	if (index <= EVTDAT_ADDR_MAX) {
		return index;
	}
	else if (index <= (-250 * 1000000)) {
		return index;
	}
	else if (index <= EVTDAT_FLOAT_MAX) {
		return (s32)check_float(index);
	}
	else if (index <= EVTDAT_UF_MAX) {
		index += EVTDAT_UF_BASE;
		//not quite 1:1 but close enough
		index = entry->ufBase[index / 32] & (1 << (index % 32));
		return index != 0;
	}
	else if (index <= EVTDAT_UW_MAX) {
		index += EVTDAT_UW_BASE;
		value = entry->uwBase[index];
		if (value <= EVTDAT_ADDR_MAX) return value;
		if (value <= EVTDAT_FLOAT_MAX) {
			value = (s32)check_float(value);
		}
		return value;
	}
	else if (index <= EVTDAT_GSW_MAX) {
		return swByteGet(index + EVTDAT_GSW_BASE);
	}
	else if (index <= EVTDAT_LSW_MAX) {
		return _swByteGet(index + EVTDAT_LSW_BASE);
	}
	else if (index <= EVTDAT_GSWF_MAX) {
		return swGet(index + EVTDAT_GSWF_BASE);
	}
	else if (index <= EVTDAT_LSWF_MAX) {
		return _swGet(index + EVTDAT_LSWF_BASE);
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		return (wp->gfData[index / 32] & (1 << (index % 32))) != 0;
	}
	else if (index <= EVTDAT_LF_MAX) {
		index += EVTDAT_LF_BASE;
		return (entry->lfData[index / 32] & (1 << (index % 32))) != 0;
	}
	else if (index <= EVTDAT_GW_MAX) {
		index += EVTDAT_GW_BASE;
		value = wp->gwData[index];
		if (value <= EVTDAT_ADDR_MAX) return value;
		if (value <= EVTDAT_FLOAT_MAX) {
			value = (s32)check_float(value);
		}
		return value;
	}
	else if (index <= EVTDAT_LW_MAX) {
		index += EVTDAT_LW_BASE;
		value = entry->lwData[index];
		if (value <= EVTDAT_ADDR_MAX) return value;
		if (value <= EVTDAT_FLOAT_MAX) {
			value = (s32)check_float(value);
		}
		return value;
	}
	else {
		return index;
	}
}

s32 evtGetNumber(EventEntry* entry, s32 index) {
	if (index <= EVTDAT_ADDR_MAX) {
		return index;
	}
	else if (index <= (-250 * 1000000)) {
		return index;
	}
	else if (index <= EVTDAT_FLOAT_MAX) {
		return index;
	}
	else if (index <= EVTDAT_UF_MAX) {
		return index + EVTDAT_UF_BASE;
	}
	else if (index <= EVTDAT_UW_MAX) {
		return index + EVTDAT_UW_BASE;
	}
	else if (index <= EVTDAT_GSW_MAX) {
		return index + EVTDAT_GSW_BASE;
	}
	else if (index <= EVTDAT_LSW_MAX) {
		return index + EVTDAT_LSW_BASE;
	}
	else if (index <= EVTDAT_GSWF_MAX) {
		return index + EVTDAT_GSWF_BASE;
	}
	else if (index <= EVTDAT_LSWF_MAX) {
		return index + EVTDAT_LSWF_BASE;
	}
	else if (index <= EVTDAT_GF_MAX) {
		return index + EVTDAT_GF_BASE;
	}
	else if (index <= EVTDAT_LF_MAX) {
		return index + EVTDAT_LF_BASE;
	}
	else if (index <= EVTDAT_GW_MAX) {
		return index + EVTDAT_GW_BASE;
	}
	else if (index <= EVTDAT_LW_MAX) {
		return index + EVTDAT_LW_BASE;
	}
	else {
		return index;
	}
}

//almost 1:1, additional stack variables from check_float
s32 evtSetValue(EventEntry* entry, s32 index, s32 value) {
	s32 retval;
	s32 shift;

	EventWork* wp = evtGetWork();
	if (index <= EVTDAT_ADDR_MAX) {
		return value;
	}
	else if (index <= EVTDAT_FLOAT_MAX) {
		return (s32)check_float(value);
	}
	else if (index <= EVTDAT_UF_MAX) {
		index += EVTDAT_UF_BASE;
		shift = index % 32;
		if (value != 0) { //set bit
			entry->ufBase[index / 32] &= ~(1 << shift);
		}
		else { //clear bit
			entry->ufBase[index / 32] |= (1 << shift);
		}
		return value;
	}
	else if (index <= EVTDAT_UW_MAX) {
		index += EVTDAT_UW_BASE;
		retval = entry->uwBase[index];
		entry->uwBase[index] = value;
		return retval;
	}
	else if (index <= EVTDAT_GSW_MAX) {
		index += EVTDAT_GSW_BASE;
		retval = swByteGet(index);
		swByteSet(index, value);
		return retval;
	}
	else if (index <= EVTDAT_LSW_MAX) {
		index += EVTDAT_LSW_BASE;
		retval = _swByteGet(index);
		_swByteSet(index, (u8)value);
		return retval;
	}
	else if (index <= EVTDAT_GSWF_MAX) {
		index += EVTDAT_GSWF_BASE;
		retval = swGet(index);
		if (value != 0) { //set bit
			swSet(index);
		}
		else { //clear bit
			swClear(index);
		}
		return retval;
	}
	else if (index <= EVTDAT_LSWF_MAX) {
		index += EVTDAT_LSWF_BASE;
		retval = _swGet(index);
		if (value != 0) { //set bit
			_swSet(index);
		}
		else { //clear bit
			_swClear(index);
		}
		return retval;
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		shift = index % 32;
		if (value != 0) {
			wp->gfData[index / 32] &= ~(1 << shift);
		}
		else {
			wp->gfData[index / 32] |= (1 << shift);
		}
		return value;
	}
	else if (index <= EVTDAT_LF_MAX) {
		index += EVTDAT_LF_BASE;
		shift = index % 32;
		if (value != 0) {
			entry->lfData[index / 32] &= ~(1 << shift);
		}
		else {
			entry->lfData[index / 32] |= (1 << shift);
		}
		return value;
	}
	else if (index <= EVTDAT_GW_MAX) {
		index += EVTDAT_GW_BASE;
		retval = wp->gwData[index];
		wp->gwData[index] = value;
		return retval;
	}
	else if (index <= EVTDAT_LW_MAX) {
		index += EVTDAT_LW_BASE;
		retval = entry->lwData[index];
		entry->lwData[index] = value;
		return retval;
	}
	else {
		return value;
	}
}

//almost 1:1, additional stack variables from check_float
f32 evtGetFloat(EventEntry* entry, s32 index) {
	s32 value;

	EventWork* wp = evtGetWork();
	if (index <= EVTDAT_ADDR_MAX) {
		return (f32)index;
	}
	else if (index <= (-250 * 1000000)) {
		return (f32)index;
	}
	else if (index <= EVTDAT_FLOAT_MAX) {
		return check_float(index);
	}
	else if (index <= EVTDAT_UW_MAX) {
		index += EVTDAT_UW_BASE;
		value = entry->uwBase[index];
		if (EVTDAT_FLOAT_MAX >= value) {
			return (f32)(value + EVTDAT_FLOAT_BASE) / 1024.0f;
		}
		else {
			return (f32)value;
		}
	}
	else if (index <= EVTDAT_GSW_MAX) {
		value = swByteGet(index + EVTDAT_GSW_BASE);
		if (EVTDAT_FLOAT_MAX >= value) {
			return (f32)(value + EVTDAT_FLOAT_BASE) / 1024.0f;
		}
		else {
			return (f32)value;
		}
	}
	else if (index <= EVTDAT_LSW_MAX) {
		value = _swByteGet(index + EVTDAT_LSW_BASE);
		if (EVTDAT_FLOAT_MAX >= value) {
			return (f32)(value + EVTDAT_FLOAT_BASE) / 1024.0f;
		}
		else {
			return (f32)value;
		}
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		if ((wp->gfData[index / 32] & (1 << (index % 32))) != 0) {
			return 1.0f;
		}
		else {
			return 0.0f;
		}
	}
	else if (index <= EVTDAT_LF_MAX) {
		index += EVTDAT_LF_BASE;
		if ((entry->lfData[index / 32] & (1 << (index % 32))) != 0) {
			return 1.0f;
		}
		else {
			return 0.0f;
		}
	}
	else if (index <= EVTDAT_GW_MAX) {
		index += EVTDAT_GW_BASE;
		value = wp->gwData[index];
		if (EVTDAT_FLOAT_MAX >= value) {
			return (f32)(value + EVTDAT_FLOAT_BASE) / 1024.0f;
		}
		else {
			return (f32)value;
		}
	}
	else if (index <= EVTDAT_LW_MAX) {
		index += EVTDAT_LW_BASE;
		value = entry->lwData[index];
		if (EVTDAT_FLOAT_MAX >= value) {
			return (f32)(value + EVTDAT_FLOAT_BASE) / 1024.0f;
		}
		else {
			return (f32)value;
		}
	}
	else {
		return check_float(index);
	}
}