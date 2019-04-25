#include "panel.h"
#include "util.h"
#include "../util/util.h"
#include "../subsystems.h"
#include "message_logger.h"
#include <iostream>
#include <stdlib.h> 
#include <cmath>

using namespace std;

const unsigned GRABBER_AXIS = 2 ;
const unsigned COLLECTION_END_HEIGHT_AXIS = 3 ;
const unsigned INTAKE_AXIS = 4 ;
const unsigned AUTO_GRABBER_AXIS = 5 ;
const unsigned AUTO_SELECTOR_AXIS = 6;

#define BUTTONS \
	X(floor)\
	X(exchange)\
	X(switch_)\
	X(scale)\
	X(collect_closed)\
	X(collect_open)\
	X(eject)\
	X(drop)\
	X(climb)\
	X(wings)\
	X(calibrate_grabber)\
	X(calibrate_lifter)

#define TWO_POS_SWITCHES \
	X(grabber_auto)\
	X(intake_auto)\
	X(climb_disabled)\
	X(lifter_high_power)

#define THREE_POS_SWITCHES \
	X(grabber)\
	X(intake)\
	X(collection_end_height)\
	X(lifter)

#define TEN_POS_SWITCHES \
	X(auto_select)
#define DIALS \

#define PANEL_ITEMS \
	BUTTONS \
	TWO_POS_SWITCHES \
	THREE_POS_SWITCHES \
	TEN_POS_SWITCHES \
	DIALS

Panel::Panel():
	in_use(0),
	#define X(BUTTON) BUTTON(false),
	BUTTONS
	#undef X
	grabber(Panel::Grabber::OFF),
	intake(Panel::Intake::OFF),
	collection_end_height(Panel::Collection_end_height::EXCHANGE),
	lifter(Panel::Lifter::OFF),
	auto_select(0)
{}

ostream& operator<<(ostream& o,Panel::Grabber a){
	#define X(NAME) if(a==Panel::Grabber::NAME) return o<<""#NAME;
	X(CLOSE) X(OFF) X(OPEN)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Panel::Intake a){
	#define X(NAME) if(a==Panel::Intake::NAME) return o<<""#NAME;
	X(IN) X(OFF) X(OUT)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Panel::Collection_end_height a){
	#define X(NAME) if(a==Panel::Collection_end_height::NAME) return o<<""#NAME;
	X(EXCHANGE) X(SWITCH) X(SCALE)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Panel::Lifter a){
	#define X(NAME) if(a==Panel::Lifter::NAME) return o<<""#NAME;
	X(DOWN) X(OFF) X(UP)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Panel p){
	o<<"Panel(";
	o<<"in_use:"<<p.in_use;
	#define X(NAME) o<<", "#NAME":"<<p.NAME;
	PANEL_ITEMS
	#undef X
	return o<<")";
}

bool operator==(Panel const& a,Panel const& b){
	return true
	#define X(NAME) && a.NAME==b.NAME
	PANEL_ITEMS
	#undef X
	;
}

bool operator!=(Panel const& a,Panel const& b){
	return !(a==b);
}

float axis_to_percent(double a){
	return .5-(a/2);
}

bool set_button(const float AXIS_VALUE, const float LOWER_VALUE, const float TESTING_VALUE, const float UPPER_VALUE){
	float lower_tolerance = (TESTING_VALUE - LOWER_VALUE)/2;
	float upper_tolerance = (UPPER_VALUE - TESTING_VALUE)/2;
	float min = TESTING_VALUE - lower_tolerance;
	float max = TESTING_VALUE + upper_tolerance; 
	return (AXIS_VALUE > min && AXIS_VALUE < max);
}

bool get_in_use(Joystick_data d) {
	for(int i = 0; i < JOY_AXES; i++) {
		if(d.axis[i] != 0) return true;
	}
	for(int i = 0; i < JOY_BUTTONS; i++) {
		if(d.button[i] != 0) return true;
	}
	return false;
}

Panel interpret_oi(Joystick_data d){
	Panel p;
	static const float ARTIFICIAL_MAX = 1.5;
	{
		p.in_use=get_in_use(d);
		if(!p.in_use)
		{
			return p;
		}
	}
	{//set the auto mode number from the dial value
		float auto_dial_value = d.axis[AUTO_SELECTOR_AXIS];
		p.auto_select = interpret_10_turn_pot(auto_dial_value);
	}
	{//two position switches
		p.grabber_auto = [&]{
			float grabber_auto = d.axis[AUTO_GRABBER_AXIS];
			return (grabber_auto < 0) ? false : true;
		}();
		p.intake_auto = d.button[10];
		p.climb_disabled = !d.button[14];
		p.lifter_high_power = d.button[15];
	}
	{//three position switches
		p.grabber = [&]{
			float grabber = d.axis[GRABBER_AXIS];
			static const float CLOSE=-1,OFF=0,OPEN=1;
			if(set_button(grabber,CLOSE,OFF,OPEN)) return Panel::Grabber::OFF;
			if(set_button(grabber,OFF,OPEN,ARTIFICIAL_MAX)) return Panel::Grabber::CLOSE;
			return Panel::Grabber::OPEN;
		}();
		p.intake = [&]{
			float intake = d.axis[INTAKE_AXIS];
			static const float IN=-1,OFF=0,OUT=1;
			if(set_button(intake,IN,OFF,OUT)) return Panel::Intake::OFF;
			if(set_button(intake,OFF,OUT,ARTIFICIAL_MAX)) return Panel::Intake::IN;
			return Panel::Intake::OUT;
		}();
		p.collection_end_height = [&]{
			float collection_end_height = d.axis[COLLECTION_END_HEIGHT_AXIS];
			static const float SCALE=-1,SWITCH=0,EXCHANGE=1;
			if(set_button(collection_end_height,SCALE,SWITCH,EXCHANGE)) return Panel::Collection_end_height::SWITCH;
			if(set_button(collection_end_height,SWITCH,EXCHANGE,ARTIFICIAL_MAX)) return Panel::Collection_end_height::EXCHANGE;
			return Panel::Collection_end_height::SCALE;
		}();
		p.lifter = [&]{
			if(d.button[12]){
				return Panel::Lifter::UP;
			}
			if(d.button[13]){
				return Panel::Lifter::DOWN;
			}
			return Panel::Lifter::OFF;
		}();
	}	
	{//buttons
		p.floor = d.button[0];
		p.exchange = d.button[1];
		p.switch_ = d.button[2];
		p.scale = d.button[3];
		p.collect_closed = d.button[4];
		p.collect_open = d.button[9];
		p.eject = d.button[5];
		p.drop = d.button[6];
		p.climb = d.button[7];
		p.wings = d.button[8];
		p.calibrate_grabber = d.button[11];
		p.calibrate_lifter = d.button[11];
	}
	{//Dials
	}
	
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_PANEL);
	logger << "Panel:\n";
	logger << d << "\n";
	logger << p << "\n";
	logger.endMessage();
	
	return p;
}

Panel interpret_test_oi(Joystick_data d){	
	//static const unsigned int POTENTIOMETER_AXIS=1, TEN_POS_SWITCH_AXIS = 0; //TODO: need real values
	//static const unsigned int BUTTON_0_LOC=0, BUTTON_1_LOC=1, BUTTON_2_LOC=2, BUTTON_3_LOC=3, TWO_POS_SWITCH_0_LOC = 4, TWO_POS_SWITCH_1_LOC =5; //TODO: need real values

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
		if(!p.in_use) return p;
	}
	//p. = interpret_10_turn_pot(d.axis[TEN_POS_SWITCH_AXIS]); //set the switch value from the pot value
	{//two position switches
		//p. = d.button[TWO_POS_SWITCH_0_LOC];
		//p. = d.button[TWO_POS_SWITCH_1_LOC];
	}
	{//buttons
		//p. = d.button[BUTTON_0_LOC];
		//p. = d.button[BUTTON_1_LOC];
		//p. = d.button[BUTTON_2_LOC];
		//p. = d.button[BUTTON_3_LOC];
	}
	{//dials
		//p. = d.axis[POTENTIOMETER_AXIS];
	}
	return p;
}

Panel interpret_gamepad(Joystick_data d){
	Panel p;
	p.in_use = get_in_use(d);
	if(!p.in_use) return p;
	
	p.auto_select = 0;

	p.climb = d.axis[Gamepad_axis::LTRIGGER] > .1;

	p.lifter_high_power = d.axis[Gamepad_axis::RTRIGGER] > .1;

	p.climb_disabled = !d.button[Gamepad_button::START];
	p.wings = d.button[Gamepad_button::BACK];

	bool alternate_operation = d.button[Gamepad_button::RB];

	if(!alternate_operation) {
		p.grabber_auto = true;
		p.intake_auto = true;

		p.calibrate_grabber = d.button[Gamepad_button::LB];

		p.collect_closed = d.button[Gamepad_button::A];
		p.collect_open = d.button[Gamepad_button::B];
		p.eject = d.button[Gamepad_button::X];	
		p.drop = d.button[Gamepad_button::Y];	
	
		switch(pov_section(d.pov)){
			case POV_section::CENTER:
				break;
			case POV_section::UP:
				p.scale = true;
				break;
			case POV_section::UP_LEFT:
				break;
			case POV_section::LEFT:
				p.exchange = true;
				break;
			case POV_section::DOWN_LEFT:
				break;
			case POV_section::DOWN:
				p.floor = true;
				break;
			case POV_section::DOWN_RIGHT:
				break;
			case POV_section::RIGHT:
				p.switch_ = true;
				break;
			case POV_section::UP_RIGHT:
				break;
			default:
				assert(0);
		}	
	} else {
		p.grabber_auto = false;
		p.intake_auto = false;

		p.calibrate_lifter = d.button[Gamepad_button::LB];
		
		p.grabber = Panel::Grabber::OFF;
		p.intake = Panel::Intake::OFF;
		switch(pov_section(d.pov)){
			case POV_section::CENTER:
				break;
			case POV_section::UP:
				p.intake = Panel::Intake::OUT;
				break;
			case POV_section::UP_LEFT:	
				p.grabber = Panel::Grabber::CLOSE;
				p.intake = Panel::Intake::OUT;
				break;
			case POV_section::LEFT:
				p.grabber = Panel::Grabber::CLOSE;
				break;
			case POV_section::DOWN_LEFT:
				p.grabber = Panel::Grabber::CLOSE;
				p.intake = Panel::Intake::IN;
				break;
			case POV_section::DOWN:
				p.intake = Panel::Intake::IN;
				break;
			case POV_section::DOWN_RIGHT:
				p.grabber = Panel::Grabber::OPEN;
				p.intake = Panel::Intake::IN;
				break;
			case POV_section::RIGHT:
				p.grabber = Panel::Grabber::OPEN;
				break;
			case POV_section::UP_RIGHT:
				p.grabber = Panel::Grabber::OPEN;
				p.intake = Panel::Intake::OUT;
				break;
			default:
				assert(0);
		}
	}

	p.collection_end_height = Panel::Collection_end_height::EXCHANGE;
	switch(joystick_section(d.axis[Gamepad_axis::RIGHTX],d.axis[Gamepad_axis::RIGHTY])){
		case Joystick_section::UP:
			p.collection_end_height = Panel::Collection_end_height::SCALE;
			break;
		case Joystick_section::DOWN:
			p.collection_end_height = Panel::Collection_end_height::SWITCH;
			break;
		default:
			break;
	}

	p.lifter = Panel::Lifter::OFF;
	switch(joystick_section(d.axis[Gamepad_axis::LEFTX], d.axis[Gamepad_axis::LEFTY])){
		case Joystick_section::UP:
			p.lifter = Panel::Lifter::UP;
			break;
		case Joystick_section::DOWN:
			p.lifter = Panel::Lifter::DOWN;
			break;
		default:
			break;
	}
	
	return p;
}

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

Panel rand(Panel*){
	return interpret_oi(driver_station_input_rand());
}

#ifdef PANEL_TEST
int main(){
	Panel p;
	for(unsigned i=0;i<50;i++){
		interpret_oi(driver_station_input_rand());
	}
	cout<<p<<"\n";
	return 0;
}
#endif
