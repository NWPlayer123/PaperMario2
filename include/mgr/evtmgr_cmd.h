#pragma once

#include <dolphin/types.h>

typedef struct EvtEntry EvtEntry;

#pragma enumsalwaysint off
typedef enum EvtOpcode {
	OP_InternalFetch = 0,
	OP_ScriptEnd = 1,
	OP_Return = 2,
	OP_Label = 3,
	OP_Goto = 4,
	OP_LoopBegin = 5,
	OP_LoopIterate = 6,
	OP_LoopBreak = 7,
	OP_LoopContinue = 8,
	OP_WaitFrames = 9,
	OP_WaitMS = 10,
	OP_WaitUntil = 11,
	OP_IfStringEqual = 12,
	OP_IfStringNotEqual = 13,
	OP_IfStringLess = 14,
	OP_IfStringGreater = 15,
	OP_IfStringLessEqual = 16,
	OP_IfStringGreaterEqual = 17,
	OP_IfFloatEqual = 18,
	OP_IfFloatNotEqual = 19,
	OP_IfFloatLess = 20,
	OP_IfFloatGreater = 21,
	OP_IfFloatLessEqual = 22,
	OP_IfFloatGreaterEqual = 23,
	OP_IfIntEqual = 24,
	OP_IfIntNotEqual = 25,
	OP_IfIntLess = 26,
	OP_IfIntGreater = 27,
	OP_IfIntLessEqual = 28,
	OP_IfIntGreaterEqual = 29,
	OP_IfBitsSet = 30,
	OP_IfBitsClear = 31,
	OP_Else = 32,
	OP_EndIf = 33,
	OP_SwitchExpr = 34,
	OP_SwitchRaw = 35,
	OP_CaseIntEqual = 36,
	OP_CaseIntNotEqual = 37,
	OP_CaseIntLess = 38,
	OP_CaseIntGreater = 39,
	OP_CaseIntLessEqual = 40,
	OP_CaseIntGreaterEqual = 41,
	OP_CaseDefault = 42,
	OP_CaseIntEqualAny = 43,
	OP_CaseIntNotEqualAll = 44,
	OP_CaseBitsSet = 45,
	OP_EndMultiCase = 46,
	OP_CaseIntRange = 47,
	OP_SwitchBreak = 48,
	OP_EndSwitch = 49,
	OP_SetExprIntToExprInt = 50,
	OP_SetExprIntToRaw = 51,
	OP_SetExprFloatToExprFloat = 52,
	OP_AddInt = 53,
	OP_SubtractInt = 54,
	OP_MultiplyInt = 55,
	OP_DivideInt = 56,
	OP_ModuloInt = 57,
	OP_AddFloat = 58,
	OP_SubtractFloat = 59,
	OP_MultiplyFloat = 60,
	OP_DivideFloat = 61,
	OP_MemOpSetBaseInt = 62,
	OP_MemOpReadInt = 63,
	OP_MemOpReadInt2 = 64,
	OP_MemOpReadInt3 = 65,
	OP_MemOpReadInt4 = 66,
	OP_MemOpReadIntIndexed = 67,
	OP_MemOpSetBaseFloat = 68,
	OP_MemOpReadFloat = 69,
	OP_MemOpReadFloat2 = 70,
	OP_MemOpReadFloat3 = 71,
	OP_MemOpReadFloat4 = 72,
	OP_MemOpReadFloatIndexed = 73,
	OP_SetUserWordBase = 74,
	OP_SetUserFlagBase = 75,
	OP_AllocateUserWordBase = 76,
	OP_AndExpr = 77,
	OP_AndRaw = 78,
	OP_OrExpr = 79,
	OP_OrRaw = 80,
	OP_ConvertMSToFrames = 81,
	OP_ConvertFramesToMS = 82,
	OP_StoreIntToPtr = 83,
	OP_StoreFloatToPtr = 84,
	OP_LoadIntFromPtr = 85,
	OP_LoadFloatFromPtr = 86,
	OP_StoreIntToPtrExpr = 87,
	OP_StoreFloatToPtrExpr = 88,
	OP_LoadIntFromPtrExpr = 89,
	OP_LoadFloatFromPtrExpr = 90,
	OP_CallCppSync = 91,
	OP_CallScriptAsync = 92,
	OP_CallScriptAsyncSaveTID = 93,
	OP_CallScriptSync = 94,
	OP_TerminateThread = 95,
	OP_Jump = 96,
	OP_SetThreadPriority = 97,
	OP_SetThreadTimeQuantum = 98,
	OP_SetThreadTypeMask = 99,
	OP_ThreadSuspendTypes = 100,
	OP_ThreadResumeTypes = 101,
	OP_ThreadSuspendTypesOther = 102,
	OP_ThreadResumeTypesOther = 103,
	OP_ThreadSuspendTID = 104,
	OP_ThreadResumeTID = 105,
	OP_CheckThreadRunning = 106,
	OP_ThreadStart = 107,
	OP_ThreadStartSaveTID = 108,
	OP_ThreadEnd = 109,
	OP_ThreadChildStart = 110,
	OP_ThreadChildStartSaveTID = 111,
	OP_ThreadChildEnd = 112,
	OP_DebugOutputString = 113,
	OP_DebugUnk1 = 114,
	OP_DebugExprToString = 115,
	OP_DebugUnk2 = 116,
	OP_DebugUnk3 = 117,
	OP_DebugUnk4 = 118
} EvtOpcode;
#pragma enumsalwaysint on

// negative 10s * 1000000
#define EVTDAT_ADDR_MAX -270 * 1000000
#define EVTDAT_FLOAT_MAX -220 * 1000000
#define EVTDAT_UF_MAX -200 * 1000000
#define EVTDAT_UW_MAX -180 * 1000000

#define EVTDAT_GSW_MIN -170000000 //-170M
#define EVTDAT_GSW_MAX -160000000 //-160M

#define EVTDAT_LSW_MAX -140 * 1000000

#define EVTDAT_GSWF_MIN -130000000 //-130M
#define EVTDAT_GSWF_MAX -120000000 //-120M

#define EVTDAT_LSWF_MIN -110000000 //-110M
#define EVTDAT_LSWF_MAX -100000000 //-100M

#define EVTDAT_GF_MAX -80 * 1000000
#define EVTDAT_LF_MAX -60 * 1000000
#define EVTDAT_GW_MAX -40 * 1000000
#define EVTDAT_LW_MAX -20 * 1000000

#define EVTDAT_FLOAT_BASE 230 * 1000000
#define EVTDAT_UF_BASE 210 * 1000000
#define EVTDAT_UW_BASE 190 * 1000000
#define EVTDAT_GSW_BASE 170 * 1000000
#define EVTDAT_LSW_BASE 150 * 1000000

#define EVTDAT_GSWF_BASE 130000000
#define EVTDAT_LSWF_BASE 110000000

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

typedef enum EvtStatus {
	EVT_RETURN_BLOCK = 0,
	EVT_RETURN_YIELD = 1,
	EVT_RETURN_DONE = 2,
	EVT_RETURN_REPEAT = 3,
	EVT_RETURN_FINISH = 255
} EvtStatus;

s32 evtmgrCmd(EvtEntry* entry);
s32 evtGetValue(EvtEntry* entry, s32 index);
s32 evtGetNumber(EvtEntry* entry, s32 index);
s32 evtSetValue(EvtEntry* entry, s32 index, s32 value);
f32 evtGetFloat(EvtEntry* entry, s32 index);
f32 evtSetFloat(EvtEntry* entry, s32 index, f32 value);