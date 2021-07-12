#include "seq/seq_e3.h"
#include "drv/fadedrv.h"
#include "mariost.h"
#include "pmario_sound.h"
#include "system.h"

extern GlobalWork* gp;

static seq_e3_work work;
static seq_e3_work* wp;

//local prototypes
void alarm_handler(OSAlarm* alarm, OSContext* context);

void alarm_handler(OSAlarm* alarm, OSContext* context) {
	gp->mFlags |= 0x2000;
}

void seq_e3Init(seqdrv_work* work) {
	wp->handle = fileAllocf(4, "%s/monthry.tpl", getMarioStDvdRoot());
	wp->field_0x4 = 0;
	wp->field_0x8 = 0;
	wp->field_0xC = 0;
	wp->field_0x64 = 270.0f;
	OSCreateAlarm(&wp->alarm);
	OSSetAlarm(&wp->alarm, OSSecondsToTicks(600), alarm_handler);
	wp->field_0x48 = OSGetTime(); //initTime? 
	gp->mFlags &= ~2;
	gp->mFlags |= 0x20;
	gp->mFlags |= 0x1000;
	gp->mFlags &= ~0x2000;
	fadeEntry(9, 200, (GXColor) {0x00, 0x00, 0x00, 0xFF});
	fadeReset(1);
	psndStopAllFadeOut();
	psndBGMOn(0x200, "BGM_FILE_MENU1");
	work->field_0x4 = 0;
	//animGroupBaseAsync("OFF_d_meku", 0, 0);
}

void seq_e3Exit(seqdrv_work* work) {
	fileFree(wp->handle);
	psndStopAllFadeOut();
}

void seq_e3Main(seqdrv_work* work) {

}