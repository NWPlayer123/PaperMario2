#include "eff/eff_nice.h"
#include "drv/animdrv.h"
#include "mariost.h"

extern GlobalWork* gp;

//.sdata
const char* anim_jp[] = {
	"EFF_nice",
	"EFF_good",
	"EFF_great",
	"EFF_wonderful",
	"EFF_excellent",
	NULL,
	"EFF_lucky"
};
const char* anim_us[] = {
	"EFF_nice_us",
	"EFF_good_us",
	"EFF_great_us",
	"EFF_wonderful_us",
	"EFF_excellent_us",
	NULL,
	"EFF_lucky_us"
};
const char** anim_tbl[2] = { anim_jp, anim_us };
s32 acrobat_jp[] = {179, 180, 181, 182, 183, 184, 185};
s32 acrobat_us[] = {183, 184, 185, 186, 187, 188}; //includes nonexistant textures
s32* acrobal_tbl[2] = { acrobat_jp, acrobat_us };



BOOL effNiceAsync(s32 group) {
	const char* name;
	int i;

	for (i = 0; i < 7; i++) {
		name = anim_tbl[gp->mLanguage][i];
		animGroupBaseAsync(name, group, NULL);
	}
	return FALSE;
}