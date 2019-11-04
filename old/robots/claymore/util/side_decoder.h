#pragma once
#include <string>
 

class sideDecoder {
public:
	sideDecoder(const std::string& sides);
	enum class side { LEFT, RIGHT };
	side nearSwitch();
	side scale();
	side farSwitch();
private:
	std::string sideString;
};
