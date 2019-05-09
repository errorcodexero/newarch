//Code to override values for outputs
#ifndef FORCE_H
#define FORCE_H

#include "../util/interface.h"

struct Force{
	Robot_outputs values;
	//Each bool sets whether or not the value is overridden by Force
	std::bitset<Robot_outputs::PWMS> pwm;
	std::bitset<Robot_outputs::SOLENOIDS> solenoid;
	std::bitset<Robot_outputs::RELAYS> relay;
	std::bitset<Robot_outputs::DIGITAL_IOS> digital_io;
	
	Force();
	
	Robot_outputs operator()(Robot_outputs)const;
};
bool operator==(Force,Force);
bool operator!=(Force,Force);
std::ostream& operator<<(std::ostream&,Force);

#endif
