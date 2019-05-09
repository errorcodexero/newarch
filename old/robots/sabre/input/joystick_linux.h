#ifndef JOYSTICK_LINUX_H
#define JOYSTICK_LINUX_H

#include<map>
#include<string>
#include<vector>
#include "../util/interface.h"

struct Joystick_data_linux{
	std::string name;
	std::map<unsigned,int> axis;
	std::map<unsigned,bool> button;
};

std::ostream& operator<<(std::ostream&,Joystick_data_linux const&);

Joystick_data as_joystick_data(Joystick_data_linux const&);

std::vector<std::string> joystick_devices();

class Joystick_linux{
	Joystick_data_linux data;
	int fd;

	public:
	explicit Joystick_linux(const char *device_path);

	private:
	//return to these when we're using a C++11 compiler and make this class move-only.
	Joystick_linux(Joystick_linux const&);
	Joystick_linux& operator=(Joystick_linux const&);

	public:
	~Joystick_linux();

	Joystick_data_linux const& read();
};

std::ostream& operator<<(std::ostream&,Joystick_linux const&);

#endif
