#include "mgr/evtmgr_cmd.h"
#include "mgr/evtmgr.h"
#include "drv/swdrv.h"

static inline f32 check_float(s32 val) { // always inlined
	if (val <= EVTDAT_FLOAT_MAX) {
		return (val + EVTDAT_FLOAT_BASE) / 1024.0f;
	}
	else {
		return val;
	}
}

//almost 1:1, missing additional 1024.0 load from check_float
f32 evtSetFloat(EvtEntry* entry, s32 index, f32 value) {
	s32 retval;
	s32 shift;

	evtWork* work = evtGetWork();
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
			work->gfData[index / 32] &= ~(1 << shift);
		}
		else {
			work->gfData[index / 32] |= (1 << shift);
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
		retval = work->gwData[index];
		work->gwData[index] = (s32)(value / 1024.0f) + EVTDAT_FLOAT_BASE;
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

//almost 1:1, additional stack variables from check_float
f32 evtGetFloat(EvtEntry* entry, s32 index) {
	s32 value;

	evtWork* work = evtGetWork();
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
		value = (s32)swByteGet(index + EVTDAT_GSW_BASE);
		if (EVTDAT_FLOAT_MAX >= value) {
			return (f32)(value + EVTDAT_FLOAT_BASE) / 1024.0f;
		}
		else {
			return (f32)value;
		}
	}
	else if (index <= EVTDAT_LSW_MAX) {
		value = (s32)_swByteGet(index + EVTDAT_LSW_BASE);
		if (EVTDAT_FLOAT_MAX >= value) {
			return (f32)(value + EVTDAT_FLOAT_BASE) / 1024.0f;
		}
		else {
			return (f32)value;
		}
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		if ((work->gfData[index / 32] & (1 << (index % 32))) != 0) {
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
		value = work->gwData[index];
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

//almost 1:1, additional stack variables from check_float
s32 evtSetValue(EvtEntry* entry, s32 index, s32 value) {
	s32 retval;
	s32 shift;

	evtWork* work = evtGetWork();
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
		retval = (s32)swByteGet(index);
		swByteSet(index, (u32)value);
		return retval;
	}
	else if (index <= EVTDAT_LSW_MAX) {
		index += EVTDAT_LSW_BASE;
		retval = (s32)_swByteGet(index);
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
			work->gfData[index / 32] &= ~(1 << shift);
		}
		else {
			work->gfData[index / 32] |= (1 << shift);
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
		retval = work->gwData[index];
		work->gwData[index] = value;
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

s32 evtGetNumber(EvtEntry* entry, s32 index) {
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

//almost 1:1, additional stack variables and delayed addze on UF
s32 evtGetValue(EvtEntry* entry, s32 index) {
	s32 value;

	evtWork* work = evtGetWork();
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
		return (s32)swByteGet(index + EVTDAT_GSW_BASE); //TODO: retype to s32?
	}
	else if (index <= EVTDAT_LSW_MAX) {
		return (s32)_swByteGet(index + EVTDAT_LSW_BASE);
	}
	else if (index <= EVTDAT_GSWF_MAX) {
		return swGet(index + EVTDAT_GSWF_BASE);
	}
	else if (index <= EVTDAT_LSWF_MAX) {
		return _swGet(index + EVTDAT_LSWF_BASE);
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		return (work->gfData[index / 32] & (1 << (index % 32))) != 0;
	}
	else if (index <= EVTDAT_LF_MAX) {
		index += EVTDAT_LF_BASE;
		return (entry->lfData[index / 32] & (1 << (index % 32))) != 0;
	}
	else if (index <= EVTDAT_GW_MAX) {
		index += EVTDAT_GW_BASE;
		value = work->gwData[index];
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

/*
s32 evtmgrCmd(EvtEntry* entry) {
	s32 loopFlag;
	while (1) {
		loopFlag = 2;
		switch (entry->opcode) {
		case OP_InternalFetch: //0
			loopFlag = 3;
			entry->wCurrentCommandPtr = entry->nextCmdPtr;
			entry->opcode = *entry->nextCmdPtr & 0xFF;
			entry->paramCount = *entry->nextCmdPtr >> 16;
			entry->currCmdArgs = ++entry->nextCmdPtr;
			entry->nextCmdPtr += entry->paramCount;
			entry->sleeping = FALSE;
			break;
		case OP_Return: //2
			evtDelete(entry);
			loopFlag = 0xFF;
			break;
		}
		//TODO: clean up this bit, not sure how wrt switch statement on negative check
		if (loopFlag == 0xFF) {
			return -1;
		}
		else if (loopFlag < 0) {
			return 1;
		}
		else if (loopFlag == 0) {
			return 0;
		}
		else if (loopFlag == 1) {
			entry->opcode = OP_InternalFetch;
			return 0;
		}
		else if (loopFlag == 2) {
			entry->opcode = OP_InternalFetch;
		}
	}
}
*/