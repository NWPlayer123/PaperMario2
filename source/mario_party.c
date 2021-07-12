// the best Mario Party, partners with actual abilities in Paper Mario,
// something Nintendo's forgotten about
#include "mario_party.h"

void partyJoin(MarioPartner partnerId) {
	if (partnerId < 8) {
		pouchGetPtr()->mPartyData[partnerId].mFlags |= 1; //obtained
	}
}

s32 marioGetPartyId(void) {

}