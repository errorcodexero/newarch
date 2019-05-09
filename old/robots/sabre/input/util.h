#ifndef INPUT_UTIL_H
#define INPUT_UTIL_H

#include<iosfwd>
#include "../util/interface.h"

enum class Joystick_section{LEFT,RIGHT,UP,DOWN,CENTER};
std::ostream& operator<<(std::ostream&,Joystick_section);

#define POV_SECTIONS X(UP) X(UP_LEFT) X(LEFT) X(DOWN_LEFT) X(DOWN) X(DOWN_RIGHT) X(RIGHT) X(UP_RIGHT) X(CENTER)
#define X(SECTION) SECTION,
enum class POV_section{POV_SECTIONS};
#undef X
std::ostream& operator<<(std::ostream&,POV_section);

Joystick_section joystick_section(double x,double y);
Joystick_section divide_vertical(double y);

POV_section pov_section(int x);

namespace Gamepad_button{
	//how the logitech gamepads appear in the driver station
	static const unsigned A=0,B=1,X=2,Y=3,LB=4,RB=5,BACK=6,START=7,L_JOY=8,R_JOY=9;
}
namespace Gamepad_axis{
	//How the axes appear in the DS; though
	static const unsigned LEFTX=0,LEFTY=1,LTRIGGER=2,RTRIGGER=3,RIGHTX=4,RIGHTY=5,DPADX=6;
	//DPADY does not exist, neither does axis 6
}

unsigned interpret_10_turn_pot(Volt);

#endif
