#pragma once

#include <dolphin/types.h>

typedef struct msgdrv_struct {
	u32 length; //0x0
	void* address; //0x4
	u32 count; //0x8
	u32** lookup; //0xC
} msgdrv_struct;

typedef struct msgdrv_work {
	msgdrv_struct somearray[2]; //0x0
	s32 poseId; //0x20
} msgdrv_work;

/*
TODO: old header file

#include <dolphin/types.h>
#include <stdio.h>
#include "mariost.h"
#include "dvdmgr.h"

typedef struct msgWorkStruct {
	u32 length; //TODO: standardize size/length
	void* address;
	u32 count;
	u32** lookup;
}msgWorkStruct;

typedef struct msgWork_gp {
	msgWorkStruct somearray[2];
	int animIndex;
} msgWork_gp;

char* msgSearch(char* tag);
*/