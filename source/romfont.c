#include "romfont.h"
#include <dolphin/os.h>

//.sbss
static romFontWork work;

//.sdata
static romFontWork* wp = &work;

//.rodata string, .sdata table
const char* msg_jp[10] = {
	"�v���O���b�V�u���[�h�ŕ\�����܂����H",
	"�́@���@�@�@�@�@�@������",
	"�y�@�@�@�@�z",
	"��ʕ\�����[�h�̓v���O���b�V�u���[�h�ɃZ�b�g����܂����B",
	"��ʕ\�����[�h�̓C���^�[���[�X���[�h�ɃZ�b�g����܂����B",
	"�f�B�X�N�J�o�[���J���Ă��܂��B\n\n�Q�[���𑱂���ꍇ�́A\n�f�B�X�N�J�o�[��߂Ă��������B",
	"�y�[�p�[�}���I�q�o�f��\n�f�B�X�N���Z�b�g���Ă��������B",
	"�f�B�X�N��ǂ߂܂���ł����B\n\n���킵���́A�{�̂̎戵��������\n���ǂ݂��������B",
	"�G���[���������܂����B\n\n�{�̂̃p���[�{�^���������ēd����OFF�ɂ��A\n�{�̂̎戵�������̎w���ɏ]���Ă��������B",
	NULL
};

const char* msg_en[9] = {
	"Would you like to display in progressive scan mode?",
	"Yes              N o",
	"[    ]",
	"Progressive scan mode has been turned on.",
	"Progressive scan mode has been turned off.",
	"The Disc Cover is open.\nIf you want to continue the game,\nplease close the Disc Cover.",
	"Please insert\nthe \"Paper Mario 2\" Game Disc.",
	"The Game Disc could not be read.\nPlease read the\nNintendo GameCube Instruction Booklet\nfor more information.",
	NULL
};

static const char** msg_tbl[2] = { msg_jp, msg_en };

//local prototypes
void romFontMake(void);

void romFontMake(void) {
	//u8 char1;
	//u16 char2;
	int i, j;

	for (i = 0; i < 2; i++) {
		j = 0;
		while (msg_tbl[i][j]) { //check if ran out of messages
			if (wp->mLanguage == 0 || i == 0) { //double check 

			}
		}
	}
}

void romFontInit(void) {
	wp->field_0x0 = 0;
	wp->field_0x4 = 0;
	wp->mLanguage = OSGetFontEncode() != OS_FONT_ENCODE_SJIS;
	
}

const char* romFontGetMessage(s32 msg) {
	return msg_tbl[0][msg]; //has eng but always returns JP
}