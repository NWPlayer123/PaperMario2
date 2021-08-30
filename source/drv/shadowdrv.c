#include "drv/shadowdrv.h"

//.bss
DepthShadowWork depth_shadow_work;
ProjShadowWork proj_shadow_work;
CharShadowWork char_shadow_work;

//.sdata
CharShadowWork* cswp = &char_shadow_work;
ProjShadowWork* pswp = &proj_shadow_work;
DepthShadowWork* dswp = &depth_shadow_work;
u16 ShadowSizeTbl[4] = {0x190, 0x100, 0x80, 0x40};

void shadowInit(void) {

}

void shadowMain(void) {

}