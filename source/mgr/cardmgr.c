#include "mgr/cardmgr.h"
#include "mariost.h"
#include "memory.h"
#include <string.h>

extern GlobalWork* gp;
extern int sprintf(char* str, const char* format, ...);

//.rodata
const u32 _mariost_icon_tex[] = {
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010203,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04040405, 0x06010000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000,
	0x00000007, 0x06040809, 0x0000070E, 0x0F0A1010, 0x00071516, 0x17101010, 0x07031611, 0x10101018,
	0x0A0B0A09, 0x080C0D00, 0x10101010, 0x11121314, 0x18191A19, 0x181B1C1D, 0x1A1E1A1A, 0x1F202104,
	0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x07000000, 0x00000000,
	0x22232222, 0x24252224, 0x30313230, 0x32323334, 0x3C3D3E3F, 0x403E4041, 0x4C4D4C4E, 0x4C4D4F2E,
	0x26271710, 0x10101028, 0x35211010, 0x1010101A, 0x1C101010, 0x10101028, 0x17101010, 0x10101018,
	0x292A2B2C, 0x2A1E2D2E, 0x362A2A2A, 0x2A37382E, 0x422A372A, 0x432A4445, 0x1E2A5051, 0x522E5354,
	0x2F242223, 0x23232224, 0x393A3032, 0x3B32303B, 0x46474849, 0x4A3D3E4B, 0x54545454, 0x4C4D4C4D,
	0x55555556, 0x55575859, 0x5E5F5F60, 0x61136263, 0x69696A6B, 0x16631C0B, 0x7778797A, 0x11107B11,
	0x10101010, 0x1010105A, 0x10101010, 0x10106465, 0x10101010, 0x106C6654, 0x1010105A, 0x7C547D7E,
	0x5B5C5454, 0x54545454, 0x66545454, 0x54545454, 0x546D6E6F, 0x70707154, 0x707F8070, 0x70818254,
	0x5454545D, 0x55555555, 0x54546768, 0x5E5F5F5F, 0x72737475, 0x69697674, 0x83848577, 0x77777777,
	0x86868709, 0x10108810, 0x9494047B, 0x9565655A, 0x9E9E9F53, 0x54A08A7C, 0xA4A5A6A7, 0x6F7070A8,
	0x1064898A, 0x6E8B7070, 0x5A969770, 0x70707070, 0x54549770, 0x70707070, 0x5454548A, 0x6F707070,
	0x8C8D8E70, 0x707F8F54, 0x7F8D7F70, 0x70989970, 0xA18D7F70, 0x9A6F7070, 0xA9AAA970, 0x70707070,
	0x54549091, 0x86929293, 0x709A7D54, 0x9B9C949D, 0x70707071, 0xA29E9EA3, 0x707070A8, 0x54A5A4A5,
	0xABAC547E, 0x7070709A, 0xAFB05470, 0x9A548A9A, 0xB6B6546F, 0x70706DA8, 0xBBBBBCA8, 0x70706FB1,
	0xA054548A, 0x6F707070, 0xB171546E, 0x7070B2B3, 0x70707070, 0x7070B78D, 0x70707070, 0x7070BD8D,
	0x70707070, 0x70707070, 0x81707070, 0x70707070, 0xAAB27070, 0x70707070, 0x8D8DB78C, 0xA9707070,
	0x70707097, 0x54ADABAE, 0x70707099, 0xB4B5B0AF, 0x70708BA0, 0xB8B9B6BA, 0xA96FA7BE, 0xBFBBC0BB,
	0xC1C2C3A0, 0x6F707070, 0xC9C9CACB, 0x54A77171, 0xCFCFD0CF, 0xD1D2D3D4, 0xD8D8D8D8, 0xD8D9DAD8,
	0x70707070, 0x707080B7, 0xB1707070, 0x70707080, 0x54998B70, 0x70707070, 0xDBDC547D, 0x7E707070,
	0x8D8D8D8D, 0x8DC4C5C4, 0xAA8D8D8D, 0x8D8D8D8D, 0xB2A1B78D, 0xAAD5D6D6, 0x70707081, 0x80A854DD,
	0xC654C7C8, 0xC2C1C1C2, 0xCCCDC9CE, 0xC9C9CACE, 0xBED7CFCF, 0xCFCFCFCF, 0xDEDFDAE0, 0xDAD8D8DA,
	0xE1E2E3E4, 0xE5E6E1E5, 0xEDEEEDED, 0xEEEFEEED, 0xF8F9F8F8, 0xF8F9FAF9, 0xFCFCFCFC, 0xFCFCFCFC,
	0xE5E7E8E9, 0x54547171, 0xEDEDEEED, 0xF0F1F2F3, 0xF8F8F9F9, 0xF9F8F9F9, 0xFCFCFCFC, 0xFCFCFCFC,
	0x71717D7E, 0x97A0EAEB, 0xF3F3F3F3, 0xF3F4F5F6, 0xF9F8F9F8, 0xF8F9FBF8, 0xFCFCFCFC, 0xFCFCFCFC,
	0xE2E6EBE5, 0xECE6EBE4, 0xF6EEEEED, 0xF7EEEDED, 0xF8F9F8F8, 0xF9F9F8F9, 0xFCFCFCFC, 0xFCFCFCFC
};
const u32 _mariost_icon_tlut[] = {
	0x8E778E34, 0x9D4AA4A4, 0xAC20A883, 0x9D2A8E56, 0xBC20D060, 0xD881E4A1, 0xAC4195AE, 0xA4A4B400,
	0xF8E2F4C2, 0xCC40B020, 0x91F1A884, 0xC020E8C1, 0xF62FF2F5, 0xF39CF503, 0xC420A4C6, 0xD1CEC94A,
	0xE672DC81, 0x8E778E77, 0x8E778E77, 0x9D2AB800, 0xF35AEB18, 0xB400C508, 0xD5EFE548, 0xB00095F1,
	0x8E788E78, 0x8E788E78, 0x95F2B020, 0xE2B5C4E7, 0xE1EE95F2, 0x8E788E78, 0x92789278, 0x92789278,
	0x9278A883, 0xDA31C0A5, 0xC949AC20, 0xA4C4A0E6, 0x9D4A95F2, 0x92789278, 0x92789278, 0x927899AF,
	0xD1CEBD07, 0xB061A840, 0xA8609298, 0x96789657, 0xA8A4C840, 0xE4C1C969, 0xAC40A4C4, 0x96989698,
	0x9A34A4C6, 0xB000E0A1, 0xF0E2D0A1, 0xAC60A14A, 0x96779698, 0x9A55AC41, 0xD8A1C545, 0xDA0AEA8D,
	0xFF52BD04, 0xA1299A55, 0x96989A98, 0x96989A99, 0x9A99A529, 0xBC00D461, 0xB860C966, 0xE64CCDC9,
	0xFB10E68D, 0xA881A18E, 0x9E129A77, 0x9EB9A883, 0xECC1C880, 0xB4A2FB10, 0xDE4C9820, 0xDA2BA040,
	0xA881A1CF, 0x9EB99E99, 0x9EB9DCC1, 0xB8A1DE2B, 0xC145CDA7, 0xF6EFA213, 0x9EB99EB9, 0xA2B9A8E6,
	0xAC81D1EA, 0xA508A2B9, 0xA2B9A2B9, 0xA4E6B8E3, 0xD5C9F2EF, 0xA061A6DA, 0xA6DAA6DA, 0xA6DAAADA,
	0xAADAEEAE, 0xEEAEA482, 0xA8A3A6DA, 0xAADAC146, 0xA98DAADA, 0xAADAAEFA, 0xA881C587, 0xA8A3AEB9,
	0xAEFAAEFA, 0xAEFBA96B, 0xACC3B0E4, 0x9840A906, 0xAE97B2FB, 0xB2FBAD4A, 0xA040AD8B, 0xB2FBB2FB,
	0xB2FBB254, 0xAD8CA906, 0xA8C5BD25, 0xA040B2D9, 0xB71BB71B, 0xB71BADCF, 0xA881B211, 0xB6D9B71B,
	0xB6FBB71B, 0xB71BB71B, 0xB71BB71B, 0xB71BB6F9, 0xB1CFA8A3, 0xAD6AB71B, 0xB71BB71B, 0xB71BB71B,
	0xB6B8B211, 0xB1ADAD07, 0xAD6AB6FA, 0xB71BB71B, 0xBB1BBB1B, 0xBB1BBB1B, 0xBB1B0000, 0x00000000
};
const u32 _mariost_banner_tex[] = {
	0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x85D285D2, 0x86BA85D2, 0xFFFFFFFF,
	0x86BA86BA, 0x86BA85D2, 0x86BA85D2, 0x85D2FFFF, 0x85D2FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF98A2,
	0x85D285D2, 0x85D286BA, 0xFFFFFFFF, 0xFFFF85D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0C1A8E1, 0xB18AFFFF,
	0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x85D286BA, 0x86BA86BA, 0xFFFF85D2, 0x86BA86BA,
	0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA85D2,
	0x86BA86BA, 0x85D285D2, 0x86BA85D2, 0xFFFFFFFF, 0x85D2FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0x85D286BA, 0x86BA85D2, 0xFFFF85D2, 0x85D2FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE739BD22, 0xBD22B942,
	0x85D285D2, 0x85D286BA, 0xFFFFFFFF, 0xFFFF85D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xB942B942, 0xE739FFFF,
	0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x86BA86BA, 0x85D286BA, 0x86BA85D2, 0x800086BA, 0x85D2FFFF,
	0x86BA85D2, 0x85D285D2, 0x85D2FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF9CC4, 0xB942B942,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xB942B942, 0x98A2A0C1,
	0x85D285D2, 0x85D286BA, 0xFFFFFFFF, 0xFFFF85D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0C198A2, 0xFFFFFFFF,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0C1BD82, 0xD603FFFF,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0C1BD82, 0xBD8298A2,
	0x85D286BA, 0x86BA85D2, 0xFFFF85D2, 0x85D2FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xE739FFFF, 0xFFFFFFFF,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA8E1FF05, 0xF6C5A0A1,
	0x85D286BA, 0x86BA86BA, 0xFFFF85D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFDEF7,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xDEF798A2, 0x98A298A2, 0xA0A1FEE5, 0xFEE5FEE5,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0x98A2DEF7, 0xFFFFFFFF, 0xFEE598A2, 0xF7BDB102,
	0x85D285D2, 0x86BA85D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xB102DE64, 0xB942B942, 0xDE64FEE5, 0xFEE5FEE5,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xB942B942, 0xB942FFFF, 0xFEE5FEE5, 0xEE24B942,
	0x86BA86BA, 0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFE739, 0xFFFFFFFF, 0xFFFFB942,
	0x85D285D2, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xB942B942, 0xB942C631, 0xFEE5FEE5, 0xFEE5A0A1,
	0x85D286BA, 0x86BA86BA, 0xFFFF85D2, 0x86BA86BA, 0xFFFFFFFF, 0x85D286BA, 0xC631FFFF, 0xFFFF85D2,
	0x85D2FFFF, 0xFFFFA0C1, 0x85D2FFFF, 0xA0A1FEC5, 0xFFFFFFFF, 0xE5E4FE65, 0xFFFFB8E2, 0xFE24FE24,
	0xA8E1B962, 0xFFFFDE03, 0xFEC5FEC5, 0xCDC3FE04, 0xFE65FE65, 0xFE65CCE2, 0xF5E4FE24, 0xFE24FE24,
	0xFEA5FEA5, 0xEDC3B16A, 0xB4C1CDC3, 0xEDC3B16A, 0xFD63FD63, 0xCCE29084, 0xCCA2CCA2, 0x8000B18C,
	0xFFFF8000, 0x85D285D2, 0xFFFFFFFF, 0xFFFFFFFF, 0xBD82BD82, 0xA8E1A0C1, 0xDDC3FE85, 0xFE85FE85,
	0x85D285D2, 0x85D2FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xA0C1B122, 0x9CC4FFFF, 0xFE85FE85, 0xB902EF7B,
	0xFFFFFFFF, 0xBD22BD22, 0xFFFFBD22, 0xFE64FE64, 0xB16AF604, 0xFE24FE24, 0xBCE2FDC4, 0xFDC4EDA3,
	0xBD22FEE5, 0xC542EE24, 0xC562FEE5, 0xCCA2FDC4, 0x9860FE85, 0xCCC2FD43, 0x8000D583, 0xFE24CCA2,
	0xFEE5EE24, 0xB942FFFF, 0xB942FDC4, 0xA061FFFF, 0xFD23CCC2, 0x9060BD82, 0xCCA28000, 0xA0A1FE85,
	0xFFFF85D2, 0x85D2FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xBD82A0C1, 0xA0C1A0C1, 0xFE85FE85, 0xFE85FE85,
	0xFFFFB942, 0xEE24FEE5, 0xFFFFFFFF, 0xCD63FE85, 0xB942FFFF, 0x98A398A3, 0xFE85A8C1, 0xFFFFFFFF,
	0xFEE5FEE5, 0xFEE5FEE5, 0xFE85FE85, 0xFE85FE85, 0x98A398A3, 0x98A3CD63, 0xFFFFD6B5, 0xB0C3E583,
	0xFEE5EE24, 0x98A3FFFF, 0xFE85FE85, 0xB942EF7B, 0xFE44FE44, 0xFE449461, 0xFE04FE04, 0xCD428000,
	0xDDC3FE64, 0xA081FFFF, 0xDD63FDE4, 0xA081FFFF, 0xDD02FD43, 0xA081FFFF, 0xA081A081, 0xA081FFFF,
	0xA0A1FE85, 0xFE85BD42, 0xA0C1FE44, 0xFE44B902, 0xBD02FE24, 0xFE24A081, 0xC502FDE4, 0xFDE49C61,
	0xB18CBD42, 0xBD42A0A1, 0xBDEFFE85, 0xFE85FE85, 0xC631FE44, 0xFE44FE44, 0xDEF7BD02, 0xBD02E583,
	0xA0A1FEC5, 0xFEC5A0A1, 0xFE85FE85, 0xFE85FE85, 0xFE44FE44, 0xFE44FE44, 0xFDE4FDE4, 0xE58398C6,
	0x9881FFFF, 0xFFFF98A2, 0xDDE3A0A1, 0xFFFF98A2, 0xDDA3A0A1, 0xFFFF98A2, 0xA0819860, 0xFFFF98A2,
	0xFEA5FEA5, 0xF664FEA5, 0xFE64FE64, 0x8000CD83, 0xFE04FE04, 0x8000CD42, 0xFDC4FDC4, 0xF5A3FDC4,
	0xFEA5FEA5, 0x98A2B18C, 0xFE64FE64, 0xFE64A8C1, 0xFE04FE04, 0xCD428000, 0xFDC4DD63, 0x8000B18C,
	0xDDE3FEA5, 0xFEA5FEA5, 0xBD22FE44, 0xFE44BD22, 0x9882FE04, 0xFE04BD02, 0xFFFFC461, 0xFDC4FDC4,
	0xFEA5FEA5, 0xFEA5DDE3, 0x8820EE04, 0xFE44DDC3, 0x9CC5EDC3, 0xFE04DD83, 0xFDC4FDC4, 0xFDC4CD02,
	0xB9CEFFFF, 0xB942FEA5, 0xA108B942, 0xFE64FE64, 0xA081FE04, 0xFE04FE04, 0xA061FDC4, 0xFDC4FDC4,
	0xFEA5FEA5, 0xFEA5FEA5, 0xFE64D5A3, 0xEE04FE64, 0x9860BD22, 0xBD22BD22, 0x8000DD63, 0xFDC4FDC4,
	0xA0A1E739, 0xFFFF8000, 0xA0A1DEF7, 0xFFFFFFFF, 0xBD22C631, 0xFFFFFFFF, 0xFDC48000, 0xFFFFFFFF,
	0xFFFFF5A3, 0xFDC4FDC4, 0xFFFFA061, 0xF543ED23, 0xFFFFFFFF, 0x98639C40, 0x89F3FFFF, 0xFFFFEF7B,
	0xBCE2B4C1, 0xFDC4FDC4, 0x8800BDEF, 0xD4E2FD63, 0x9084F7BD, 0xB16AE4C2, 0xDEF7FFFF, 0xFFFF9841,
	0xFDC4ED83, 0xA061FFFF, 0xFD63FD63, 0xF5439883, 0xFD03FD03, 0xB4618842, 0xBC61E4C2, 0x8000C631,
	0xDD63FDE4, 0xFDE4FDE4, 0xDD02FD23, 0xED03CCC2, 0xA8419000, 0x80009084, 0xFFFFEF7B, 0xEF7BFFFF,
	0xFDE4FDE4, 0xD5429CC6, 0xAC819020, 0x8000A461, 0xB18CCE73, 0xEF7BFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xF563FD83, 0xFD83C4E2, 0xF523FD43, 0xFD439840, 0x9841F4E2, 0xECE28000, 0xFFFF9863, 0xB4419084,
	0x9084ACA1, 0xFDE4FDE4, 0xCE73B16A, 0xFD83FD83, 0xE739FFFF, 0xD4A2FD23, 0xFFFFFFFF, 0xAC41BC61,
	0xDD63A94A, 0xA081FDE4, 0xFD83A061, 0xFFFFFD23, 0xF503A461, 0xDEF7A841, 0x88008842, 0xD6B5FFFF,
	0xFDE4FDE4, 0xFDE4FDE4, 0xFD23E502, 0xC4C2A461, 0x88008000, 0x98C6D6B5, 0xF7BDF7BD, 0xFFFFFFFF,
	0xFDE4B8C1, 0xFFFFBD22, 0x88008000, 0xBD22BD22, 0xF7BDFFFF, 0xFFFF9841, 0xFFFFFFFF, 0xFFFFFFFF,
	0xBD22E583, 0xCCE2FDC4, 0xFD83FD83, 0xFD83FD83, 0xE502FD43, 0xFD43FD43, 0xA928D4A2, 0xFD03FD03,
	0xFDC4CD22, 0x8000B18C, 0xCCE28000, 0xB18CFFFF, 0xAC61B18C, 0xFFFFFFFF, 0xDCC2B18C, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFFFC502, 0xFFFFFFFF, 0xC502FD63, 0xFFFFAC61, 0xFD23FD23, 0xFFFFFFFF, 0xC502FCE3,
	0xFDA4FDA4, 0xFDA48800, 0xFD63FD63, 0xFD638000, 0xFD23F503, 0x9820C631, 0xE4C28800, 0xC631FFFF,
	0xDEF79861, 0xE563FDA4, 0x9C83F543, 0xFD63FD63, 0xFFFFE4E2, 0xFD23FD23, 0xFFFF9841, 0xF4E2BC41,
	0xFDA4FDA4, 0xDD4398C6, 0xE523FD63, 0xE52398C6, 0x8800FD23, 0xFD2398C6, 0x8800FCE3, 0xFCE398C6,
	0xDEF7DEF7, 0xFFFF98A2, 0xFFFFFFFF, 0xFFFF98A2, 0xFFFFFFFF, 0xFFFF98A2, 0xFFFFFFFF, 0xFFFF98A2,
	0xFD83FD83, 0xFD83FD83, 0xFD43FD43, 0xFD43FD43, 0xFD03FD03, 0xA441FD03, 0xFCC2FCC2, 0x9820DC82,
	0xE5438800, 0xA108FFFF, 0xFD43CCC2, 0x9C83FFFF, 0xFD03FD03, 0xCCC2EF7B, 0xFCC2FCC2, 0xFCC28000,
	0xFFFFC461, 0xFD83FD83, 0xFFFFC461, 0xFD43FD43, 0xFFFFC461, 0xFCE3FCE3, 0xFFFFC461, 0xFCA2FCA2,
	0xFD83FD83, 0xDD839840, 0xBCA1BCA1, 0xA461FFFF, 0x9000BDEF, 0xD6B5FFFF, 0x9C20EF7B, 0xFFFFFFFF,
	0xA061FD83, 0xFD83FD83, 0xA041FD43, 0xFD43FD43, 0xFFFFBC41, 0xFCE3FCE3, 0xFFFFFFFF, 0xBC41FCA2,
	0x9020AC81, 0xCD02FD83, 0xDCE2DCE2, 0xDCE2FD43, 0xFCE3FCE3, 0xFCE3FCE3, 0xFCA2FCA2, 0xFCA2F4A2,
	0xFD838000, 0xFFFFFFFF, 0xFD438000, 0xFFFFFFFF, 0xE4C28000, 0xFFFFFFFF, 0x982098C6, 0xFFFFFFFF,
	0x8EDB89F3, 0xFFFFFFFF, 0x8EDB92DB, 0x89F38000, 0x92DB92DB, 0x92DB92DB, 0x8E139298, 0x96DC96DC,
	0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x8000FFFF, 0x92DB92DB, 0x92DB89D2, 0x96DC96DC, 0x96DC96DC,
	0xBDEFB18C, 0xC631FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000, 0x84A7A481, 0xD4C1D861,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF8000, 0x80008000, 0x80008000, 0xF8E2F880, 0xF840F460,
	0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000, 0xE460E860, 0xE481DC41, 0xEC60E840, 0xE440E440,
	0xFFFFFFFF, 0xD6B5C631, 0x8000FFFF, 0xFFFFFFFF, 0xD441B420, 0x8000FFFF, 0xE040DC20, 0xD800BC00,
	0xFFFFFFFF, 0xFFFFCE73, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x8000FFFF, 0x80008000, 0x8000A020,
	0xCE73FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF8000, 0x80008000, 0x80008000,
	0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000, 0x8000F0C2, 0xD060C881, 0xC041E060,
	0xFFFFFFFF, 0x8000FFFF, 0x80008000, 0x800089F3, 0x842192BA, 0x92DB92DB, 0x882192BA, 0x96DB96DC,
	0xFFFFF7BD, 0xB441DCC2, 0xFFFFFFFF, 0xFFFFA108, 0x80648000, 0xFFFFFFFF, 0x8485F2E0, 0xDA408000,
	0x8000A108, 0xFFFFFFFF, 0xA108FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF8000, 0x80008000, 0x80008000,
	0x8000FFFF, 0xFFFFB441, 0x8000DA40, 0xFFFFFFFF, 0x8000FF40, 0xDA40FFFF, 0xFF40FF40, 0xFF40DA40,
	0x8800C631, 0xFFFFFFFF, 0xD6B5FFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000,
	0xFFFFFFFF, 0xA8418820, 0xFFFFFFFF, 0xFFFFD6B5, 0x8000FFFF, 0xFFFFFFFF, 0x80008400, 0x80008000,
	0xEF7BBC61, 0xBC6198C6, 0xF7BDEF7B, 0xBDEFBDEF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000,
	0xFFFFFFFF, 0xFFFF98A2, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x8000FFFF, 0x80008000, 0x8000A106,
	0xBC41BC41, 0x90009863, 0xCE73BDEF, 0xBDEFEF7B, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000,
	0xBC41BC41, 0xBC418000, 0xEF7BBDEF, 0xBDEFBDEF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000,
	0xFFFFA021, 0x9C209C20, 0xFFFFFFFF, 0xDEF7DEF7, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000,
	0x8800DEF7, 0xFFFFFFFF, 0xDEF7F7BD, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFF8000, 0x80008000, 0x80008000,
	0xFFFFFFFF, 0xFFFF9820, 0x8000FFFF, 0xFFFFFFFF, 0x8000C821, 0xFFFFFFFF, 0xF801F821, 0xD4208000,
	0x9C20B441, 0xBC419820, 0xE739DEF7, 0xC631BDEF, 0xFFFFFFFF, 0xFFFFFFFF, 0x80008000, 0x80008000,
	0x8842E739, 0xFFFFFFFF, 0xE739FFFF, 0xFFFF8000, 0xFFFFFFFF, 0x80008000, 0x80008000, 0x800096DC,
	0xA1048884, 0x890A9257, 0xFFF4EF2E, 0xBDA88883, 0xE70EFFF4, 0xFFF3F32E, 0xB124C1E9, 0xFFD2FFF3,
	0x96FC96FC, 0x96FC8DB1, 0x894C96B9, 0x8DD2A441, 0xB9A78CC5, 0x9C21D081, 0xFFD3B987, 0x9C008800,
	0x9042D881, 0xF4C2F0C2, 0xECC2E4A2, 0xA8418400, 0xA8418000, 0x80008000, 0x80008000, 0x80009483,
	0xCC60B020, 0xC0E4C546, 0x8000B5CA, 0xF373FF93, 0xC5EAFFB6, 0xFFB4FF72, 0xFF94BD88, 0xBD68FF93,
	0xC567C988, 0xC988D1A9, 0xFF94FFB4, 0xFFB5D24D, 0xFF51FF52, 0xFFB4A905, 0xFF52FF52, 0xFF93C5A9,
	0xD1A8C966, 0xC946C967, 0xC20BFFB6, 0xFB94FF94, 0x8420F711, 0xFFB4FF51, 0x8000CDEB, 0xFFB4FF31,
	0xCDEAF773, 0xDA4C9C60, 0xFFB4FFD5, 0xDE6EAC80, 0xFF31FF93, 0xE6AFA480, 0xFF31FF94, 0xC9EAA880,
	0x9860BC61, 0xD860BCE4, 0xC1019840, 0x9483F773, 0xBCE1A860, 0xBD87FFB5, 0xB8C1A040, 0xC9A8F711,
	0xCE6DDECF, 0xD64CBD05, 0xFFB4FF10, 0xFF71FB72, 0xE26DE68E, 0xFF30FF51, 0xC5A9F711, 0xFF73FF31,
	0x802196BA, 0x96FC96FC, 0xA505890A, 0x9AFC9AFC, 0xF7108442, 0x9ABA9EFC, 0xFF519CE5, 0x96349EFC,
	0x84A7E260, 0xFF40FF00, 0x8D8EB560, 0xFF80FEA0, 0x9ADA8441, 0xFB60FEE0, 0x9EFC918F, 0x98A0FF80,
	0xFF40FF40, 0xFF40FF40, 0xFF00FF40, 0xFF40FF40, 0xFEA0FEE0, 0xFF20FF20, 0xFEC0FE80, 0xFEC0FEE0,
	0xFF40FF40, 0xFF40FF80, 0xFF40FF40, 0xFF40FF80, 0xFF40FF40, 0xFF80E680, 0xFF00FF00, 0xFF40B983,
	0xF2C0C22C, 0xDF2FEB71, 0xB982D2F0, 0xD70DDF30, 0xBA0CDF2E, 0xDB0DEB92, 0xD710DF2E, 0xDB0FB9C9,
	0xA52698A3, 0xA946A946, 0xEF72F3B4, 0xE793D6CF, 0xDB30C60B, 0xC5288000, 0xB4E6E58A, 0xA0418C63,
	0x9CC38800, 0x8C41D6EF, 0x94C38420, 0x88219421, 0x94A6F7BD, 0xBE108000, 0xFFFFFFFF, 0x88428000,
	0xCEADC22A, 0xD2ADA505, 0xE56AE56A, 0xD5488400, 0xF62FFEB2, 0xCD4898E7, 0xDE2FFF37, 0xD9EEEB7B,
	0x8800A526, 0xDB10EF93, 0x80008841, 0xCE2CDF30, 0xF3BD9D08, 0x880098A3, 0xFBDEFFFF, 0x98C78000,
	0xF3B494C4, 0x8D8F96FC, 0xF7D4EB72, 0x84429ADB, 0xAD88F3B4, 0xBDEA8D6D, 0xA545E330, 0xE3308486,
	0x96FC96FC, 0x96FC96FC, 0x9AFC9AFC, 0x9AFC9AFC, 0x9EFC9EFC, 0x9EFC9ADB, 0x9EFC9EFC, 0x9EFC8D6E,
	0x96FC8DF2, 0x80819180, 0x91F280C1, 0x83008780, 0x804186E0, 0xA5698902, 0x81C08A01, 0xFBBEFBDE,
	0x90C0C800, 0xF800F001, 0x83818600, 0xB8008E21, 0x86008F61, 0x81C082C0, 0xA4EA8040, 0x81008060,
	0xA4008464, 0x923596FC, 0x822084C8, 0x9AFC9AFC, 0xB60CBDAF, 0x91B09EFC, 0x8822F79D, 0x88649A98,
	0x96FC96FC, 0x96FC96FC, 0x9AFC9AFC, 0x9AFC9AFC, 0x9EFC9EFC, 0x9EFC9EFC, 0x9EFC9EFC, 0x9EFC9EFC,
	0xF2C8C1A5, 0xB145DAAC, 0xFB09FAE9, 0xE266C184, 0xEEC8F2C9, 0xFB09FB09, 0xF2C8F2C8, 0xEEC8F6E9,
	0xF7909CC3, 0x80008000, 0xBDC890A3, 0x80008000, 0xC1A5B187, 0xBDC8AD25, 0xF2C8C5A5, 0xCA09EF0E,
	0x80008000, 0x8000A0C4, 0x80008000, 0x80009062, 0xB56694A3, 0x84638442, 0xFB90E2CC, 0x84638C63,
	0xFF9598A3, 0x8821FB52, 0xFF95BD88, 0x8000E68E, 0xB547B147, 0x98A3B567, 0xC9EAFB51, 0xFFB4FF52,
	0xFF93FF31, 0xFF93EAAE, 0xFFB4FF52, 0xFF72FB72, 0xC9EAF2F0, 0xFF52FF52, 0xF6F0EECF, 0xFF52FF31,
	0x8000B967, 0xFFD5FF72, 0xB547D22B, 0xFFB4FB31, 0xFF93FFB5, 0xC5A98420, 0xFFD5F732, 0x90418C00,
	0xFF73F711, 0x9C61B4A0, 0xFF94FF73, 0xA4C39820, 0xBD67FF72, 0xFF94C9EB, 0x8400C987, 0xFF72FFB4,
	0xACA19C40, 0xDA4CD62C, 0xA8409C61, 0xF711DA6D, 0x9461C5A8, 0xFFB4FF52, 0xFF52FF73, 0xFF52FF52,
	0xB988FF72, 0xFF73FF52, 0xC9EAFF52, 0xFB11FF93, 0xD60BD20B, 0xF710FFB5, 0xFFB4FFB4, 0xFB93C1A9,
	0xFF529CC5, 0x9A35A31D, 0xF7318442, 0xA2FBA71D, 0xB5678CE9, 0xA71DA71D, 0x8884A6DA, 0xAB3DAB3D,
	0xA31CA31C, 0x8D2B94A0, 0xA71DA71D, 0xA71D9E77, 0xA71DA71D, 0xA71DA71D, 0xAB3DAB3D, 0xAB3DAB3D,
	0xF2E0FF40, 0xFF20FEE0, 0x84639CE0, 0xCE00EAC0, 0xA71D9E56, 0x8D0A8485, 0xAB3DA2B9, 0x9E349E34,
	0xFEC0FF00, 0xF680BA0B, 0xF700F720, 0xC1A0D2D0, 0x84638464, 0x848594A3, 0xA297A71C, 0xAB3D916D,
	0xE370CE8D, 0x9D24C129, 0xDF10BE29, 0xADA6F272, 0xEF93E791, 0xB569FEF5, 0x8842A567, 0xDD6CFD6D,
	0xFAD38821, 0x8000F39C, 0xFF16EE50, 0x9CC6FFFF, 0xF6D3FEF5, 0xEA2FE75A, 0xF94CFA72, 0xFEB3CDCD,
	0xFFFFFFFF, 0x88428000, 0xFFFFFFFF, 0xD294A96C, 0xFFFFFFFF, 0xFFFFFBFF, 0xEB9DFBFF, 0xF7DECE51,
	0xDA51FEF5, 0xE22F98C6, 0xE2B4FED3, 0xDDEDC652, 0xD1EEFED3, 0xF271DAD6, 0xF250FEB3, 0xFEB3DA0E,
	0x94A5C631, 0xEF7B8C42, 0xFFFFF7BD, 0xEF9CF6B2, 0xFFFFFFFF, 0xDAF7FA50, 0xD2B5D6D6, 0xDA0FF9ED,
	0xDAEFEF72, 0x94C49A34, 0x98C48463, 0x9A34A2DA, 0xCD4A8D2A, 0xA71DA71D, 0xF50A8422, 0xAB1CAB3D,
	0xA31DA31C, 0xA31C8484, 0xA71DA71D, 0xA71D8061, 0xA71DA6FB, 0x88C78540, 0xAB3D88C7, 0x820086C0,
	0x82C09DE7, 0xFFDFFFFF, 0x83209DE7, 0xFFDFFFFF, 0x872085A1, 0xFBBEFFFF, 0x86818260, 0xB60DFFDF,
	0xDAD68000, 0xBDAFEF7B, 0xFBDE9084, 0xDA77BA8C, 0xFFFFFFFF, 0xAEC986C0, 0xFFDFCF72, 0x86E08EE1,
	0x8842B22B, 0x95C28920, 0x92C08F20, 0x9F81A3A2, 0x97419721, 0x92819241, 0x8EE18EE1, 0x930192E1,
	0x8462918F, 0xA31DA31C, 0xA3618D40, 0x88C8A71D, 0x9AE19F41, 0x918095D0, 0x97019721, 0x9F818482,
	0xF2C8F2C8, 0xF2C8EEA8, 0xF2C8F2C8, 0xF2C8F2C8, 0xF2C8F2C8, 0xF2C8F2C8, 0xF2C8F2C8, 0xF2C8F6E9,
	0xF6E9FAE9, 0xDA05AD24, 0xF2C9EA88, 0xF6E8D1E4, 0xF2C9E246, 0xE666F6E8, 0xEA87DE05, 0xEA87EA87,
	0xEAEEF32E, 0x9CC3E68E, 0xB165D24B, 0xC1A8FF93, 0xCDA39082, 0xCE0BFF93, 0xF6C7A4E1, 0xC5CAFF51,
	0xFFD6FF52, 0xFF52FF52, 0xFF52FF32, 0xFF52FF52, 0xFF31FF52, 0xFF52FF52, 0xF6EFFF31, 0xFF31FF52,
	0xFF52FF52, 0xFF31FF93, 0xFF52FF31, 0xFF72FB73, 0xFF52FF52, 0xFFB4D62C, 0xFF31FF52, 0xFFB4AD05,
	0xFFB4B147, 0x8400A8A2, 0xB1478400, 0xA061A8A2, 0x8400A461, 0xA4A29C61, 0xA040A8A2, 0xA0828C20,
	0x8C009CA2, 0xFAEEFF73, 0x84009CA2, 0xFAEEFF73, 0x8400C9A7, 0xFF30FF52, 0x9C82F6CD, 0xFF31FF31,
	0xFF52FF52, 0xFF32FF72, 0xFF32FF31, 0xFF94E6AF, 0xFF52FF52, 0xFFB5C5A9, 0xFF31FF93, 0xFB529CC3,
	0xF711DE6D, 0x9CC48885, 0x98619820, 0x9C408864, 0x9C20BCC1, 0xC1038841, 0x9040A4C2, 0x90618464,
	0xAAFCAB3D, 0xAB3DAB3D, 0xAF1CAF3D, 0xAF3DAF3D, 0xAF1CB33E, 0xB33EB33E, 0xAAFAB33E, 0xB35EB35E,
	0xAB3DAB3D, 0xAB3DA6B9, 0xAF3DAF3D, 0xA6978443, 0xB33EAADA, 0x8463A4A2, 0xB33D88A6, 0x9C81D585,
	0x8D098442, 0x8C419462, 0x9C82C565, 0xDDC7DDC6, 0xD586D985, 0xB8E1B504, 0xC923A461, 0xC1CCDEF6,
	0x8C6288C7, 0xA298AB3D, 0xD585B0E3, 0x8842A6B9, 0xC9A9D5E9, 0xA0A288A6, 0xCE72CE51, 0xB9AB8421,
	0xA6DA8442, 0xF5ADF94B, 0xAF3D8864, 0xFAB2F64F, 0xAF1C8884, 0xF250F60D, 0xA656912B, 0xBD6AFE91,
	0xF5AEFA93, 0xFED4FA92, 0xF691F692, 0xFAB3FAB3, 0xF22EF650, 0xF671FA92, 0xEDCBF20E, 0xF64FF650,
	0xD5EEDA52, 0xD1CCEA0D, 0xFED4FA71, 0xD630DB39, 0xFAB3EE4F, 0xDB18F37B, 0xF671FEB2, 0xBDACE400,
	0xFEF5FED4, 0xFED3FEF4, 0xC1CCFAD3, 0xF292D5ED, 0xC4009000, 0xB528BD49, 0xF400A484, 0xFAB2FA70,
	0xF250F24F, 0xFA70F64F, 0xFED3FA71, 0xF64FF1ED, 0xFA71F62F, 0xF20EF5EC, 0xF22EEDED, 0xF1CBFE91,
	0xFA509463, 0x95AF958E, 0xFA919083, 0xCE73DAD6, 0xF2508C41, 0xFFFFFFFF, 0xAD08B0E1, 0xFBBBFFFF,
	0x99F280C0, 0xBB8EF7BD, 0xB16C95C5, 0xFBDEFFFF, 0xFBFFBA6E, 0xF7BEFBDE, 0xFBBDB9AC, 0xF7DEF7BD,
	0xFBFEEBDA, 0x92838DE3, 0xFFFFFFFF, 0xFBFE9723, 0xFFFFFBDE, 0xFFFFF7DD, 0xFFFFFBDE, 0xFFFFFFFF,
	0x9E4686A0, 0x8AC18EC1, 0x82C08EE1, 0x8EC18EC1, 0x86A18620, 0x8F018EC1, 0x962381A0, 0x8F218EC1,
	0x8EC18AC1, 0x8EE18EE1, 0x8EC18EC1, 0x8EC18EE1, 0x8EC18EC1, 0x8EC18AC1, 0x8EE18EE1, 0x8AC18AA1,
	0x93018EE1, 0x93408D41, 0x8EE18AA0, 0x8B008D81, 0x8AA08660, 0x86E08960, 0x86608240, 0x8F218482,
	0xF2C9F2C8, 0xF2C8F2C9, 0xEEA8F2C8, 0xF2E9EA88, 0xE667F2E9, 0xF2C8E246, 0xEEA8F2C9, 0xEEA8EA66,
	0xDE25E226, 0xEEA7E646, 0xD9E5EA87, 0xEA87E666, 0xE646F6C8, 0xEE87F285, 0xDA26B543, 0xCDC4C141,
	0xFAE8BD83, 0x9062FF71, 0xEE86A4E2, 0x8C65C9C9, 0xB922A657, 0xB73D9DB0, 0xA508B75E, 0xBB5EB75E,
	0xFACEF6CE, 0xFAEFFAEF, 0xFB0FFF0F, 0xFF30FF71, 0xAD26C5C8, 0xD20BC5C9, 0xB73D9DF2, 0x84218000,
	0xFF30FFB4, 0xCE0B9420, 0xFB51B989, 0x8C20A881, 0x9CA39020, 0xA881A8A2, 0x9820B0A2, 0x9C619041,
	0xA881A082, 0xA8A28C20, 0xA8A2A081, 0x98618400, 0xA4819441, 0x8400C060, 0x8C208400, 0x9C00ECA1,
	0x9CA3FACE, 0xFF10FAF0, 0x9021EE8C, 0xFF0FF6CE, 0xC040DA2A, 0xFACEFF30, 0xC040DA2A, 0xFF51E6CE,
	0xFF10FFB4, 0xC60B9420, 0xFF71E6CE, 0x9041EC80, 0xF73088AC, 0x842AC860, 0x8C648827, 0xCCA3F0A0,
	0xDCA1DCC2, 0xC061A861, 0xF8C2F4C1, 0xFCE2F8E2, 0xCC81B861, 0xE4A1FCC2, 0xFCC2D881, 0xC881FCC2,
	0x88428CE8, 0xAEDAB75E, 0xECC2BC61, 0x8863A213, 0xF8A2FCC2, 0xD0808000, 0xF8A1F860, 0xBC6580B4,
	0x998E8C41, 0xC944CD43, 0x8863B8E3, 0xD9849C61, 0x8C41D165, 0xAC81B9AC, 0xA4C3C4E0, 0xA4C4F37A,
	0x9C61D6B3, 0xE758C1AA, 0xD292EB59, 0xB125CD63, 0xF37AAD05, 0xD5C5E628, 0xB569D5C5, 0xE649DE08,
	0xC9A7CDA7, 0xBDAB94A5, 0xD5C6D9C5, 0xD1A69062, 0xD9E8D9E8, 0xE6089C82, 0xDA08D9E8, 0xE2289882,
	0x99AFB31C, 0x8442E22F, 0x99AFB75E, 0xAED98442, 0x9DD1956C, 0x88638C20, 0x84429C41, 0xC4C2C4C2,
	0xFE91FA0D, 0xF60DF62E, 0xA908D9ED, 0xE5EEE9EE, 0x840090A3, 0xC66CC24B, 0xC480B526, 0xEFB3DB6F,
	0xFA4FFE71, 0xFA91D8A4, 0xE5EED5ED, 0xD18BCD8B, 0xCEEEAE6B, 0xA000D800, 0xB5C8C820, 0xE800AC00,
	0xD4C5FAD3, 0xFE91FE50, 0xCD8BCD6A, 0xC18AC98B, 0xE000AC20, 0xCAEE8421, 0xC000BA2B, 0xF7D7A526,
	0xFE2EFE4F, 0xFAB2C18A, 0xD1CCC58B, 0x9CA59CE7, 0x954B8863, 0xE739F7BC, 0xAEB88864, 0xFBDEF39A,
	0x8C20F621, 0xF622FE86, 0xF2F2F601, 0xFE63D141, 0xFFFFFAAA, 0xFE019560, 0xFBFFFEEF, 0xF9A0A1C6,
	0xAD2181A0, 0xDAB6FFFF, 0x82208B40, 0x85E1C1F0, 0x83008EE1, 0x8B008280, 0x872182A0, 0x82A086A0,
	0xFFFFFFFF, 0xFBDED6B5, 0xF75DFFDF, 0xEF5BDEF7, 0x858191C3, 0xE7B8FFFF, 0x82E09203, 0xFBFEEF7C,
	0xB16C8982, 0x82E08AE1, 0xFFFFF7BD, 0x9DC68AA0, 0xC611CA52, 0xCE348843, 0x80408422, 0x84428200,
	0x8AC18AA1, 0x86808260, 0x8B018B00, 0x86E08AE0, 0x80A08541, 0x854184C1, 0x8A208021, 0x952AB31C,
	0x82608B21, 0x856199AF, 0x8AC18500, 0x8CC8B75E, 0x88849DF1, 0xB75EB75E, 0xBB5EBB5E, 0xBB5EBB5E
};

//.sbss
static CardWork* wp;

//.sdata
const char* version = "009";

//local prototypes
void read_all_main(void);
void create_main(void);
void mountDetachCallback(s32 chan, s32 result);
void mountAttachCallback(s32 chan, s32 result);
void checkCallback(s32 chan, s32 result);
void formatCallback(s32 chan, s32 result);
void createCallback(s32 chan, s32 result);
void writeCallback(s32 chan, s32 result);
void readCallback(s32 chan, s32 result);
void statusCallback(s32 chan, s32 result);
void memcard_open(void);

//close, not 1:1, requires s32 loop, status;
#define CardPollFunction(function)								\
	loop = 0;													\
	while (status = (function), status == CARD_RESULT_BUSY) {	\
		if (++loop > 1000000) {									\
			status = CARD_RESULT_FATAL_ERROR;					\
			break;												\
		}														\
	}

CardData* cardGetFilePtr(void) {
	return wp->data;
}

void cardBufReset(void) {
	OSCalendarTime calendar;
	CardSaveSlot* save;
	CardMetadata* data;
	u32 checksum;
	int i, slot;

	if (wp->flags & 1) {
		CARDCancel(&wp->info);
		for (i = 0; i < 1000000; i++) {
			if (!(wp->flags & 1)) {
				break;
			}
		}
		wp->flags &= ~1;
	}
	data = &wp->data->metadata;
	memset(data, 0, sizeof(data->data));
	strcpy(data->data.game_string, "\x83\x79\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x8A\x83\x49\x82\x71\x82\x6F\x82\x66"); //"ペーパーマリオＲＰＧ"
	OSTicksToCalendarTime(OSGetTime(), &calendar);
	sprintf(data->data.date_string, "\x25\x32\x64\x8C\x8E\x25\x32\x64\x93\xFA\x82\xCC\x83\x5A\x81\x5B\x83\x75\x83\x66\x81\x5B\x83\x5E\x82\xC5\x82\xB7", calendar.mon + 1, calendar.mday); //"%2d月%2d日のセーブデータです"
	memcpy(data->data.banner_tex, _mariost_banner_tex, sizeof(_mariost_banner_tex));
	memcpy(data->data.icon_tex, _mariost_icon_tex, sizeof(_mariost_icon_tex));
	memcpy(data->data.icon_tlut, _mariost_icon_tlut, sizeof(_mariost_icon_tlut));
	data->data.field_0x1E40 = (u16)(gp->field_0x1294 != 0);
	data->data.field_0x1E42 = (u16)gp->field_0x1274;
	data->data.field_0x1E44 = (u16)gp->field_0x11B8;
	data->checksum1 = 0;
	data->checksum2 = 0xFFFFFFFF;
	checksum = 0;
	for (i = 0; i < sizeof(data->data); i++) {
		checksum += *(u8*)((u32)&data + i);
	}
	data->checksum1 = checksum;
	data->checksum2 = ~data->checksum1;

	for (slot = 0; slot < 4; slot++) {
		if (slot == -1) {
			save = wp->tempSave;
		}
		else {
			save = &wp->data->saves[slot];
		}
		memset(save, 0, sizeof(save->save));
		save->save.flags |= 1;
		strcpy(save->version, version);
		save->size = sizeof(save->save);
		save->checksum1 = 0;
		save->checksum2 = 0xFFFFFFFF;
		checksum = 0;
		for (i = 0; i < sizeof(save->save); i++) {
			checksum += *(u8*)((u32)&save + i);
		}
		save->checksum1 = checksum;
		save->checksum2 = ~save->checksum1;
	}
	wp->channel = -1;
	wp->fileNo = -1;
	wp->serialNo = 0;
	wp->state = 0;
	wp->resultcode = CARD_RESULT_NOCARD;
	wp->flags = 0;
}

void cardInit(void) {
	OSCalendarTime calendar;
	CardSaveSlot* save;
	CardMetadata* data;
	u32 checksum;
	int i, slot;

	wp = __memAlloc(HEAP_DEFAULT, sizeof(CardWork));
	memset(wp, 0, sizeof(CardWork));

	wp->workArea = __memAlloc(HEAP_DEFAULT, CARDMGR_WORKAREA_SIZE);
	memset(wp->workArea, 0, CARDMGR_WORKAREA_SIZE);

	wp->data = __memAlloc(HEAP_DEFAULT, sizeof(CardData));
	memset(wp->data, 0, sizeof(CardData));

	wp->currentSave = __memAlloc(HEAP_DEFAULT, sizeof(CardSaveSlot));
	memset(wp->currentSave, 0, sizeof(CardSaveSlot));

	wp->tempSave = __memAlloc(HEAP_DEFAULT, sizeof(CardSaveSlot));
	memset(wp->tempSave, 0, sizeof(CardSaveSlot));

	data = &wp->data->metadata;
	memset(data, 0, sizeof(data->data));
	strcpy(data->data.game_string, "\x83\x79\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x8A\x83\x49\x82\x71\x82\x6F\x82\x66"); //"ペーパーマリオＲＰＧ"
	OSTicksToCalendarTime(OSGetTime(), &calendar);
	sprintf(data->data.date_string, "\x25\x32\x64\x8C\x8E\x25\x32\x64\x93\xFA\x82\xCC\x83\x5A\x81\x5B\x83\x75\x83\x66\x81\x5B\x83\x5E\x82\xC5\x82\xB7", calendar.mon + 1, calendar.mday); //"%2d月%2d日のセーブデータです"
	memcpy(data->data.banner_tex, _mariost_banner_tex, sizeof(_mariost_banner_tex));
	memcpy(data->data.icon_tex, _mariost_icon_tex, sizeof(_mariost_icon_tex));
	memcpy(data->data.icon_tlut, _mariost_icon_tlut, sizeof(_mariost_icon_tlut));
	data->data.field_0x1E40 = (u16)(gp->field_0x1294 != 0);
	data->data.field_0x1E42 = (u16)gp->field_0x1274;
	data->data.field_0x1E44 = (u16)gp->field_0x11B8;
	data->checksum1 = 0;
	data->checksum2 = 0xFFFFFFFF;
	checksum = 0;
	for (i = 0; i < sizeof(data->data); i++) {
		checksum += *(u8*)((u32)&data + i);
	}
	data->checksum1 = checksum;
	data->checksum2 = ~data->checksum1;

	for (slot = 0; slot < 4; slot++) {
		if (slot == -1) {
			save = wp->tempSave;
		}
		else {
			save = &wp->data->saves[slot];
		}
		memset(save, 0, sizeof(save->save));
		save->save.flags |= 1;
		strcpy(save->version, version);
		save->size = sizeof(save->save);
		save->checksum1 = 0;
		save->checksum2 = 0xFFFFFFFF;
		checksum = 0;
		for (i = 0; i < sizeof(save->save); i++) {
			checksum += *(u8*)((u32)&save + i);
		}
		save->checksum1 = checksum;
		save->checksum2 = ~save->checksum1;
	}
	wp->channel = -1;
	wp->fileNo = -1;
	wp->serialNo = 0;
	wp->state = 0;
	wp->resultcode = CARD_RESULT_NOCARD;
	wp->flags = 0;
	CARDInit();
}







BOOL cardIsExec(void) {
	return TRUE;
}


void cardMain(void) {

}





void read_all_main(void) {
	int loop;
	s32 status;
	CardMetadata* metadata;
	BOOL validData;
	u32 checksum;
	CardSaveSlot* save;

	s32 memSize;
	s32 sectorSize;
	OSCalendarTime calendar;
	int i;

	if (wp->resultcode != CARD_RESULT_READY) {
		wp->state = 8;
		wp->flags |= 0x200u;
		wp->flags &= ~2;
	}
	else if (!(wp->flags & 1)) {
		switch (wp->step) {
			case 0:
				wp->flags |= 1;
				CardPollFunction(CARDProbeEx(0, &memSize, &sectorSize));
				if (status == CARD_RESULT_READY && sectorSize != 0x2000) {
					status = CARD_RESULT_WRONGDEVICE;
				}
				if (status != CARD_RESULT_READY) {
					if (status == CARD_RESULT_WRONGDEVICE) {
						wp->resultcode = CARD_RESULT_WRONGDEVICE;
						wp->channel = 0;
					}
					else {
						wp->resultcode = status;
						wp->channel = -1;
					}
				}
				else
				{
					wp->resultcode = CARD_RESULT_READY;
					wp->channel = 0;
				}
				wp->flags &= ~1;
				break;
			case 1:
				CARDSetFastMode(TRUE);
				wp->flags |= 1;
				CARDMountAsync(wp->channel, wp->workArea, mountDetachCallback, mountAttachCallback);
				break;
			case 2:
				wp->flags |= 1;
				CARDCheckAsync(wp->channel, checkCallback);
				break;
			case 3:
				memcard_open();
				break;
			case 4:
				wp->flags |= 1;
				CARDReadAsync(&wp->info, wp->data, sizeof(CardSaveSlot), 0, readCallback);
				break;
			case 5:
				metadata = &wp->data->metadata;
				if (metadata->checksum2 == ~metadata->checksum1) {
					checksum = 0;
					for (i = 0; i < 0x1E46; i++) {
						checksum += *(u8*)((u32)&metadata + i);
					}
					validData = metadata->checksum1 == checksum;
				}
				else {
					validData = FALSE;
				}

				if (!validData) {
					memset(metadata, 0, 0x1E46); //TODO: un-hardcode?
					strcpy(metadata->data.game_string, "\x83\x79\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x8A\x83\x49\x82\x71\x82\x6F\x82\x66"); //"ペーパーマリオＲＰＧ"
					OSTicksToCalendarTime(OSGetTime(), &calendar);
					sprintf(metadata->data.date_string, "\x25\x32\x64\x8C\x8E\x25\x32\x64\x93\xFA\x82\xCC\x83\x5A\x81\x5B\x83\x75\x83\x66\x81\x5B\x83\x5E\x82\xC5\x82\xB7", calendar.mon + 1, calendar.mday); //"%2d月%2d日のセーブデータです"
					memcpy(metadata->data.banner_tex, _mariost_banner_tex, sizeof(_mariost_banner_tex));
					memcpy(metadata->data.icon_tex, _mariost_icon_tex, sizeof(_mariost_icon_tex));
					memcpy(metadata->data.icon_tlut, _mariost_icon_tlut, sizeof(_mariost_icon_tlut));
					metadata->data.field_0x1E40 = (u16)(gp->field_0x1294 != 0);
					metadata->data.field_0x1E42 = (u16)gp->field_0x1274;
					metadata->data.field_0x1E44 = (u16)gp->field_0x11B8;
					metadata->checksum1 = 0;
					metadata->checksum2 = 0xFFFFFFFF;
					checksum = 0;
					for (i = 0; i < 0x1E46; i++) {
						checksum += *(u8*)((u32)&metadata + i);
					}
					metadata->checksum1 = checksum;
					metadata->checksum2 = ~metadata->checksum1;
					wp->flags |= 0x400;
				}
				gp->field_0x1274 = metadata->data.field_0x1E42;
				gp->field_0x1294 = metadata->data.field_0x1E40;
				gp->field_0x11B8 = metadata->data.field_0x1E44;
				break;
			case 6:
				wp->flags |= 1;
				CARDReadAsync(&wp->info, wp->currentSave, sizeof(CardSaveSlot), (s32)(sizeof(CardMetadata) + (sizeof(CardSaveSlot) * wp->currentSlot)), readCallback);
				break;
			case 7:
				save = wp->currentSave;
				if (save->size == 0x2140) {
					if (strcmp(save->version, version)) {
						validData = FALSE;
					}
					else {
						if (save->checksum2 == ~save->checksum1) {
							checksum = 0;
							for (i = 0; i < 0x2140; i++) {
								checksum += *(u8*)((u32)&save + i);
							}
							validData = save->checksum1 == checksum;
						}
						else {
							validData = FALSE;
						}
					}
				}
				else {
					validData = FALSE;
				}
				//TODO: finish
				break;






			
		}
	}
	else {
		if (CARDGetResultCode(wp->channel) != CARD_RESULT_BUSY &&
			CARDGetResultCode(wp->channel) != wp->resultcode) {
			wp->resultcode = CARDGetResultCode(wp->channel);
			wp->flags &= ~1;
		}
	}
}

void cardCreate(void) {
	if (!(wp->flags & 2)) {
		wp->state = 5;
		wp->step = 0;
		wp->resultcode = CARD_RESULT_READY;
		wp->flags |= 2;
		wp->flags &= ~0x700;
		wp->flags &= ~0x2000;
		wp->flags &= ~0x800;
	}
}

void create_main(void) { //finished, need to fix CardPollFunction and regalloc
	s32 loop, status;
	CardMetadata* metadata;
	CardSaveSlot* save;
	u32 checksum;
	s32 sectorSize;
	s32 memSize;
	OSCalendarTime calendar;
	int i, slot;

	if (wp->resultcode != CARD_RESULT_READY) {
		wp->state = 8;
		wp->flags |= 0x200;
		wp->flags &= ~2;
	}
	else if (!(wp->flags & 1)) {
		switch (wp->step) {
			case 0:
				wp->flags |= 1;
				CardPollFunction(CARDProbeEx(0, &memSize, &sectorSize));
				if (status == CARD_RESULT_READY && sectorSize != 0x2000) {
					status = CARD_RESULT_WRONGDEVICE;
				}
				if (status != CARD_RESULT_READY) {
					if (status == CARD_RESULT_WRONGDEVICE) {
						wp->resultcode = CARD_RESULT_WRONGDEVICE;
						wp->channel = 0;
					}
					else {
						wp->resultcode = status;
						wp->channel = -1;
					}
				}
				else
				{
					wp->resultcode = CARD_RESULT_READY;
					wp->channel = 0;
				}
				wp->flags &= ~1;
				break;
			case 1:
				CARDSetFastMode(TRUE);
				wp->flags |= 1;
				CARDMountAsync(wp->channel, wp->workArea, mountDetachCallback, mountAttachCallback);
				break;
			case 2:
				wp->flags |= 1;
				CARDCheckAsync(wp->channel, checkCallback);
				break;
			case 3:
				wp->flags |= 1;
				CardPollFunction(CARDDelete(wp->channel, "mariost_save_file"));
				if (status == CARD_RESULT_NOFILE) {
					status = CARD_RESULT_READY;
				}
				wp->resultcode = status;
				wp->flags &= ~1;
				break;
			case 4:
				wp->flags |= 1;
				CARDCreateAsync(wp->channel, "mariost_save_file", 0x22000, &wp->info, createCallback);
				break;
			case 5:
				metadata = &wp->data->metadata;
				memset(metadata, 0, 0x1E46); //TODO: un-hardcode?
				strcpy(metadata->data.game_string, "\x83\x79\x81\x5B\x83\x70\x81\x5B\x83\x7D\x83\x8A\x83\x49\x82\x71\x82\x6F\x82\x66"); //"ペーパーマリオＲＰＧ"
				OSTicksToCalendarTime(OSGetTime(), &calendar);
				sprintf(metadata->data.date_string, "\x25\x32\x64\x8C\x8E\x25\x32\x64\x93\xFA\x82\xCC\x83\x5A\x81\x5B\x83\x75\x83\x66\x81\x5B\x83\x5E\x82\xC5\x82\xB7", calendar.mon + 1, calendar.mday); //"%2d月%2d日のセーブデータです"
				memcpy(metadata->data.banner_tex, _mariost_banner_tex, sizeof(_mariost_banner_tex));
				memcpy(metadata->data.icon_tex, _mariost_icon_tex, sizeof(_mariost_icon_tex));
				memcpy(metadata->data.icon_tlut, _mariost_icon_tlut, sizeof(_mariost_icon_tlut));
				metadata->data.field_0x1E40 = (u16)(gp->field_0x1294 != 0);
				metadata->data.field_0x1E42 = (u16)gp->field_0x1274;
				metadata->data.field_0x1E44 = (u16)gp->field_0x11B8;
				metadata->checksum1 = 0;
				metadata->checksum2 = 0xFFFFFFFF;
				checksum = 0;
				for (i = 0; i < 0x1E46; i++) {
					checksum += *(u8*)((u32)&metadata + i);
				}
				metadata->checksum1 = checksum;
				metadata->checksum2 = ~metadata->checksum1;

				for (slot = 0; slot < 4; slot++) {
					if (slot == -1) {
						save = wp->tempSave;
					}
					else {
						save = &wp->data->saves[slot];
					}
					memset(save, 0, 0x2140); //TODO: unhardcode?
					save->save.flags |= 1;
					strcpy(save->version, version);
					save->size = 0x2140;
					save->checksum1 = 0;
					save->checksum2 = 0xFFFFFFFF;
					checksum = 0;
					for (i = 0; i < 0x2140; i++) {
						checksum += *(u8*)((u32)&save + i);
					}
					save->checksum1 = checksum;
					save->checksum2 = ~save->checksum1;
				}
				wp->flags |= 1;
				CARDWriteAsync(&wp->info, wp->data, sizeof(CardData), 0, writeCallback);
				break;
			case 6:
				wp->flags |= 1;
				CARDWriteAsync(&wp->info, wp->data->saves, sizeof(CardSaveSlot) * 4, sizeof(CardData), writeCallback);
				break;
			case 7:
				wp->flags |= 1;
				CardPollFunction(CARDGetStatus(wp->channel, wp->info.fileNo, &wp->stat));
				wp->resultcode = status;
				wp->flags &= ~1;
				break;
			case 8:
				wp->stat.commentAddr = 0;
				wp->stat.iconAddr = 0x40;
				wp->stat.bannerFormat = (u8)((wp->stat.bannerFormat & ~CARD_STAT_BANNER_MASK) | CARD_STAT_BANNER_RGB5A3);
				wp->stat.bannerFormat &= ~4;
				wp->stat.iconFormat = (u8)((wp->stat.iconFormat & ~CARD_STAT_ICON_MASK) | CARD_STAT_ICON_C8);
				wp->stat.iconSpeed = (u8)((wp->stat.iconSpeed & ~CARD_STAT_SPEED_MASK) | CARD_STAT_SPEED_MIDDLE);
				wp->stat.iconFormat &= ~0xC;
				wp->stat.iconSpeed &= ~0xC;
				break;
			case 9:
				wp->flags |= 1;
				CARDSetStatusAsync(wp->channel, wp->info.fileNo, &wp->stat, statusCallback);
				break;
			case 10:
				wp->flags |= 1;
				CardPollFunction(CARDGetSerialNo(wp->channel, &wp->serialNo));
				wp->resultcode = status;
				wp->flags &= ~1;
				break;
			case 11:
				wp->flags |= 1;
				CardPollFunction(CARDClose(&wp->info));
				wp->resultcode = status;
				wp->flags &= ~1;
				break;
			case 12:
				wp->flags |= 1;
				CardPollFunction(CARDUnmount(wp->channel));
				wp->resultcode = status;
				wp->flags &= ~1;
				break;
			case 13:
				wp->state = 0;
				wp->flags |= 0x100;
				wp->flags |= 0x2000;
				wp->flags &= ~2;
				break;
			default:
				break;
		}
		wp->step++;
	}
	else {
		if (CARDGetResultCode(wp->channel) != CARD_RESULT_BUSY &&
			CARDGetResultCode(wp->channel) != wp->resultcode) {
			wp->resultcode = CARDGetResultCode(wp->channel);
			wp->flags &= ~1;
		}
	}
}

void cardFormat(void) {
	if (!(wp->flags & 2)) {
		wp->state = 6;
		wp->step = 0;
		wp->resultcode = CARD_RESULT_READY;
		wp->flags |= 2;
		wp->flags &= ~0x700;
		wp->flags &= ~0x2000;
	}
}

void mountDetachCallback(s32 chan, s32 result) {
	; //stubbed in retail
}

void mountAttachCallback(s32 chan, s32 result) {
	wp->resultcode = result;
	if (result == CARD_RESULT_BROKEN || result == CARD_RESULT_ENCODING) {
		wp->resultcode = CARD_RESULT_READY;
	}
	wp->flags &= ~1;
}

void checkCallback(s32 chan, s32 result) {
	wp->resultcode = result;
	wp->flags &= ~1;
}

void formatCallback(s32 chan, s32 result) {
	wp->resultcode = result;
	wp->flags &= ~1;
}

void createCallback(s32 chan, s32 result) {
	wp->resultcode = result;
	wp->flags &= ~1;
}

void writeCallback(s32 chan, s32 result) {
	wp->resultcode = result;
	wp->flags &= ~1;
}

void readCallback(s32 chan, s32 result) {
	wp->resultcode = result;
	wp->flags &= ~1;
}

void statusCallback(s32 chan, s32 result) {
	wp->resultcode = result;
	wp->flags &= ~1;
}

void memcard_open(void) { //finished, need to fix CardPollFunction and regalloc
	int i, loop;
	s32 fileNo;
	s32 status;
	s32 filesNotUsed;
	s32 bytesNotUsed;
	u64 serialNo;
	CARDStat stat;

	serialNo = 0;
	wp->flags |= 1;
	if (wp->flags & 0x2000) {
		CardPollFunction(CARDGetSerialNo(wp->channel, &serialNo));
		if (status != CARD_RESULT_READY) {
			wp->resultcode = status;
			wp->flags &= ~1;
			return;
		}
		if (serialNo != wp->serialNo) {
			wp->resultcode = CARD_RESULT_NOPERM;
			wp->flags &= ~1;
			return;
		}
	}
	fileNo = 0;
	for (i = 0; i < 127; i++) {
		CardPollFunction(CARDGetStatus(wp->channel, fileNo, &stat));
		if (status != CARD_RESULT_NOFILE && status != CARD_RESULT_NOPERM) {
			if (status == CARD_RESULT_READY) {
				if (!strcmp(stat.fileName, "mariost_save_file")) {
					if (stat.commentAddr || stat.iconAddr != 0x40 || stat.length != 0x22000) {
						wp->fileNo = fileNo;
						wp->resultcode = CARD_RESULT_NOFILE;
						wp->flags &= ~1;
						return;
					}
					wp->fileNo = fileNo;
					break;
				}
			}
			if (status == CARD_RESULT_FATAL_ERROR || status == CARD_RESULT_NOCARD) {
				wp->resultcode = status;
				wp->flags &= ~1;
				return;
			}
		}
	}
	if (fileNo < 127) {
		CardPollFunction(CARDOpen(wp->channel, "mariost_save_file", &wp->info));
		wp->resultcode = status;
		wp->flags &= ~1;
	}
	else {
		CardPollFunction(CARDFreeBlocks(wp->channel, &bytesNotUsed, &filesNotUsed));
		if (status) {
			wp->resultcode = status;
			wp->flags &= ~1;
		}
		else if (bytesNotUsed >= 0x22000) {
			if (filesNotUsed >= 1) {
				wp->resultcode = CARD_RESULT_NOFILE;
			}
			else {
				wp->resultcode = CARD_RESULT_NOENT;
			}
			wp->flags &= ~1;
		}
		else {
			wp->resultcode = CARD_RESULT_INSSPACE;
			wp->flags &= ~1;
		}
	}
}
