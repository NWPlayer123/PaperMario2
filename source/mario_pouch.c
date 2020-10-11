#include "mario_pouch.h"

char* pouchGetYoshiName(void) {
	if (strlen(mpp->yoshi_name) <= 1)
		return mpp->yoshi_name;
	else
		return msgSearch("name_party3");
}

void pouchSetYoshiName(char* name) {
	strcpy(mpp->yoshi_name, name);
}

void pouchSetPartyColor(u32 member, u32 color) {
	mpp->party_data[member].flags = ((mpp->party_data[member].flags & 0x1FFF) & ~0xE000) | (color << 13);
}

u32 pouchGetPartyColor(u32 member) { //game doesn't specifically mask "& 7", TODO?
	return mpp->party_data[member].flags >> 13;
}

//simplified from source code, accepts 0x00-0x7F (that many bits in each array)
u32 pouchCheckMail(u32 id) {
	u32 byte_index = id >> 5; //they do div 32
	u32 bit_index = id & 31;
	if ((mpp->e_mail_read[byte_index] >> bit_index) & 1)
		return 2; //mail has been read
	else
		return (mpp->e_mail_received[byte_index] >> bit_index) & 1; //0 or 1, did_receive
}

//see above function
u32 pouchOpenMail(u32 id) {
	u32 byte_index = id >> 5;
	u32 bit_index = id & 31;
	mpp->e_mail_read[byte_index] |= (1 << bit_index);
}

