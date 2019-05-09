#ifndef PANEL_H
#define PANEL_H 

#include "../util/maybe.h"
#include "../util/interface.h"

struct Panel{
	bool in_use;
	//Buttons:
	bool learn;
	bool cheval;
	bool drawbridge;
	bool shoot_prep;
	bool shoot_low;
	bool collect;
	bool shoot_high;
	bool collector_up;
	bool collector_down;
	//2 position swicthes:
	bool lock_climber;
	bool tilt_auto;
	bool front_auto;
	bool sides_auto;	
	//3 position switches: 
	enum class Collector_pos{STOW,DEFAULT,LOW};
	Collector_pos collector_pos;
	enum class Collector{IN,OFF,OUT};
	Collector front, sides;
	enum class Winch{UP,STOP,DOWN};
	Winch winch;
	enum class Shooter_mode{OPEN,CLOSED_MANUAL,CLOSED_AUTO};
	Shooter_mode shooter_mode;
	//10 position switches:

	//todo: remove this & put in main
	enum class Auto_mode{NOTHING,REACH,STATICF,STATICS,PORTCULLIS,CHEVAL,LBLS,LBWLS,LBWHS,S};
	Auto_mode auto_mode;
	int auto_switch;//0-9

	//Dials:
	float speed_dial;
	Panel();
};

std::ostream& operator<<(std::ostream&,Panel::Collector_pos);
std::ostream& operator<<(std::ostream&,Panel::Collector);
std::ostream& operator<<(std::ostream&,Panel::Winch);
std::ostream& operator<<(std::ostream&,Panel::Shooter_mode);
std::ostream& operator<<(std::ostream&,Panel::Auto_mode);
std::ostream& operator<<(std::ostream&,Panel);

Panel interpret(Joystick_data);

#endif
