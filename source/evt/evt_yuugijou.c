#include "evt/evt_yuugijou.h"
#include <string.h>

yuugijou_work yuugijouwork;
yuugijou_work* yuwp;

void yuugijou_init(void) {
	yuwp = &yuugijouwork;
	memset(yuwp, 0, sizeof(yuugijou_work));
}