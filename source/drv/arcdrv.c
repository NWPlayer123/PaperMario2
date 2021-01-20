#include "drv/arcdrv.h"
#include <string.h>

//.bss
static arcObj work[4];




//void* arcOpen(char* filename, )


//BOOL arcDataCheck()

void arcInit(void) {
	int i;

	for (i = 0; i < 4; i++) {
		memset(&work[i], 0, sizeof(arcObj));
	}
}