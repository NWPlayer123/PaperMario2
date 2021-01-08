#include "mgr/evtmgr_cmd.h"
#include "mgr/evtmgr.h"
#include "drv/swdrv.h"

static float check_float(s32 val) { // always inlined
	if (val <= EVTDAT_FLOAT_MAX) {
		return (val + EVTDAT_FLOAT_BASE) / 1024.0f;
	}
	else {
		return val;
	}
}

u32 evtSetValue(EvtEntry* entry, s32 index, s32 value) {
	u32 retval;

	evtWork* work = evtGetWork();
	if (index <= EVTDAT_ADDR_MAX) {
		return (u32)value;
	}
	else if (index <= EVTDAT_FLOAT_MAX) {
		return (u32)check_float(value);
	}
	else if (index <= EVTDAT_UF_MAX) {
		index += EVTDAT_UF_BASE;
		if (value == 0) { //clear bit
			entry->ufBase[index / 8] &= ~(1 << (index % 8));
		}
		else {
			entry->ufBase[index / 8] |= (1 << (index % 8));
		}
		return (u32)value;
	}
	else if (index <= EVTDAT_UW_MAX) {
		index += EVTDAT_UW_BASE;
		retval = (u32)entry->uwBase[index];
		entry->uwBase[index] = value;
		return retval;
	}
	else if (index <= EVTDAT_GSW_MAX) {
		index += EVTDAT_GSW_BASE;
		retval = swByteGet((u32)index);
		swByteSet((u32)index, (u32)value);
		return retval;
	}
	else if (index <= EVTDAT_LSW_MAX) {
		index += EVTDAT_LSW_BASE;
		retval = _swByteGet((u32)index);
		_swByteSet((u32)index, (u8)value);
		return retval;
	}
	else if (index <= EVTDAT_GSWF_MAX) {
		index += EVTDAT_GSWF_BASE;
		retval = swGet((u32)index);
		if (value == 0) { //clear bit
			swClear((u32)index);
		}
		else {
			swSet((u32)index);
		}
		return retval;
	}
	else if (index <= EVTDAT_LSWF_MAX) {
		index += EVTDAT_LSWF_BASE;
		retval = _swGet((u32)index);
		if (value == 0) { //clear bit
			_swClear((u32)index);
		}
		else {
			_swSet((u32)index);
		}
		return retval;
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		if (value == 0) { //clear bit
			work->gfData[index / 8] &= ~(1 << (index % 8));
		}
		else {
			work->gfData[index / 8] |= (1 << (index % 8));
		}
		return (u32)value;
	}
	else if (index <= EVTDAT_LF_MAX) {
		index += EVTDAT_LF_BASE;
		if (value == 0) { //clear bit
			entry->lfData[index / 8] &= ~(1 << (index % 8));
		}
		else {
			entry->lfData[index / 8] |= (1 << (index % 8));
		}
		return (u32)value;
	}
	else if (index <= EVTDAT_GW_MAX) {
		index += EVTDAT_GW_BASE;
		retval = (u32)work->gwData[index];
		work->gwData[index] = value;
		return retval;
	}
	else if (index <= EVTDAT_LW_MAX) {
		index += EVTDAT_LW_BASE;
		retval = (u32)entry->lwData[index];
		entry->lwData[index] = value;
		return retval;
	}
	else {
		return (u32)value;
	}
}

u32 evtGetValue(EvtEntry* entry, s32 index) {
	s32 value;

	evtWork* work = evtGetWork();
	if (index <= EVTDAT_ADDR_MAX) {
		return (u32)index;
	}
	else if (index <= EVTDAT_FLOAT_MAX) {
		return (u32)check_float(index);
	}
	else if (index <= EVTDAT_UF_MAX) {
		index += EVTDAT_UF_BASE;
		return (u32)((entry->ufBase[index / 8] & (1 << (index % 8))) >> (index % 8));
	}
	else if (index <= EVTDAT_UW_MAX) {
		index += EVTDAT_UW_BASE;
		value = entry->uwBase[index];
		if (EVTDAT_FLOAT_MAX < value <= EVTDAT_ADDR_MAX) {
			return (u32)value;
		}
		else {
			return (u32)check_float(value);
		}
	}
	else if (index <= EVTDAT_GSW_MAX) {
		return swByteGet((u32)(index + EVTDAT_GSW_BASE));
	}
	else if (index <= EVTDAT_LSW_MAX) {
		return _swByteGet((u32)(index + EVTDAT_LSW_BASE));
	}
	else if (index <= EVTDAT_GSWF_MAX) {
		return swGet((u32)(index + EVTDAT_GSWF_BASE));
	}
	else if (index <= EVTDAT_LSWF_MAX) {
		return _swGet((u32)(index + EVTDAT_LSWF_BASE));
	}
	else if (index <= EVTDAT_GF_MAX) {
		index += EVTDAT_GF_BASE;
		return (u32)((work->gfData[index / 8] & (1 << (index % 8))) >> (index % 8));
	}
	else if (index <= EVTDAT_LF_MAX) {
		index += EVTDAT_LF_BASE;
		return (u32)((entry->lfData[index / 8] & (1 << (index % 8))) >> (index % 8));
	}
	else if (index <= EVTDAT_GW_MAX) {
		index += EVTDAT_GW_BASE;
		value = work->gwData[index];
		if (EVTDAT_FLOAT_MAX < value <= EVTDAT_ADDR_MAX) { //not accurate but looks better
			return (u32)value;
		}
		else {
			return (u32)check_float(value);
		}
	}
	else if (index <= EVTDAT_LW_MAX) {
		index += EVTDAT_LW_BASE;
		value = entry->lwData[index];
		if (EVTDAT_FLOAT_MAX < value <= EVTDAT_ADDR_MAX) { //not accurate but looks better
			return (u32)value;
		}
		else {
			return (u32)check_float(value);
		}
	}
	else {
		return (u32)index;
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