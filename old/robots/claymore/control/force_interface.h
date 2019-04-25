#ifndef FORCE_INTERFACE_H
#define FORCE_INTERFACE_H

#include<iosfwd>
#include "../util/posedge_trigger.h"
#include "force.h"

class Force_interface{
	public:
	typedef enum{PWM,SOLENOID,RELAY,DIO,B0,B1,BUTTONS} Button;

	static const unsigned LOCATION_BITS=4;
	static const unsigned VALUE_BITS=2;
	static const unsigned LEN=1+LOCATION_BITS+VALUE_BITS;

	Force force;
	Posedge_trigger button[BUTTONS];
	Button recent[LEN];
	void update(Button b);
	int pwm(unsigned,int);
	int solenoid(unsigned,int);
	int relay(unsigned,int);
	int dio(unsigned,int);
	
	public:
	Force_interface();
	void update(bool pwm,bool solenoid,bool relay,bool dio,bool b0,bool b1);
	Robot_outputs operator()(Robot_outputs)const;
	Force const& get()const;

	friend std::ostream& operator<<(std::ostream&,Force_interface const&);
};

bool operator==(Force_interface,Force_interface);
bool operator!=(Force_interface,Force_interface);
std::ostream& operator<<(std::ostream&,Force_interface::Button);
std::ostream& operator<<(std::ostream&,Force_interface const&);

#endif
