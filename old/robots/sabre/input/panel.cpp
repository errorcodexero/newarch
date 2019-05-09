#include "panel.h"
#include <iostream>
#include <stdlib.h> 
#include "util.h"
#include "../util/util.h"
#include <cmath>

using namespace std;

Panel::Panel():
	in_use(0),
	learn(0),
	cheval(0),
	drawbridge(0),
	shoot_prep(0),
	shoot_low(0),
	collect(0),
	shoot_high(0),
	collector_up(0),
	collector_down(0),
	lock_climber(0),
	tilt_auto(0),
	front_auto(0),
	sides_auto(0),
	collector_pos(Collector_pos::DEFAULT),
	front(Collector::OFF),
	sides(Collector::OFF),
	winch(Winch::STOP),
	shooter_mode(Shooter_mode::CLOSED_AUTO),
	auto_mode(Auto_mode::NOTHING),
	auto_switch(0),
	speed_dial(0)
{}

ostream& operator<<(ostream& o,Panel::Collector_pos a){
	o<<"Panel::Collector_pos(";
	#define X(name) if(a==Panel::Collector_pos::name)o<<""#name;
	X(STOW) X(DEFAULT) X(LOW)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Panel::Collector a){
	o<<"Panel::Collector(";
	#define X(name) if(a==Panel::Collector::name)o<<""#name;
	X(IN) X(OUT) X(OFF)	
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Panel::Winch a){
	o<<"Panel::Winch(";
	#define X(name) if(a==Panel::Winch::name)o<<""#name;
	X(UP) X(STOP) X(DOWN)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Panel::Shooter_mode a){
	o<<"Panel::Shooter_mode(";
	#define X(name) if (a==Panel::Shooter_mode::name)o<<""#name;
	X(OPEN) X(CLOSED_MANUAL) X(CLOSED_AUTO)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Panel::Auto_mode a){
	o<<"Panel::Auto_mode(";
	#define X(name) if(a==Panel::Auto_mode::name)o<<""#name;
	X(NOTHING) X(REACH) X(STATICS) X(STATICF) X(PORTCULLIS) X(CHEVAL) X(LBLS) X(LBWLS) X(LBWHS) X(S)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Panel p){
	o<<"Panel(";
	o<<"in_use:"<<p.in_use;
	#define X(name) o<<", "#name":"<<p.name;
	X(learn) X(cheval) X(drawbridge) X(shoot_prep) X(shoot_low) X(collect) X(shoot_high) X(collector_up) X(collector_down) //buttons
	X(lock_climber) X(tilt_auto) X(front_auto) X(sides_auto) //2-pos switches
	X(collector_pos) X(front) X(sides) X(winch) X(shooter_mode) //3-pos switches
	X(auto_mode) //10-pos switches
	X(auto_switch)
	X(speed_dial) //Dials
	#undef X
	return o<<")";
}

Panel::Auto_mode auto_mode_convert(int potin){
	switch(potin) {
		case 0:
			return Panel::Auto_mode::NOTHING;
		case 1:
			return Panel::Auto_mode::REACH;
		case 2:
			return Panel::Auto_mode::STATICS;
		case 3:
			return Panel::Auto_mode::STATICF;
		case 4:
			return Panel::Auto_mode::PORTCULLIS;
		case 5: 
			return Panel::Auto_mode::CHEVAL;
		case 6:
			return Panel::Auto_mode::LBLS;
		case 7:
			return Panel::Auto_mode::LBWLS;
		case 8:
			return Panel::Auto_mode::LBWHS;	
		case 9: 
			return Panel::Auto_mode::S;
		default:
			return Panel::Auto_mode::NOTHING;
	}
}

float axis_to_percent(double a){
	return .5-(a/2);
}

Panel interpret(Joystick_data d){
	Panel p;
	{
		p.in_use=[&](){
			for(int i=0;i<JOY_AXES;i++) {
				if(d.axis[i]!=0)return true;
			}
			for(int i=0;i<JOY_BUTTONS;i++) {
				if(d.button[i]!=0)return true;
			}
			return false;
		}();
		if (!p.in_use) return p;
	}
	{
		Volt auto_mode=d.axis[0];
		p.auto_switch=interpret_10_turn_pot(auto_mode);
		p.auto_mode=auto_mode_convert(p.auto_switch);
	}
	p.lock_climber = d.button[0];
	p.tilt_auto = d.button[1];
	p.sides_auto = d.button[2];
	p.front_auto = d.button[3];
	#define AXIS_RANGE(axis, last, curr, next, var, val) if (axis > curr-(curr-last)/2 && axis < curr+(next-curr)/2) var = val;
	{
		float op = d.axis[2];
		static const float DEFAULT=-1, COLLECTOR_UP=-.8, COLLECTOR_DOWN=-.62, SHOOT_HIGH=-.45, COLLECT=-.29, SHOOT_LOW=-.11, SHOOT_PREP=.09, DRAWBRIDGE=.33, CHEVAL=.62, LEARN=1;
		#define X(button) p.button = 0;
		X(collector_up) X(collector_down) X(shoot_high) X(collect) X(shoot_low) X(shoot_prep) X(drawbridge) X(cheval) X(learn)
		#undef X
		AXIS_RANGE(op, DEFAULT, COLLECTOR_UP, COLLECTOR_DOWN, p.collector_up, 1)
		else AXIS_RANGE(op, COLLECTOR_UP, COLLECTOR_DOWN, SHOOT_HIGH, p.collector_down, 1)
		else AXIS_RANGE(op, COLLECTOR_DOWN, SHOOT_HIGH, COLLECT, p.shoot_high, 1)
		else AXIS_RANGE(op, SHOOT_HIGH, COLLECT, SHOOT_LOW, p.collect, 1)
		else AXIS_RANGE(op, COLLECT, SHOOT_LOW, SHOOT_PREP, p.shoot_low, 1)
		else AXIS_RANGE(op, SHOOT_LOW, SHOOT_PREP, DRAWBRIDGE, p.shoot_prep, 1)
		else AXIS_RANGE(op, SHOOT_PREP, DRAWBRIDGE, CHEVAL, p.drawbridge, 1)
		else AXIS_RANGE(op, DRAWBRIDGE, CHEVAL, LEARN, p.cheval, 1)
		else AXIS_RANGE(op, CHEVAL, LEARN, 1.38, p.learn, 1)
	}
	{
		float collector_pos = d.axis[5];
		static const float LOW=-1, DEFAULT=0, STOW=1;
		p.collector_pos = Panel::Collector_pos::LOW;
		AXIS_RANGE(collector_pos, LOW, DEFAULT, STOW, p.collector_pos, Panel::Collector_pos::DEFAULT)
		else AXIS_RANGE(collector_pos, DEFAULT, STOW, 1.5, p.collector_pos, Panel::Collector_pos::STOW)
	}
	{
		float front = d.axis[4];
		static const float OUT=-1, OFF=.48, IN=1;
		p.front = Panel::Collector::OUT;
		AXIS_RANGE(front, OUT, OFF, IN, p.front, Panel::Collector::OFF)
		else AXIS_RANGE(front, OFF, IN, 1.5, p.front, Panel::Collector::IN)
	}
	{
		float sides = d.axis[6];
		static const float OUT=-1, OFF=0, IN=1;
		p.sides = Panel::Collector::OUT;
		AXIS_RANGE(sides, OUT, OFF, IN, p.sides, Panel::Collector::OFF)
		else AXIS_RANGE(sides, OFF, IN, 1.5, p.sides, Panel::Collector::IN)
	}
	{
		float winch = d.axis[3];
		static const float UP=-1, STOP=0, DOWN=1;
		p.winch = Panel::Winch::UP;
		AXIS_RANGE(winch, UP, STOP, DOWN, p.winch, Panel::Winch::STOP)
		else AXIS_RANGE(winch, STOP, DOWN, 1.5, p.winch, Panel::Winch::DOWN)
	}
	{
		//A three position switch connected to two digital inputs
		p.shooter_mode = Panel::Shooter_mode::CLOSED_MANUAL;
		if (d.button[5]) p.shooter_mode = Panel::Shooter_mode::CLOSED_AUTO;
		if (d.button[6]) p.shooter_mode = Panel::Shooter_mode::OPEN;
	}
	p.speed_dial = -d.axis[1];//axis_to_percent(d.axis[1]);
	#undef AXIS_RANGE
	return p;
}

#ifdef PANEL_TEST
Joystick_data driver_station_input_rand(){
	Joystick_data r;
	for(unsigned i=0;i<JOY_AXES;i++){
		r.axis[i]=(0.0+rand()%101)/100;
	}
	for(unsigned i=0;i<JOY_BUTTONS;i++){
		r.button[i]=rand()%2;
	}
	return r;
}

int main(){
	Panel p;
	for(unsigned i=0;i<50;i++){
		interpret(driver_station_input_rand());
	}
	cout<<p<<"\n";
	return 0;
}
#endif
