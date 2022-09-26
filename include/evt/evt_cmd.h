#pragma once

/* This header file is meant for creating evt scripts, until there's a better compiler.
 * Includes macros for the scripts themselves, along with user function helpers.
 */

#include "mgr/evtmgr.h"

//Function Helpers

//Used to define native C functions that scripts can call
#define USERFUNC_DEF(function) \
	s32 (function)(EventEntry* evt, BOOL isFirstCall)



//Script Helpers
#define NUMARGS(...)  (sizeof((s32[]){__VA_ARGS__})/sizeof(s32))

#define PTR(value) (s32)(value)
#define STRING(value) PTR(value)

#define EVT_HELPER_CMD(param_count, opcode) \
	(((param_count) << 16) | (opcode))

#define EVT_DEF(name) \
	s32 (name)[]

#define EVT_BEGIN(name) \
	s32 (name)[] = {

#define EVT_END() \
	0x1 };

//Script Parameters
#define RETURN() \
	EVT_HELPER_CMD(0, 0x2),


#define WAIT_MSEC(msec) \
	EVT_HELPER_CMD(1, 0xA), msec,



#define IF_LESS(lhs, rhs) \
	EVT_HELPER_CMD(2, 0x1A), lhs, rhs,


#define SET(index, value) \
	EVT_HELPER_CMD(2, 0x32), index, value,




#define USER_FUNC(...) \
	EVT_HELPER_CMD(NUMARGS((s32)__VA_ARGS__), 0x5B), \
	(s32)__VA_ARGS__,

//TODO: RUN_SCRIPT_TID, RUN_SCRIPT_ASYNC
#define RUN_SCRIPT(evt) \
	EVT_HELPER_CMD(1, 94), (s32)(evt),
