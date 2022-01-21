#include "drv/shadowdrv.h"

//.bss
CharShadowWork char_shadow_work;
ProjShadowWork proj_shadow_work;
DepthShadowWork depth_shadow_work;

//.sdata
u16 ShadowSizeTbl[4] = {0x190, 0x100, 0x80, 0x40};
DepthShadowWork* dswp = &depth_shadow_work;
ProjShadowWork* pswp = &proj_shadow_work;
CharShadowWork* cswp = &char_shadow_work;

void shadowInit(void) {

}

void shadowMain(void) {

}