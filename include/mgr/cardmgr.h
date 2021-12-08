#pragma once

#include "mario/mario_pouch.h"
#include "mario/mariost.h"
#include <dolphin/card.h>
#include <dolphin/os.h>

#define CARDMGR_WORKAREA_SIZE 0xA000

typedef struct CardMetadata {
	struct data {
		char game_string[0x20]; //0x0
		char date_string[0x20]; //0x20
		u8 banner_tex[0x1800]; //0x40
		u8 icon_tex[0x400]; //0x1840
		u8 icon_tlut[0x200]; //0x1C40
		u16 field_0x1E40; //0x1E40
		u16 field_0x1E42; //0x1E42
		u16 field_0x1E44; //0x1E44
	} data;
	u8 pad_0x1E46[0x1FF8 - 0x1E46]; //0x1E46
	u32 checksum1; //0x1FF8
	u32 checksum2; //0x1FFC
} CardMetadata;

typedef struct CardSaveSlot {
	struct save {
		u16 flags; //0x0
		u8 field_0x2[0x8 - 0x2]; //0x2
		GlobalWork mario; //0x8
		PouchData pouch; //0x13E0
		u8 field_0x19B4[0x2140 - 0x19B4]; //0x19B4
	} save;
	u8 pad_0x2140[0x3FF0 - 0x2140]; //0x2140
	char version[4]; //0x3FF0, "009" in JP, "010" in US, see "version" in cardmgr.c
	u32 size; //0x3FF4, size of stored data, used as check
	u32 checksum1; //0x3FF8
	u32 checksum2; //0x3FFC
} CardSaveSlot;

typedef struct CardData {
	CardMetadata metadata; //0x0
	CardSaveSlot saves[4]; //0x2000 (+ 0x4000 * save_index)
} CardData;

typedef struct CardWork {
	u16 flags; //0x0
	u8 pad_2[2]; //0x2
	void* workArea; //0x4, CARDMGR_WORKAREA_SIZE
	s32 channel; //0x8
	s32 fileNo; //0xC
	u64 serialNo; //0x10
	u8 field_0x18[0x1C - 0x18]; //0x18
	CARDFileInfo info; //0x1C
	CARDStat stat; //0x30
	s32 resultcode; //0x9C, see CARD_RESULT_
	u8 field_0xA0[0xA4 - 0xA0]; //0xA0
	s32 currentSlot; //0xA4
	CardData* data; //0xA8
	CardSaveSlot* currentSave; //0xAC, TODO rename readSave?
	CardSaveSlot* tempSave; //0xB0, TODO rename writeSave?
	u8 field_0xB4[0xD8 - 0xB4]; //0xB4
	s32 step; //0xD8
	u8 field_0xDC[0xE0 - 0xDC]; //0xDC
	s32 state; //0xE0
	u8 field_0xE4[0xF0 - 0xE4]; //0xE4
} CardWork;

CardData* cardGetFilePtr(void);
void cardBufReset(void);




void cardInit(void);



BOOL cardIsExec(void);

void cardMain(void);


void cardCreate(void);
void cardFormat(void);
