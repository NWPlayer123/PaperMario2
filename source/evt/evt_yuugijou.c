#include "evt/evt_yuugijou.h"
#include <string.h>

yuugijou_work yuugijouwork;
yuugijou_work* yuwp;

void yuugijou_init(void) {
	yuwp = &yuugijouwork;
	memset(yuwp, 0, sizeof(yuugijou_work));

	yuwp->field_0x14 = 1;
	yuwp->field_0x18 = 1;
	yuwp->field_0x1C = 1;
	yuwp->field_0x20 = 1;
	yuwp->field_0x24 = 1;

	yuwp->field_0x74 = 0x5B964;
	yuwp->field_0x78 = 0x5825A;
	yuwp->field_0x7C = 0x4F826;
	yuwp->field_0x80 = 0x4C3D8;
	yuwp->field_0x84 = 0x4237E;

	yuwp->field_0xD4 = 0xE;
	yuwp->field_0xD8 = 0xC;
	yuwp->field_0xDC = 0xF;
	yuwp->field_0xE0 = 0xB;
	yuwp->field_0xE4 = 0xA;

	yuwp->field_0x2C = 1;
	yuwp->field_0x30 = 1;
	yuwp->field_0x34 = 1;
	yuwp->field_0x38 = 1;
	yuwp->field_0x3C = 1;

	yuwp->field_0x8C = 2;
	yuwp->field_0x90 = 5;
	yuwp->field_0x94 = 0x12;
	yuwp->field_0x98 = 0x1B;
	yuwp->field_0x9C = 0x8A;


}