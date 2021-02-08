#include "mariost.h"
#include "sdk/DEMOInit.h"
//#include <demo/DEMOPad.h>
#include "memory.h"
#include "pmario_sound.h"
//#include "evt/evt_badgeshop.h"
#include "evt/evt_yuugijou.h"
#include "mgr/dvdmgr.h"
#include "seq/seq_title.h"
#include "romfont.h"
#include <dolphin/mtx.h>
#include <dolphin/vi.h>
#include <string.h>

extern void *DemoCurrentBuffer, *DemoFrameBuffer1, *DemoFrameBuffer2;
extern int sprintf(char* str, const char* format, ...);
extern void DEMOPadInit(void);

extern void badgeShop_init(void);
extern void johoya_init(void);

void systemErrorHandler(OSError error, OSContext* context, u32 dsisr, u32 dar);
void setupErrorHandler(void);

//.bss
static u8 stack[0x1000];
OSThread DvdCheckThread;
marioStruct marioSt;

//.sdata
marioStruct* gp = &marioSt;

//.sbss
BOOL DvdCheckThreadOn; //DvdCheckTreadOn
u32 _mariostSystemLevel;

GXRenderModeObj sRMObjHReso = {
	VI_TVMODE_NTSC_INT, //viTVmode
	0x260, //fbWidth
	0x1C0, //efbHeight
	0x1C0, //xfbHeight
	0x1B, //viXOrigin
	0x10, //viYOrigin
	0x29A, //viWidth
	0x1C0, //viHeight
	VI_XFBMODE_DF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj sRMObjHReso_prog = {
	VI_TVMODE_NTSC_PROG, //viTVmode
	0x260, //fbWidth
	0x1C0, //efbHeight
	0x1C0, //xfbHeight
	0x1B, //viXOrigin
	0x10, //viYOrigin
	0x29A, //viWidth
	0x1C0, //viHeight
	VI_XFBMODE_SF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj GXNtsc480IntDfMarioSt = {
	VI_TVMODE_NTSC_INT, //viTVmode
	0x260, //fbWidth
	0x1E0, //efbHeight
	0x1E0, //xfbHeight
	0x1E, //viXOrigin
	0x0, //viYOrigin
	0x294, //viWidth
	0x1E0, //viHeight
	VI_XFBMODE_DF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

GXRenderModeObj GXNtsc480ProgMarioSt = {
	VI_TVMODE_NTSC_PROG, //viTVmode
	0x260, //fbWidth
	0x1E0, //efbHeight
	0x1E0, //xfbHeight
	0x1E, //viXOrigin
	0x0, //viYOrigin
	0x294, //viWidth
	0x1E0, //viHeight
	VI_XFBMODE_SF, //xFBmode
	0, //field_rendering
	0, //aa
	6, 6, 6, 6, 6, 6, //pix 0
	6, 6, 6, 6, 6, 6, //pix 1
	6, 6, 6, 6, 6, 6, //pix 2
	6, 6, 6, 6, 6, 6, //pix 3
	8, 8, 10, 12, 10, 8, 8, //vfilter
};

//local definitions
void gcResetCheck(void);
void gcRumbleCheck(void);
void* gcDvdCheckThread(void* param);
void viPostCallback(u32 retraceCount);

void gcResetCheck(void) {
	if (gp->field_0x1278) {
		VISetBlack(TRUE);
		VIFlush();
		VIWaitForRetrace();
		VIWaitForRetrace();
		VIWaitForRetrace();
		psndExit();
		DVDMgrDelete();
		if ((OSGetCurrentThread() != &DvdCheckThread) && DvdCheckThreadOn) {
			OSCancelThread(&DvdCheckThread);
			DvdCheckThreadOn = FALSE;
		}
		/*
		while (cardIsExec()) {
			cardMain();
			VIWaitForRetrace();
		}
		*/
		if (gp->field_0x1278 == 2) {
			OSResetSystem(OS_RESET_HOTRESET, 0, TRUE);
		}
		else {
			if (gp->mFlags & 4) {
				OSResetSystem(OS_RESET_RESTART, gp->mFlags & 8 != 0, FALSE);
			}
			else {
				OSResetSystem(OS_RESET_RESTART, 2, FALSE);
			}
		}
		while (1) {;}

	}
}

void gcRumbleCheck(void) {
	int i;

	for (i = 0; i < 4; i++) {

	}
}

void* gcDvdCheckThread(void* param) {
	return NULL;
}

void viPostCallback(u32 retraceCount) {

}

u32 marioStGetSystemLevel(void) {
	return gp->mSystemLevelFlags;
}



void marioStDisp(void) {

}

void marioStMain(void) {

}

void marioStInit(void) {
	if (!OSIsRestart() || (OSGetResetCode() & OS_RESET_SHUTDOWN)) {
		DEMOInit(&GXNtsc480IntDfMarioSt);
	}
	else {
		switch (OSGetResetCode() & OS_RESET_HOTRESET) {
			case 0:
				DEMOInit(&GXNtsc480IntDfMarioSt);
				break;
			case 1:
				DEMOInit(&GXNtsc480ProgMarioSt);
				break;
		}
	}
	DEMOEnableGPHangWorkaround(5);
	setupErrorHandler();
	DEMOPadInit();
	GXSetCopyClear((GXColor) { 0, 0, 0, 0 }, 0xFFFFFF);
	__GXSetIndirectMask(0);
	memset(gp, 0, sizeof(marioStruct));
	gp->fbWidth = DEMOGetRenderModeObj()->fbWidth;
	gp->efbHeight = DEMOGetRenderModeObj()->efbHeight;
	gp->field_0x1294 = 1;
	gp->field_0x1274 = 0;

	gp->mLastFrameRetraceLocalTime = 0;
	gp->mLastFrameRetraceDeltaTime = 0;

	gp->mLastFrameRetraceTime = OSGetTime();

	gp->mAnimationTimeNoBattle = 0;
	gp->mAnimationTimeInclBattle = 0;

	gp->field_0x60 = 0;
	gp->field_0x58 = 0;
	gp->field_0x50 = 0;
	gp->field_0x48 = 0;

	gp->mNextMapChangeFadeOutType = 9;
	gp->mNextMapChangeFadeOutDuration = 300;
	gp->mNextMapChangeFadeInType = 10;
	gp->mNextMapChangeFadeInDuration = 300;
	gp->mNextAreaChangeFadeOutType = 9;
	gp->mNextAreaChangeFadeOutDuration = 300;
	gp->mNextAreaChangeFadeInType = 10;
	gp->mNextAreaChangeFadeInDuration = 300;
	gp->mLanguage = (OSGetFontEncode() != OS_FONT_ENCODE_SJIS) || ((gp->mFlags & 0x1000) != 0);
	gp->mFPS = 60;
	gp->mSystemLevelFlags = 0;
	badgeShop_init(); //Badge Shop
	yuugijou_init(); //Pianta Parlor
	johoya_init(); //?????
	gp->mpMapAlloc = __memAlloc(HEAP_DEFAULT, 660 * 1024);
	VISetPostRetraceCallback(viPostCallback);
	romFontInit();
	OSCreateThread(&DvdCheckThread, gcDvdCheckThread, NULL, (void*)((u32)&stack + sizeof(stack)), sizeof(stack), 16, OS_THREAD_ATTR_DETACH);
	DvdCheckThreadOn = TRUE;
	OSResumeThread(&DvdCheckThread);
	DVDMgrInit();
}

void systemErrorHandler(OSError error, OSContext* context, u32 dsisr, u32 dar) {
	//registers
	OSFontHeader* fontHeader;
	char* loop_newline;
	char* loop_zero;
	u16 width2, height2;
	int i;
	u32 count, stack;
	u16 w1, w2, h1, h2, x1, x2, y1, y2;
	//stack vars
	s32 width; // 0x10
	s32 y; // 0x14
	s32 x; // 0x18
	void* image; // 0x1C
	GXTexObj texObj; // 0x20
	Mtx matrix2; // 0x40
	Mtx matrix; // 0x70
	Mtx44 projection; // 0xA0
	char print_buffer[0x1000]; // 0xE0
	char print_buffer2[0x400]; // 0x10E0

	if (getDebugMode() >= 0) {
		psndExit();
		while (1) {} //softlock
	}

	switch (error) {
		case OS_ERROR_DSI:
			strcpy(print_buffer, "---- OS_ERROR_DSI ----\n");
			break;
		case OS_ERROR_ISI:
			strcpy(print_buffer, "---- OS_ERROR_ISI ----\n");
			break;
		case OS_ERROR_PROGRAM:
			strcpy(print_buffer, "---- OS_ERROR_PROGRAM ----\n");
			break;
	}
	sprintf(print_buffer2, "---- Context 0x%08x ----\n", context);
	strcat(print_buffer, print_buffer2);
	for (i = 0; i < 16; i++) {
		sprintf(print_buffer2, "r%-2d  = 0x%08x r%-2d  = 0x%08x\n",
			i, context->gpr[i],
			i + 16, context->gpr[i + 16]
		);
		strcat(print_buffer, print_buffer2);
	}
	sprintf(print_buffer2, "LR   = 0x%08x CR   = 0x%08x\n", context->lr, context->cr);
	strcat(print_buffer, print_buffer2);
	sprintf(print_buffer2, "SRR0 = 0x%08x SRR1 = 0x%08x\n", context->srr0, context->srr1);
	strcat(print_buffer, print_buffer2);
	sprintf(print_buffer2, "DSISR= 0x%08x DAR  = 0x%08x\n", dsisr, dar);
	strcat(print_buffer, print_buffer2);
	strcat(print_buffer, "\n");
	sprintf(print_buffer2, "Address:      Back Chain    LR Save\n");
	strcat(print_buffer, print_buffer2);
	stack = context->gpr[1]; //get r1 (stack)
	i = 0;
	while ((stack != 0) && (stack != 0xFFFFFFFF) && (i++ < 16)) {
		sprintf(print_buffer2, "0x%08x:   0x%08x    0x%08x\n", stack, *(u32*)stack, *(u32*)(stack + 4));
		strcat(print_buffer, print_buffer2);
		stack = *(u32*)stack;
	}
	strcat(print_buffer, "\n");
	sprintf(print_buffer2, "Instruction at 0x%x\n(read from SRR0) attempted to access\ninvalid address 0x%x (read from DAR)\n", context->srr0, dar);
	strcat(print_buffer, print_buffer2);
	psndExit();
	OSFillFPUContext(context);
#ifdef __MWERKS__
	asm{
		mfmsr	r5
		ori		r5, r5, 0x8000
		ori		r5, r5, 2
		mtmsr	r5
		isync
	};
#endif
	smartReInit();
	if (OSGetFontEncode() == OS_FONT_ENCODE_SJIS) {
		fontHeader = smartAlloc(OS_FONT_SIZE_SJIS, 0)->alloc;
	}
	else {
		fontHeader = smartAlloc(OS_FONT_SIZE_ANSI, 0)->alloc;
	}
	OSInitFont(fontHeader);
	GXSetCopyClear((GXColor) { 0, 0, 0, 0xFF }, 0xFFFFFF);
	GXCopyDisp(DEMOGetCurrentBuffer(), 1);
	GXSetViewport(0.0f, 0.0f, 608.0f, 480.0f, 0.0f, 1.0f);
	GXSetScissor(0, 0, 608, 480);
	MTXOrtho(projection, 0.0f, 608.0f, 0.0f, 480.0f, 0.0f, 1.0f);
	GXSetProjection(projection, GX_ORTHOGRAPHIC);
	MTXScale(matrix, 0.5f, 0.5f, 1.0f);
	GXLoadPosMtxImm(matrix, 0);
	GXSetCurrentMtx(0);
	GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
	GXSetNumChans(0);
	GXSetNumTevStages(1);
	GXSetTevOp(GX_TEVSTAGE0, GX_REPLACE);
	GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
	GXSetNumTexGens(1);
	GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 30, GX_FALSE, 125);
	GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
	GXClearVtxDesc();
	GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
	GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, 1, 3, 0);
	GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, 1, 3, 0);
	while (1) {
		count = VIGetRetraceCount();
		loop_newline = (char*)&print_buffer;
		width2 = 50;
		height2 = 50;
		i = 0;
		loop_zero = loop_newline;
		while (*loop_zero != '\0') {
			if (*loop_newline == '\n') {
				width2 = 50;
				height2 += 24;
				i++;
				loop_zero++;
				loop_newline++;
			}
			else {
				OSGetFontTexture((const char*)&print_buffer[i], &image, &x, &y, &width);
				GXInitTexObj(&texObj, image, fontHeader->sheetWidth, fontHeader->sheetHeight,
					(GXTexFmt)fontHeader->sheetFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
				GXLoadTexObj(&texObj, GX_TEXMAP0);
				MTXScale(matrix2, (float)fontHeader->sheetWidth, (float)fontHeader->sheetHeight, 1.0f);
				GXLoadTexMtxImm(matrix2, 0x1E, GX_MTX2x4);
				GXBegin(GX_QUADS, GX_VTXFMT0, 4);
				w1 = width2;
				w2 = (u16)(w1 + fontHeader->cellWidth);
				h1 = height2;
				h2 = (u16)(h1 + fontHeader->cellHeight);
				x1 = (u16)x;
				x2 = (u16)(x1 + fontHeader->cellWidth);
				y1 = (u16)y;
				y2 = (u16)(y1 + fontHeader->cellHeight);

				GXPosition1x16(w1);
				GXPosition1x16(h1);
				GXPosition1x16(0);
				GXPosition1x16(x1);
				GXPosition1x16(y1);

				GXPosition1x16(w2);
				GXPosition1x16(h1);
				GXPosition1x16(0);
				GXPosition1x16(x2);
				GXPosition1x16(y1);

				GXPosition1x16(w2);
				GXPosition1x16(h2);
				GXPosition1x16(0);
				GXPosition1x16(x2);
				GXPosition1x16(y2);

				GXPosition1x16(w1);
				GXPosition1x16(h2);
				GXPosition1x16(0);
				GXPosition1x16(x1);
				GXPosition1x16(y2);

				i++;
				loop_zero++;
				loop_newline++;
				width2 += width;
			}
		}
		GXSetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GXSetColorUpdate(1);
		GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
		GXSetAlphaUpdate(1);
		GXCopyDisp(DemoCurrentBuffer, GX_TRUE);
		VISetNextFrameBuffer(DemoCurrentBuffer);
		VISetBlack(0);
		VIFlush();
		do { ; } while (count == VIGetRetraceCount());
		if (DemoCurrentBuffer == DemoFrameBuffer1)
			DemoCurrentBuffer = DemoFrameBuffer2;
		else
			DemoCurrentBuffer = DemoFrameBuffer1;
	}
}

void setupErrorHandler(void) {
	OSSetErrorHandler(OS_ERROR_DSI, (void (*)(u16, OSContext*, ...))systemErrorHandler);
	OSSetErrorHandler(OS_ERROR_ISI, (void (*)(u16, OSContext*, ...))systemErrorHandler);
	OSSetErrorHandler(OS_ERROR_PROGRAM, (void (*)(u16, OSContext*, ...))systemErrorHandler);
}