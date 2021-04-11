#include "eff/eff_kemuri.h"

typedef struct effKemuriData {
	s32 field_0x0; //0x0
	f32 field_0x4; //0x4
	f32 field_0x8; //0x8
	f32 field_0xC; //0xC
	u8 field_0x10[0x40 - 0x10]; //0x10
	s32 field_0x40; //0x40
	f32 field_0x44; //0x44
	f32 field_0x48; //0x48
	u8 field_0x4C[0x8C - 0x4C]; //0x4C
} effKemuriData;

EffEntry* effKemuriEntry(s32 int2, f32 float3, f32 float4, f32 float5) {
	effKemuriData* data;
	s32 numEffects, i;
	
	EffEntry* entry = effEntry();
	switch (int2) {
		case 0:
			numEffects = 3;
			break;
		case 1:
			numEffects = 2;
			break;
		case 2:
			numEffects = 2;
			break;
	}
	entry->field_0x14 = "Kemuri";
	entry->effCount = numEffects;
	data->field_0x0 = int2;
	data->field_0x4 = float3;
	data->field_0x8 = float4;
	data->field_0xC = float5;
	for (i = 1; i < entry->effCount; i++) {

	}
}

void effKemuriMain(EffEntry* effect) {

}