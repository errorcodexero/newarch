#include "lights.h"
#include <stdlib.h>
#include <cmath>

using namespace std;

#define BLINKY_LIGHT_INFO_ADDRESS 5

struct OI_light_addresses{
	static const unsigned COLLECTOR_OPEN = 0;
	static const unsigned COLLECTOR_CLOSED = 1;
	static const unsigned HAS_CUBE = 2;
	static const unsigned WINGS_DEPLOYED = 3; 
	static const unsigned ENABLED = 4;
	
	static const unsigned CUBE_COLLECTED_SIGNAL = 4;
	
	/*
	static const unsigned LIFTER_STATUS_BINARY_A = 5;
	static const unsigned LIFTER_STATUS_BINARY_B = 6;
	static const unsigned LIFTER_STATUS_BINARY_C = 7;
	*/
};

Lights::Goal::Goal(
	Camera_light a,
	bool c,
	unsigned h,
	double l,
	double r,
	bool cube,
	bool open,
	bool closed,
	bool wings,
	Lights::Goal::Lifter_status lifter
):
	camera_light(a),
	climbing(c),
	lifter_height(h),
	drive_left(l),
	drive_right(r),
	has_cube(cube),
	collector_open(open),
	collector_closed(closed),
	wings_deployed(wings),
	lifter_status(lifter)
{}

Lights::Goal::Goal():
	Lights::Goal(
		Camera_light::OFF,
		false,
		0,
		0.0,
		0.0,
		false,
		false,
		false,
		false,
		Lights::Goal::Lifter_status::UNKNOWN
	)
{}

Lights::Output::Output(
	bool c,
	vector<uint8_t> b,
	std::array<bool,Lights::Output::LIFTER_STATUS_BINARY_LEN> lifter,
	bool open,
	bool closed,
	bool cube,
	bool wings,
	bool en
):
	camera_light(c),
	blinky_light_info(b),
	collector_open(open),
	collector_closed(closed),
	has_cube(cube),
	wings_deployed(wings),
	enabled(en)
{}
Lights::Output::Output():
	Output(
		false,
		{},
		{0,0,0},
		false,
		false,
		false,
		false,
		false
	)
{}

Lights::Status_detail::Status_detail(
	unsigned h,
	bool c,
	bool m,
	bool e,
	Alliance a,
	Time t
):
	lifter_height(h),
	climbing(c),
	autonomous(m),
	enabled(e),
	alliance(a),
	now(t)
{}

Lights::Status_detail::Status_detail():
	Status_detail(
		0,
		false,
		false,
		false,
		Alliance::INVALID,
		0
	)
{}

Lights::Input::Input(bool a,bool e,Alliance al):autonomous(a),enabled(e),alliance(al){}
Lights::Input::Input():Input(false,false,Alliance::INVALID){}

ostream& operator<<(ostream& o, Lights::Goal::Lifter_status a){
	#define X(STATUS) if(a==Lights::Goal::Lifter_status::STATUS) return o<<""#STATUS;
	LIFTER_STATUSES_FOR_LIGHTS
	#undef X
	nyi
}

ostream& operator<<(ostream& o, Lights::Camera_light a){
	#define X(name) if(a==Lights::Camera_light::name)return o<<""#name;
	X(OFF) X(ON)
	#undef X
	nyi
}

ostream& operator<<(ostream& o, Lights::Status_detail a){
	o<<"(";
	o<<" lifter_height:"<<a.lifter_height;
	o<<" climbing:"<<a.climbing;
	o<<" autonomous:"<<a.autonomous;
	o<<" enabled:"<<a.enabled;
	o<<" alliance:"<<a.alliance;
	o<<" now:"<<a.now;
	o<<")";
	return o;
}

ostream& operator<<(ostream& o, Lights::Output a){
	o<<"(";
	o<<"camera_light:"<<a.camera_light;
	o<<" blinky_light_info:"<<a.blinky_light_info;
	o<<" lifter_status:"<<a.lifter_status;
	o<<" collector_open:"<<a.collector_open;
	o<<" collector_closed:"<<a.collector_closed;
	o<<" has_cube:"<<a.has_cube;
	o<<" wings_deployed:"<<a.wings_deployed;
	o<<" enabled:"<<a.enabled;
	o<<")";
	return o;
}

ostream& operator<<(ostream& o, Lights::Goal a){
	o<<"(";
	o<<"camera_light:"<<a.camera_light;
	o<<" climbing:"<<a.climbing;
	o<<" lifter_height:"<<a.lifter_height;
	o<<" drive_left:"<<a.drive_left;
	o<<" drive_right"<<a.drive_right;
	o<<" has_cube:"<<a.has_cube;
	o<<" collector_open:"<<a.collector_open;
	o<<" collector_closed:"<<a.collector_closed;
	o<<" wings_deployed:"<<a.wings_deployed;
	o<<" lifter_status:"<<a.lifter_status;
	return o<<")";
}

ostream& operator<<(ostream& o, Lights::Input a){
	o<<"(";
	o<<"autonomous:"<<a.autonomous;
	o<<" enabled:"<<a.enabled;
	o<<" alliance:"<<a.alliance;
	return o<<")";
}

ostream& operator<<(ostream& o, Lights){ return o<<"Lights()";}

#define CMP(ELEM) \
	if(a.ELEM < b.ELEM) return true; \
	if(b.ELEM < a.ELEM) return false;

bool operator==(Lights::Status_detail a,Lights::Status_detail b){
	return a.lifter_height == b.lifter_height && a.climbing == b.climbing && a.autonomous == b.autonomous && a.enabled == b.enabled && a.alliance == b.alliance && a.now == b.now;
}

bool operator!=(Lights::Status_detail a,Lights::Status_detail b){
	return !(a==b);
}

bool operator<(Lights::Status_detail a,Lights::Status_detail b){
	CMP(lifter_height)
	CMP(climbing)
	CMP(autonomous)
	CMP(alliance)
	CMP(enabled)
	CMP(now)
	return false;
}

bool operator==(Lights::Output a,Lights::Output b){
	return a.camera_light == b.camera_light && 
		a.blinky_light_info == b.blinky_light_info && 
		a.lifter_status == b.lifter_status && 
		a.collector_open == b.collector_open &&
		a.has_cube == b.has_cube &&
		a.wings_deployed == b.wings_deployed &&
		a.enabled == b.enabled;
}

bool operator!=(Lights::Output a,Lights::Output b){
	return !(a==b);
}

bool operator<(Lights::Output a,Lights::Output b){
	CMP(camera_light)
	CMP(blinky_light_info)
	return false;
}

bool operator==(Lights::Goal a,Lights::Goal b){
	return a.camera_light == b.camera_light && 
		a.climbing == b.climbing && 
		a.lifter_height == b.lifter_height && 
		a.drive_left == b.drive_left && 
		a.drive_right == b.drive_right && 
		a.has_cube == b.has_cube && 
		a.collector_open == b.collector_open &&
		a.collector_closed == b.collector_closed && 
		a.wings_deployed == b.wings_deployed &&
		a.lifter_status == b.lifter_status;
}

bool operator<(Lights::Goal a,Lights::Goal b){
	CMP(camera_light)
	CMP(climbing)
	CMP(lifter_height)
	CMP(drive_left)
	CMP(drive_right)
	CMP(has_cube)
	CMP(collector_open)
	CMP(collector_closed)
	CMP(wings_deployed)
	CMP(lifter_status)
	return 0;
}

bool operator!=(Lights::Goal a, Lights::Goal b){
	return !(a==b);
}

bool operator==(Lights::Input a,Lights::Input b){
	return a.autonomous == b.autonomous && a.enabled == b.enabled && a.alliance == b.alliance;
}

bool operator!=(Lights::Input a,Lights::Input b){
	return !(a==b);
}

bool operator<(Lights::Input a,Lights::Input b){
	CMP(autonomous)
	CMP(alliance)
	CMP(enabled)
	return false;
}

bool operator==(Lights::Input_reader, Lights::Input_reader){
	return true;
}

bool operator==(Lights::Output_applicator, Lights::Output_applicator){
	return 1;
}

bool operator==(Lights::Estimator, Lights::Estimator){
	return true;
}

bool operator!=(Lights::Estimator a, Lights::Estimator b){
	return !(a==b);
}

bool operator==(Lights a, Lights b){
	return (a.input_reader==b.input_reader && a.estimator==b.estimator && a.output_applicator==b.output_applicator);
}

bool operator!=(Lights a, Lights b){
	return !(a==b);
}

#undef CMP

Lights::Input Lights::Input_reader::operator()(Robot_inputs r)const{
	return {r.robot_mode.autonomous,r.robot_mode.enabled,r.ds_info.alliance};
}

Robot_inputs Lights::Input_reader::operator()(Robot_inputs r, Lights::Input in)const{
	r.robot_mode.autonomous = in.autonomous;
	r.ds_info.alliance = in.alliance;
	r.robot_mode.enabled = in.enabled;
	return r;
}

Lights::Output Lights::Output_applicator::operator()(Robot_outputs r)const{
	Output out;
	out.blinky_light_info = r.i2c.data;
	
	out.has_cube = r.driver_station.digital[OI_light_addresses::CUBE_COLLECTED_SIGNAL];
	
	/*	
	out.collector_open = r.driver_station.digital[OI_light_addresses::COLLECTOR_OPEN];
	out.collector_closed = r.driver_station.digital[OI_light_addresses::COLLECTOR_CLOSED];
	out.has_cube = r.driver_station.digital[OI_light_addresses::HAS_CUBE];
	out.wings_deployed = r.driver_station.digital[OI_light_addresses::WINGS_DEPLOYED];
 	out.enabled = r.driver_station.digital[OI_light_addresses::ENABLED];
	
	out.lifter_status[0] = r.driver_station.digital[OI_light_addresses::LIFTER_STATUS_BINARY_A];
	out.lifter_status[1] = r.driver_station.digital[OI_light_addresses::LIFTER_STATUS_BINARY_B];
	out.lifter_status[2] = r.driver_station.digital[OI_light_addresses::LIFTER_STATUS_BINARY_C];
	*/
	return out;
}

Robot_outputs Lights::Output_applicator::operator()(Robot_outputs r, Lights::Output out)const{
	r.i2c.data = out.blinky_light_info;
	
	r.driver_station.digital[OI_light_addresses::CUBE_COLLECTED_SIGNAL] = out.has_cube;
	
	/*
	r.driver_station.digital[OI_light_addresses::COLLECTOR_OPEN] = out.collector_open;
	r.driver_station.digital[OI_light_addresses::COLLECTOR_CLOSED] = out.collector_closed;
	r.driver_station.digital[OI_light_addresses::HAS_CUBE] = out.has_cube;
	r.driver_station.digital[OI_light_addresses::WINGS_DEPLOYED] = out.wings_deployed;
	r.driver_station.digital[OI_light_addresses::ENABLED] = out.enabled;
	
	r.driver_station.digital[OI_light_addresses::LIFTER_STATUS_BINARY_A] = out.lifter_status[0];
	r.driver_station.digital[OI_light_addresses::LIFTER_STATUS_BINARY_B] = out.lifter_status[1];
	r.driver_station.digital[OI_light_addresses::LIFTER_STATUS_BINARY_C] = out.lifter_status[2];
	*/
	return r;
}

Lights::Estimator::Estimator(){}

void Lights::Estimator::update(Time now,Lights::Input in,Lights::Output){
	last.now = now;
	last.alliance = in.alliance;
	last.enabled = in.enabled;
	last.autonomous = in.autonomous;
}

Lights::Status_detail Lights::Estimator::get()const{
	return last;
}

set<Lights::Input> examples(Lights::Input*){ 
	/*
	set<Lights::Input> s;
	vector<bool> bools = {false,true};
	vector<Alliance> alliances = {Alliance::RED,Alliance::BLUE,Alliance::INVALID};
	for(bool a: bools){
		for(bool e: bools){
			for(Alliance al: alliances){
				s.insert({a,e,al});
			}
		}
	}
	return s;
	*/
	return {{}};
}

set<Lights::Status_detail> examples(Lights::Status_detail*){ 
	/*
	set<Lights::Status_detail> s;
	vector<bool> bools = {false,true};
	vector<Alliance> alliances = {Alliance::RED,Alliance::BLUE,Alliance::INVALID};
	for(bool c: bools){
		for(bool m: bools){
			for(bool e: bools){
				for(Alliance a: alliances){
					s.insert({0,c,m,e,a,0.0});
				}
			}
		}
	}
	return s;
	*/
	return {{}};
}

set<Lights::Output> examples(Lights::Output*){ 
	/*
	set<Lights::Output> s;
	vector<bool> bools = {false,true};
	set<Lights::Status_detail> statuses = examples((Lights::Status_detail*)nullptr);
	set<Lights::Goal> goals = examples((Lights::Goal*)nullptr);
	for(bool c: bools){
		for(Lights::Status_detail status: statuses){
			for(Lights::Goal goal: goals){
				s.insert(
					{
						c,
						vector<uint8_t>{
							goal.climbing, 
							status.autonomous, 
							status.enabled, 
							(uint8_t)static_cast<int>(status.alliance),
						       	(uint8_t)goal.lifter_height,
							(uint8_t)(goal.drive_left * 100),
							(uint8_t)(goal.drive_right * 100),
							(uint8_t)goal.has_cube
						},
						goal.lifter_status,
						goal.collector_open,
				});
			}
		}
	}
	return s;
	*/
	return {{}};
}

set<Lights::Goal> examples(Lights::Goal*){ 
	/*
	return {
		Lights::Goal{Lights::Camera_light::ON,false,0,0.0,0.0,false},
		Lights::Goal{Lights::Camera_light::ON,true,0,0.0,0.0,false},
		Lights::Goal{Lights::Camera_light::OFF,false,0,0.0,0.0,false},
		Lights::Goal{Lights::Camera_light::OFF,true,0,0.0,0.0,false},
		Lights::Goal{Lights::Camera_light::ON,false,0,0.0,0.0,true},
		Lights::Goal{Lights::Camera_light::ON,true,0,0.0,0.0,true},
		Lights::Goal{Lights::Camera_light::OFF,false,0,0.0,0.0,true},
		Lights::Goal{Lights::Camera_light::OFF,true,0,0.0,0.0,true}
	};
	*/
	return {{}};
}

Lights::Output control(Lights::Status status, Lights::Goal goal){
	Lights::Output out;
	out.camera_light = goal.camera_light == Lights::Camera_light::ON;
	out.blinky_light_info = {
		goal.climbing,
		status.autonomous,
		status.enabled,
		(uint8_t)static_cast<int>(status.alliance),
		(uint8_t)(goal.lifter_height / 3.0), //divide by three so we send fewer descrete values
		(uint8_t)(goal.drive_left * 100),
		(uint8_t)(goal.drive_right * 100),
		(uint8_t)goal.has_cube
	}; //encode_robot_status(status);
	out.lifter_status = [&]{
		switch(goal.lifter_status){
			case Lights::Goal::Lifter_status::UNKNOWN:
				return std::array<bool,Lights::Output::LIFTER_STATUS_BINARY_LEN>{0,0,0};//0
			case Lights::Goal::Lifter_status::FLOOR:
				return std::array<bool,Lights::Output::LIFTER_STATUS_BINARY_LEN>{0,0,1};//1
			case Lights::Goal::Lifter_status::EXCHANGE:
				return std::array<bool,Lights::Output::LIFTER_STATUS_BINARY_LEN>{0,1,0};//2
			case Lights::Goal::Lifter_status::CLIMB:
				return std::array<bool,Lights::Output::LIFTER_STATUS_BINARY_LEN>{0,1,1};//3
			case Lights::Goal::Lifter_status::SWITCH:
				return std::array<bool,Lights::Output::LIFTER_STATUS_BINARY_LEN>{1,0,0};//4
			case Lights::Goal::Lifter_status::SCALE:
				return std::array<bool,Lights::Output::LIFTER_STATUS_BINARY_LEN>{1,0,1};//5
			default:
				nyi
		}
	}();
	out.collector_open = goal.collector_open;
	out.collector_closed = goal.collector_closed;
	out.has_cube = goal.has_cube;
	out.wings_deployed = goal.wings_deployed;
	out.enabled = status.enabled;
	return out;
}

bool ready(Lights::Status, Lights::Goal){ return 1;}

Lights::Status status(Lights::Status_detail const& a){
	return a;
}

#ifdef LIGHTS_TEST
#include "formal.h"

int main(){
	/*
	Lights a;
	Tester_mode mode;
	mode.check_outputs_exhaustive = false;
	tester(a,mode);
	*/
}

#endif
