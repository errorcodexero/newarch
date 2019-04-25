#include "side_decoder.h"
#include <cassert>

sideDecoder::sideDecoder(const std::string& sides) {
	sideString = sides;
}

sideDecoder::side sideDecoder::nearSwitch() {
	if (sideString[0] == 'L') {
		return side::LEFT;
	}
	else {
		return side::RIGHT;
	}
}
sideDecoder::side sideDecoder::scale() {
	if (sideString[1] == 'L') {
		return side::LEFT;
	}
	else {
		return side::RIGHT;
	}
}
sideDecoder::side sideDecoder::farSwitch() {
	if (sideString[2] == 'L') {
		return side::LEFT;
	}
	else {
		return side::RIGHT;
	}
}


#ifdef SIDE_DECODER_TEST

int main() {
	std::string sides = "RLL";
	sideDecoder decoder(sides);
	assert(decoder.nearSwitch() == sideDecoder::side::RIGHT);
	assert(decoder.scale() == sideDecoder::side::LEFT);
	assert(decoder.farSwitch() == sideDecoder::side::LEFT);
	return 0;
}

#endif