#include "MusyX/seq.h"


extern u8 synthIsFadeOutActive(u8);

//TODO: update for TTYD's additional data
typedef struct Seq_Instance Seq_Instance;

typedef struct Seq_Note {
	u32 field_0x0; //0x0
	u32 field_0x4; //0x4
	u32 field_0x8; //0x8
	u32 field_0xC; //0xC
	u32 field_0x10; //0x10
} Seq_Note;

//TODO: better name?
typedef enum SeqInstance_State {
	SEQ_STATE_STOPPED = 0,
	SEQ_STATE_PLAYING = 1,
	SEQ_STATE_PAUSED = 2
} SeqInstance_State;

typedef struct pairs_idk {
	u32 field_0x0;
	u32 field_0x4;
} pairs_idk;

typedef struct somefield_0x14E8 {
	u8 field_0x0[0xC - 0x0]; //0x0
	pairs_idk field_0xC[2]; //0xC
	u32 field_0x1C;
	pairs_idk field_0x20[2]; //0x20
	u32 field_0x30;
	u32 field_0x34;
} somefield_0x14E8;

//TODO: identify additional 0x20 data from TTYD's MusyX 2.X
struct Seq_Instance {
	Seq_Instance* next; //0x0
	Seq_Instance* prev; //0x4
	u8 state; //0x8
	u8 id; //0x9
	u8 pad_0xA[2]; //0xA
	u8 field_0xC[0xEB0 - 0xC]; //0xC
	u8 field_0xEB0; //0xEB0, synthId?
	u8 field_0xEB1[0x14E4 - 0xEB1]; //0xEB1
	void* field_0x14E4; //0x14E4
	somefield_0x14E8 field_0x14E8[0x10]; //0x14E8
};


typedef struct Seq_MIDIPriority {
	s16 prio[0x10]; //0x0
} Seq_MIDIPriority;

//.bss
Seq_Note seqNote[SND_MAX_SEQNOTES]; //ClearNotes only
Seq_Instance seqInstance[SND_MAX_SEQINSTANCES];
Seq_MIDIPriority seqMIDIPriority[SND_MAX_SEQINSTANCES];

//.sbss
u32 seq_next_id;
u8 curFadeOutState; //TODO: "bool"
u32 curSeqId; //treated as a u8 in code
Seq_Note* noteFree; //pointer to first free node
Seq_Instance* cseq; //"current sequence"
Seq_Instance* seqFreeRoot;
Seq_Instance* seqActiveRoot;
void* seqPausedRoot; //TODO: re-type

/*
void ClearNotes(void) {

}

void InitPublicIds(void) {
	seq_next_id = 0;
}

//always called seqHandle(0x100)
void seqHandle(u32 a1) {
	Seq_Instance* seq;
	int i;
	u32 field_0xC, field_0x10;

	if (a1) {
		for (seq = seqActiveRoot; seq; seq = seq->next) {
			cseq = seq;
			curSeqId = seq->id;
			curFadeOutState = synthIsFadeOutActive(seq->field_0xEB0);
			/*if (cseq->field_0x14E4) {
				for (i = 0; i < 0x10; i++) {
					HandleMasterTrack(i);
					SetTickDelta(&cseq->field_0x14E8[i], a1);
				}
			}
			else {
				HandleMasterTrack(0);
				SetTickDelta(&cseq->field_0x14E8[i], a1);
				r26 = HandleTrackEvents(0, a1); //bool(u8, u32)
				r24 = HandleNotes(); //bool(void)
				HandleKeyOffNotes();
				for (i = 0; i < 2; i++) {
					field_0xC = cseq->field_0x14E8[0].field_0xC[0].field_0x0;
					cseq->field_0x14E8[0].field_0x20[0].field_0x0 += ;

				}
			}*
		}
	}
}

void seqInit(void) {
	int i, j;

	seqActiveRoot = NULL;
	seqPausedRoot = NULL;
	for (i = 0; i < SND_MAX_SEQINSTANCES; i++) {
		if (i == 0) { //first instance treated diff?
			seqFreeRoot = &seqInstance[i];
			seqInstance[i].prev = NULL;
		}
		else {
			seqInstance[i - 1].next = &seqInstance[i];
			seqInstance[i].prev = &seqInstance[i - 1];
		}
		seqInstance[i].id = (u8)i;
		seqInstance[i].state = SEQ_STATE_STOPPED;
		for (j = 0; j < 16; j++) {
			seqMIDIPriority[i].prio[j] = -1;
		}
	}
	seqInstance[i - 1].next = NULL;
	ClearNotes();
	InitPublicIds();
}
*/