#include "romfont.h"
#include <dolphin/os.h>

//.sbss
static romFontWork work;

//.sdata
static romFontWork* wp = &work;

//.rodata string, .sdata table
const char* msg_jp[10] = {
	"プログレッシブモードで表示しますか？",
	"は　い　　　　　　いいえ",
	"【　　　　】",
	"画面表示モードはプログレッシブモードにセットされました。",
	"画面表示モードはインターレースモードにセットされました。",
	"ディスクカバーが開いています。\n\nゲームを続ける場合は、\nディスクカバーを閉めてください。",
	"ペーパーマリオＲＰＧの\nディスクをセットしてください。",
	"ディスクを読めませんでした。\n\nくわしくは、本体の取扱説明書を\nお読みください。",
	"エラーが発生しました。\n\n本体のパワーボタンを押して電源をOFFにし、\n本体の取扱説明書の指示に従ってください。",
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