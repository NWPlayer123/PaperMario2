#include "pmario_sound.h"
#include "sound.h"
#include <string.h>
//TODO: put this in a folder?

//.data
static f32 angleABTBL[] = {
	1.0f, 1.273187f, 1.27303f, 1.272768f, 1.272402f, 1.271932f, 1.271358f,
	1.270681f, 1.269902f, 1.269021f, 1.268038f, 1.266956f, 1.265774f, 1.2644939f,
	1.263116f, 1.2616431f, 1.260075f, 1.258413f, 1.256659f, 1.254815f, 1.2528811f,
	1.250859f, 1.248752f, 1.24656f, 1.244285f, 1.2419291f, 1.239494f, 1.236981f,
	1.234393f, 1.2317311f, 1.228997f, 1.226192f, 1.22332f, 1.220382f, 1.217379f,
	1.2143151f, 1.211189f, 1.208006f, 1.204766f, 1.201471f, 1.1981241f, 1.1947269f,
	1.191281f, 1.1877871f, 1.18425f, 1.180669f, 1.177047f, 1.173386f, 1.169687f,
	1.165952f, 1.162184f, 1.158384f, 1.1545531f, 1.1506931f, 1.146806f, 1.142893f,
	1.138957f, 1.134998f, 1.131018f, 1.127019f, 1.1230021f, 1.118969f, 1.11492f,
	1.110858f, 1.106783f, 1.102697f, 1.098601f, 1.094496f, 1.090384f, 1.086266f,
	1.082142f, 1.078014f, 1.0738831f, 1.06975f, 1.065616f, 1.061482f, 1.057348f,
	1.053216f, 1.049087f, 1.04496f, 1.040838f, 1.036721f, 1.0326101f, 1.028504f,
	1.024406f, 1.020316f, 1.016234f, 1.0121599f, 1.0080971f, 1.004043f, 1.0f
};
const char* psenvlistname[] = {
	"",
	"ENV_OPN_AAA1",
	"ENV_OPN_MUJ1",
	"ENV_STG0_GOR0", "ENV_STG0_GOR1", "ENV_STG0_GOR3", "ENV_STG0_GOR4", "ENV_STG0_GOR5", "ENV_STG0_GOR6",
	"ENV_STG0_TIK1", "ENV_STG0_TIK2", "ENV_STG0_TIK3", "ENV_STG0_TIK4", "ENV_STG0_TIK5", "ENV_STG0_TIK6", "ENV_STG0_TIK7",
	"ENV_STG0_DAN1",
	"ENV_STG1_HEI1", "ENV_STG1_HEI2", "ENV_STG1_HEI3", "ENV_STG1_HEI4",
	"ENV_STG1_NOK1", "ENV_STG1_NOK2",
	"ENV_STG1_GON1", "ENV_STG1_GON2", "ENV_STG1_GON3", "ENV_STG1_GON4",
	"ENV_STG2_WIN1", "ENV_STG2_WIN2",
	"ENV_STG2_MRI1", "ENV_STG2_MRI2", "ENV_STG2_MRI3", "ENV_STG2_MRI4", "ENV_STG2_MRI5",
	"ENV_STG2_MRI6", "ENV_STG2_MRI7", "ENV_STG2_MRI8", "ENV_STG2_MRI9", "ENV_STG2_MRI10",
	"ENV_STG3_TOU1", "ENV_STG3_TOU2", "ENV_STG3_TOU3", "ENV_STG3_TOU4",
	"ENV_STG3_TOU5", "ENV_STG3_TOU6", "ENV_STG3_TOU7", "ENV_STG3_TOU8",
	"ENV_STG4_USU1",
	"ENV_STG4_GRA1",
	"ENV_STG4_JIN1", "ENV_STG4_JIN2", "ENV_STG4_JIN3",
	"ENV_STG5_MUJ1_1", "ENV_STG5_MUJ1_2", "ENV_STG5_MUJ2", "ENV_STG5_MUJ3", "ENV_STG5_MUJ4",
	"ENV_STG5_MUJ5", "ENV_STG5_MUJ6", "ENV_STG5_MUJ7", "ENV_STG5_MUJ8", "ENV_STG5_MUJ9",
	"ENV_STG5_DOU1", "ENV_STG5_DOU2", "ENV_STG5_DOU3", "ENV_STG5_DOU4", "ENV_STG5_DOU5", "ENV_STG5_DOU6",
	"ENV_STG5_DOU7", "ENV_STG5_DOU8", "ENV_STG5_DOU9", "ENV_STG5_DOU10", "ENV_STG5_DOU11", "ENV_STG5_DOU12",
	"ENV_STG6_RSH1", "ENV_STG6_RSH1_2", "ENV_STG6_RSH1_3", "ENV_STG6_RSH1_4", "ENV_STG6_RSH2",
	"ENV_STG6_EKI1", "ENV_STG6_EKI2", "ENV_STG6_EKI2_2", "ENV_STG6_EKI3", "ENV_STG6_EKI4",
	"ENV_STG6_PIK1", "ENV_STG6_PIK1_1", "ENV_STG6_PIK1_2", "ENV_STG6_PIK2", "ENV_STG6_PIK3",
	"ENV_STG7_BOM1", "ENV_STG7_BOM2", "ENV_STG7_BOM3",
	"ENV_STG7_MOO1", "ENV_STG7_MOO2", "ENV_STG7_MOO3", "ENV_STG7_MOO4",
	"ENV_STG7_AJI1", "ENV_STG7_AJI2", "ENV_STG7_AJI3", "ENV_STG7_AJI4", "ENV_STG7_AJI5", "ENV_STG7_AJI6", "ENV_STG7_AJI7",
	"ENV_STG8_LAS1", "ENV_STG8_LAS2", "ENV_STG8_LAS3", "ENV_STG8_LAS4", "ENV_STG8_LAS5",
	"ENV_STG8_LAS6", "ENV_STG8_LAS7", "ENV_STG8_LAS8", "ENV_STG8_LAS9",
	"ENV_KOOPA_KPA1",
	"ENV_END_MUJ1", "ENV_END_MUJ2", "ENV_END_MUJ3",
	"ENV_END_AAA1",
	"ENV_SYS_FL1"
};
SoundEffectList pssfxlist[] = { //4242 entries
	{"", 0, "", 0, 0, 0, 0, 0, 0, 0}
};

//.bss
SoundBGMEntry psbgm[2];
SoundEffectEntry pssfx[40];
SoundEnvEntry psenv[16];
PaperSoundWork psnd;
u8 _buf[0x80];

//.sbss
static s8 tbl[2];
s32 group;
BOOL _psndPushGroup_trg;
BOOL init_f;

//local definitions
void psndBGMMain(void);
void psndENVMain(void);
void psndSFXMain(void);

static f32 angleABf(f32 x1, f32 y1, f32 x2, f32 y2) {
	f32 xdiff, ydiff;
	f32 xabs, yabs;
	f32 v8;
	f32 v9;
	int v10;
	f32 v13;
	f32 v15;
	f32 test;

	xdiff = x2 - x1;
	ydiff = y2 - y1;
	xabs = __fabsf(xdiff);
	yabs = __fabsf(ydiff);
	if (xabs > yabs) {
		v8 = 45.0f * (yabs / xabs);
		v9 = 2.0f * v8;
		if (v9 >= 0.0f) {
			v10 = (int)(0.5 + v9);
		}
		else {
			v10 = -(int)(0.5 - v9);
		}
		test = v8 * angleABTBL[v10];
		if (xdiff >= 0.0f) {
			if (ydiff >= 0.0f) {
				v13 = 90.0f + test;
			}
			else {
				v13 = 90.0f - test;
			}
		}
		else {
			if (ydiff >= 0.0f) {
				v13 = 270.0f - test;
			}
			else {
				v13 = 270.0f + test;
			}
		}
	}
	else {
		if (yabs == 0.0f) {
			return 0.0f;
		}
		else {
			v15 = 45.0f * (xabs / yabs);
			v9 = 2.0f * v15;
			if (v9 >= 0.0f) {
				v10 = (int)(0.5 + v9);
			}
			else {
				v10 = -(int)(0.5 - v9);
			}
			test = v15 * angleABTBL[v10];
			if (ydiff >= 0.0f) {
				if (xdiff >= 0.0f) {
					v13 = 180.0f - test;
				}
				else {
					v13 = 180.0f + test;
				}
			}
			else {
				if (xdiff >= 0.0f) {
					v13 = 0.0f + test;
				}
				else {
					v13 = 360.0f - test;
				}
			}
		}
	}
	return v13;
}

//calc3D, returns the first bit of SND_3DINFO (vol, pan, span, unused)

s32 searchPSSFXList(const char* name) {
	SoundEffectList* entry = pssfxlist;
	int i;

	if ((s32)name >= 0) { //address, return
		return (s32)name;
	}
	for (i = 0; i < 4242; i++) {
		if (!strcmp(entry->name, name)) {
			break;
		}
		entry++;
	}
	if (i >= 4242) {
		i = 0;
	}
	return i;
}

void psndInit(void) {
	SoundBGMEntry* bgm;
	SoundEffectEntry* sfx;
	SoundEnvEntry* env;
	int i;

	SoundInit();
	for (bgm = psbgm, i = 0; i < 2; i++, bgm++) {
		bgm->field_0x0 = -1;
		bgm->field_0x20 = 0;
	}
	for (sfx = pssfx, i = 0; i < 40; i++, sfx++) {
		sfx->field_0x0 = -1;
		sfx->field_0x18 = -1;
		sfx->field_0x6 = 0;
	}
	for (env = psenv, i = 0; i < 16; i++, env++) {
		env->field_0x4 = 0;
	}
	psnd.field_0x18 = 0;
	psnd.field_0x0 = 0;
	psnd.field_0x1A = 0;
	psnd.field_0xC = 0;
	psnd.field_0x3C = 0.0f;
	psnd.field_0x50 = 0;
	psnd.field_0x54 = 0;
	psnd.field_0x56 = 0;
	psnd.field_0x5C = 0;
	psnd.field_0x5A = 100;
	psnd.field_0x59 = 100;
	psnd.field_0x58 = 100;
	strcpy(psnd.field_0x40, "??????");
	SoundLoadDVD2("sound/proj/pmario");
	SoundSLibLoadDVD("sound/proj/pmario");
}

void psndMain(void) {
	if (init_f) {
		if (psnd.field_0x5C)
			psnd.field_0x5C--;
		psndBGMMain();
		psndENVMain();
		psndSFXMain();
		SoundMain();
	}
}

void psndMainInt(void) {
	if (init_f) {
		SoundMainInt();
	}
}

void psndExit(void) {
	SoundDropData();
	DVDMgrSetupCallback(NULL);
	init_f = FALSE;
}

BOOL psndPushGroup(void) {
	if (_psndPushGroup_trg) {
		return TRUE;
	}
	if (group <= 11) {
		_psndPushGroup_trg = TRUE;
		tbl[0] = group;
		tbl[1] = -1;
	}
}













void psndBGMMain(void) {

}

void psndENVMain(void) {

}

void psndSFXMain(void) {

}

void psndStopAllFadeOut(void) {

}

BOOL psndBGMOn(u32 r3, char* bgm_name) {
	return FALSE;
}

BOOL psndENVOff(u32 r3) { //probably "id"
	return FALSE;
}

void psndSFXAllOff(void) {

}


void psndSFXOn(const char* name) {

}



