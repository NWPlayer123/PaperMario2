#include "nameent.h"
#include "system.h"
#include <string.h>

//.data
const char* katakana_tbl[] = {
	"\x83\x41", "\x83\x43", "\x83\x45", "\x83\x47", "\x83\x49", "\x83\x8F",
	"\x81\x40", "\x83\x92", "\x81\x40", "\x83\x93", "\x82\x4F", "\x82\x50",
	"\x82\x51", "\x82\x52", "\x83\x4A", "\x83\x4C", "\x83\x4E", "\x83\x50",
	"\x83\x52", "\x83\x4B", "\x83\x4D", "\x83\x4F", "\x83\x51", "\x83\x53",
	"\x82\x53", "\x82\x54", "\x82\x55", "\x82\x56", "\x83\x54", "\x83\x56",
	"\x83\x58", "\x83\x5A", "\x83\x5C", "\x83\x55", "\x83\x57", "\x83\x59",
	"\x83\x5B", "\x83\x5D", "\x82\x57", "\x82\x58", "\x81\x49", "\x81\x48",
	"\x83\x5E", "\x83\x60", "\x83\x63", "\x83\x65", "\x83\x67", "\x83\x5F",
	"\x83\x61", "\x83\x64", "\x83\x66", "\x83\x68", "\x81\x40", "\x94\xB5",
	"\x81\x99", "\x81\xF4", "\x83\x69", "\x83\x6A", "\x83\x6B", "\x83\x6C",
	"\x83\x6D", "\x83\x6F", "\x83\x72", "\x83\x75", "\x83\x78", "\x83\x7B",
	NULL,       NULL,       NULL,       NULL,       "\x83\x6E", "\x83\x71",
	"\x83\x74", "\x83\x77", "\x83\x7A", "\x83\x70", "\x83\x73", "\x83\x76",
	"\x83\x79", "\x83\x7C", NULL,       NULL,       NULL,       NULL,
	"\x83\x7D", "\x83\x7E", "\x83\x80", "\x83\x81", "\x83\x82", "\x83\x40",
	"\x83\x42", "\x83\x44", "\x83\x46", "\x83\x48", NULL,       NULL,
	NULL,       NULL,       "\x83\x84", "\x81\x40", "\x83\x86", "\x81\x40",
	"\x83\x88", "\x83\x83", "\x83\x85", "\x83\x87", "\x83\x62", "\x83\x94",
	NULL,       NULL,       NULL,       NULL,       "\x83\x89", "\x83\x8A",
	"\x83\x8B", "\x83\x8C", "\x83\x8D", "\x81\x7C", "\x81\x60", "\x81\x45",
	"\x81\x44", "\x81\x40", NULL,       NULL,       NULL,       NULL
};
const char* hiragana_tbl[] = {
	"\x82\xA0", "\x82\xA2", "\x82\xA4", "\x82\xA6", "\x82\xA8", "\x82\xED",
	"\x81\x40", "\x82\xF0", "\x81\x40", "\x82\xF1", "\x82\x4F", "\x82\x50",
	"\x82\x51", "\x82\x52", "\x82\xA9", "\x82\xAB", "\x82\xAD", "\x82\xAF",
	"\x82\xB1", "\x82\xAA", "\x82\xAC", "\x82\xAE", "\x82\xB0", "\x82\xB2",
	"\x82\x53", "\x82\x54", "\x82\x55", "\x82\x56", "\x82\xB3", "\x82\xB5",
	"\x82\xB7", "\x82\xB9", "\x82\xBB", "\x82\xB4", "\x82\xB6", "\x82\xB8",
	"\x82\xBA", "\x82\xBC", "\x82\x57", "\x82\x58", "\x81\x49", "\x81\x48",
	"\x82\xBD", "\x82\xBF", "\x82\xC2", "\x82\xC4", "\x82\xC6", "\x82\xBE",
	"\x82\xC0", "\x82\xC3", "\x82\xC5", "\x82\xC7", "\x81\x40", "\x94\xB5",
	"\x81\x99", "\x81\xF4", "\x82\xC8", "\x82\xC9", "\x82\xCA", "\x82\xCB",
	"\x82\xCC", "\x82\xCE", "\x82\xD1", "\x82\xD4", "\x82\xD7", "\x82\xDA",
	NULL,       NULL,       NULL,       NULL,       "\x82\xCD", "\x82\xD0",
	"\x82\xD3", "\x82\xD6", "\x82\xD9", "\x82\xCF", "\x82\xD2", "\x82\xD5",
	"\x82\xD8", "\x82\xDB", NULL,       NULL,       NULL,       NULL,
	"\x82\xDC", "\x82\xDD", "\x82\xDE", "\x82\xDF", "\x82\xE0", "\x82\x9F",
	"\x82\xA1", "\x82\xA3", "\x82\xA5", "\x82\xA7", NULL,       NULL,
	NULL,       NULL,       "\x82\xE2", "\x81\x40", "\x82\xE4", "\x81\x40",
	"\x82\xE6", "\x82\xE1", "\x82\xE3", "\x82\xE5", "\x82\xC1", "\x89\x4C",
	NULL,       NULL,       NULL,       NULL,       "\x82\xE7", "\x82\xE8",
	"\x82\xE9", "\x82\xEA", "\x82\xEB", "\x81\x7C", "\x81\x60", "\x81\x45",
	"\x81\x44", "\x81\x40", NULL,       NULL,       NULL,       NULL
};

//.bss
static NameEntryWork work;

//.sdata
static const char* msg_tbl[] = {
	"msg_nameent_1",
	"msg_nameent_2",
	"msg_nameent_3",
	"msg_nameent_3",
	"msg_nameent_4"
};
static NameEntryWork* wp = &work;

void nameEntInit(void) {
	memset(wp, 0, sizeof(NameEntryWork));
}

void nameEntReInit(void) {
	wp->field_0x4 = 3;
}

BOOL nameEntPrepare(void) {
	return fileAsyncf(4, NULL, "%s/w/jp/name.tpl", getMarioStDvdRoot()) != 0;
}

void nameEntMain(void) {

}