#include "evt/evt_johoya.h"
#include <string.h>

//.data
JohoyaWork _jdt;
const char* evtNoLabel[] = {
	"STG0_00",       "STG0_01",       "STG0_02",       "STG0_03",
	"STG0_13",       "STG0_14",       "STG0_15",       "STG0_16",
	"STG1_06_01",    "STG1_07",       "STG1_08",       "STG1_09",
	"STG1_14",       "STG1_14_01",    "STG1_15",       "STG1_15_01",
	"STG1_30",       "STG1_31",       "STG1_32",       "STG1_33",
	"STG2_04",       "STG2_05",       "STG2_05_01",    "STG2_05_02",
	"STG2_16",       "STG2_17",       "STG2_18",       "STG2_19",
	"STG2_25",       "STG2_25_01",    "STG2_25_02",    "STG2_25_03",
	"STG3_01",       "STG3_02",       "STG3_03",       "STG3_04",
	"STG3_20_05",    "STG3_20_06",    "STG3_20_07",    "STG3_20_07_00",
	"STG3_30_02",    "STG3_30_03",    "STG3_30_04",    "STG3_31",
	"STG3_52",       "STG3_END",      "STG4_00",       "STG4_01_00",
	"STG4_07",       "STG4_08",       "STG4_09",       "STG4_09_01",
	"STG4_13_03",    "STG4_13_04",    "STG4_14",       "STG4_15",
	"STG4_33",       "STG4_34",       "STG4_35",       "STG4_36",
	"STG5_18_02",    "STG5_18_03",    "STG5_19",       "STG5_19_01",
	"STG5_36",       "STG5_40",       "STG5_41",       "STG5_41_01",
	"STG5_61",       "STG5_62",       "STG5_63",       "STG5_64",
	"STG6_06",       "STG6_07",       "STG6_08",       "STG6_09",
	"STG6_22",       "STG6_23",       "STG6_24",       "STG6_24_02",
	"STG6_37",       "STG6_38",       "STG6_39",       "STG6_40",
	"STG6_51",       "STG6_52",       "STG6_53",       "STG6_54",
	"STG7_00",       "STG7_01",       "STG7_02",       "STG7_03",
	"STG7_15",       "STG7_16",       "STG7_17",       "STG7_18",
	"STG8_01_01",    "STG8_01_02",    "STG8_02",       "STG8_03",
	"STG8_15",       "STG8_16",       "STG8_END",      "STG9_00",
	"STG9_01",       "STG9_02",       "STG9_END"
};


void johoya_init(void) {
	memset(&_jdt.field_0x14, 0, sizeof(JohoyaEntry));
	memset(&_jdt.field_0x21, 0, sizeof(JohoyaEntry));
	memset(&_jdt.field_0x2E, 0, sizeof(JohoyaEntry));
}