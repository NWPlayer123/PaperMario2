#include "mgr/fontmgr.h"
#include "mgr/dvdmgr.h"
#include "memory.h"
#include "system.h"

extern int sprintf(char* str, const char* format, ...);

//.sdata
GXColor fontColTbl[3] = {
	{0x1E, 0x14, 0x14, 0xFF},
	{0xDC, 0xDC, 0xDC, 0xFF},
	{0xB4, 0x78, 0x14, 0xFF}
};
s32 charspace[2] = {4, 2};
const char* fontname[2] = {"papermarioset_JPN", "popjoyB22"};

//.sbss
BinaryFontHeader* pfh; //pointer to font header
GXTexObj fgTexObj;
BinaryFontGlyph* blockGlyph;
BinaryFontInfo* pfi; //pointer to font info
BOOL fontEdge;
u8 fontAlpha;
GXColor fontColor;
Vec fontScale;

//local prototypes

void JUTFont_DrawStart(GXColor color);

void fontmgrInit(void) {
	; //stubbed in retail
}

void fontmgrTexSetup(void) {
	DVDEntry* dvd;
	u32 size;
	u32* ptr;
	char path[128];
	int i;

	if (!pfh) {
		sprintf(path, "%s/f/%s.bfn", getMarioStDvdRoot(), fontname[0]);
		dvd = DVDMgrOpen(path, 2, 0);
		size = OSRoundUp32B(DVDMgrGetLength(dvd));
		pfh = __memAlloc(HEAP_DEFAULT, size);
		DVDMgrRead(dvd, pfh, size, 0);
		DVDMgrClose(dvd);
		ptr = (u32*)((u32)pfh + sizeof(BinaryFontHeader));
		for (i = 0; i < pfh->blockcount; i++) {
			switch (*ptr) {
				case 'INF1':
					pfi = (BinaryFontInfo*)ptr;
					break;
				case 'GLY1':
					blockGlyph = (BinaryFontGlyph*)ptr;
					break;
				case 'MAP1':
				case 'WID1':
				default:
					break;
			}
			ptr = (u32*)((u32)ptr + *(ptr + 1));
		}
		GXInitTexObj(
			&fgTexObj,
			&blockGlyph[1],
			blockGlyph->sheetwidth,
			blockGlyph->sheetheight,
			GX_TF_I4,
			GX_CLAMP,
			GX_CLAMP,
			GX_FALSE);
		GXInitTexObjLOD(
			&fgTexObj,
			GX_LINEAR,
			GX_LINEAR,
			0.0f,
			0.0f,
			0.0f,
			GX_FALSE,
			GX_FALSE,
			GX_ANISO_1);

		//TODO: finish after JUTFont done
	}
}

//does some fucky stuff on the alpha channel that doesn't change anything
void FontDrawStart(void) {
	fontScale.x = 1.0f;
	fontScale.y = 1.0f;
	fontScale.z = 1.0f;
	fontEdge = FALSE;
	fontColor = fontColTbl[0];
	fontAlpha = 0xFF;
	JUTFont_DrawStart(fontColTbl[0]);
}

void FontDrawStart_alpha(u8 alpha) {
	GXColor start;

	start = fontColTbl[0];
	fontAlpha = alpha;
	fontScale = (Vec){1.0f, 1.0f, 1.0f};
	fontEdge = FALSE;
	fontColor = fontColTbl[0];
	start.a = (start.a * alpha) / 255;
	JUTFont_DrawStart(start);
}

void JUTFont_DrawStart(GXColor color) {

}




u16 JUTFont_CodeToGlyph(u16 code);

u16 kanjiSearch(u16 code) {
	return JUTFont_CodeToGlyph(code);
}

u16 JUTFont_CodeToGlyph(u16 code) {
	return 0;
}

u16 kanjiGetWidth(u16 code) {
	return 0;
}