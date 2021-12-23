#pragma once

/* This header file is meant for creating evt scripts, until there's a better compiler.
 * Includes macros for the scripts themselves, along with user function helpers.
 */

#include "mgr/evtmgr.h"

//Function Helpers
#define USERFUNC_DEF(function) \
	EvtStatus (function)(EventEntry* evt, BOOL isFirstCall)



//Script Helpers
#define NUMARGS(...)  (sizeof((s32[]){__VA_ARGS__})/sizeof(s32))

#define PTR(value) (s32)(value)
#define STRING(value) PTR(value)

#define EVT_HELPER_CMD(param_count, opcode) \
	((param_count) << 16 | (opcode))

#define EVT_BEGIN(name) \
	s32 (name)[] = {

#define EVT_END() \
	0x1 };

//Script Parameters
#define RETURN() \
	EVT_HELPER_CMD(0, 0x2),


#define SET(index, value) \
	EVT_HELPER_CMD(2, 0x32), index, value,




#define USER_FUNC(function, ...) \
	EVT_HELPER_CMD(NUMARGS(__VA_ARGS__) + 1, 0x5B), \
	(s32)function, \
	__VA_ARGS__,