#ifndef PANEL_H
#define PANEL_H 

#include "message_logger.h"
#include "../util/maybe.h"
#include "../util/interface.h"
#include <sstream>

struct Panel{
	static const unsigned PORT = 2;
	bool in_use;
	//Buttons:
	bool floor;
	bool exchange;
	bool switch_;
	bool scale;
	bool collect_closed;
	bool collect_open;
	bool eject;
	bool drop;
	bool climb;
	bool wings;
	bool calibrate_grabber;
	bool calibrate_lifter;
	//2 position swicthes:
	bool grabber_auto;
	bool intake_auto;
	bool climb_disabled ;
	bool lifter_high_power;
	//3 position switches:
	enum class Grabber{CLOSE,OFF,OPEN};
	Grabber grabber;
	enum class Intake{IN,OFF,OUT};
	Intake intake;
	enum class Collection_end_height{EXCHANGE,SWITCH,SCALE};
	Collection_end_height collection_end_height;
	enum class Lifter{DOWN,OFF,UP};
	Lifter lifter;
	//10 position switches:
	int auto_select;
	//Dials:
	
	Panel();
};

bool operator!=(Panel const&,Panel const&);
std::ostream& operator<<(std::ostream&,Panel);

Panel interpret_oi(Joystick_data);
Panel interpret_test_oi(Joystick_data);
Panel interpret_gamepad(Joystick_data);

Joystick_data driver_station_input_rand();
Panel rand(Panel*);

inline messageLogger &operator<<(messageLogger &logger, const Panel &panel)
{
	std::stringstream strm ;

	strm << panel ;
	logger << strm.str() ;

	return logger ;
}

inline messageLogger &operator<<(messageLogger &logger, const Joystick_data &stick)
{
	std::stringstream strm ;

	strm << stick ;
	logger << strm.str() ;

	return logger ;
}

#endif
