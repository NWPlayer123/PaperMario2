#include "pmario_sound.h"
#include <string.h>
//TODO: put this in a folder?

//.data
extern SoundEffectList pssfxlist[]; //forward declaration

//.bss
SoundWork psnd;
SoundEnvironment psenv;
SoundEffectEntry pssfx[40];

//.sbss
BOOL init_f;

void psndInit(void) {

}

void psndMain(void) {

}

void psndStopAllFadeOut(void) {

}

BOOL psndBGMOn(u32 r3, char* bgm_name) {
	return FALSE;
}

void psndExit(void) {

}

BOOL psndENVOff(u32 r3) { //probably "id"
	return FALSE;
}

void psndSFXAllOff(void) {

}





s32 searchPSSFXList(const char* name) { //not 1:1, very weird function
    SoundEffectList* entry;
    int i;

	for (i = 0; i < 4242; i++) {
		entry = &pssfxlist[i];
		if (!strcmp(entry->name, name)) {
			break;
		}
	}
	if (i == 4242) {
		i = 0;
	}
	return i;
}

SoundEffectList pssfxlist[] = {
	{"", 0, "", 0, 0, 0, 0, 0, 0, 0}
};