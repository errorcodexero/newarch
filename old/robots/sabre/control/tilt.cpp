#include "tilt.h"
#include <stdlib.h>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream> 
#include <vector> 

#define POWER 1.0//negative goes up, positive goes down

#define TOP_VOLTAGE 1.71
#define BOTTOM_VOLTAGE 4
#define VOLTS_PER_DEGREE ((BOTTOM_VOLTAGE-TOP_VOLTAGE)/120) // (in volts/degree) //Assumed for now

#define ANGLE_TOLERANCE 5//in degrees

#define TILT_PDB_LOC 8 //pdb # 
#define TILT_POT_LOC 0 //pot analog #
#define TILT_LIM_LOC 9 //limit switch dio #
#define TILT_ADDRESS 4 //pwm #

#define nyi { std::cout<<"\nnyi "<<__LINE__<<"\n"; exit(44); }

float volts_to_degrees(float f){
	return (f-TOP_VOLTAGE)/VOLTS_PER_DEGREE;
}

float degrees_to_volts(float f){
	return f*VOLTS_PER_DEGREE+TOP_VOLTAGE;
}

double power_to_keep_up(double angle) {
	const double LENGTH = 9;//in inches
	const double WEIGHT = 9;//in pounds
	const double MOTOR_MAX = 1750;//in inch/pounds
	return -(sin(angle * PI / 180) * (LENGTH * WEIGHT / MOTOR_MAX));
}

Tilt::Status_detail::Status_detail():top(0),angle(0){}

Tilt::Status_detail::Status_detail(bool a,double b):top(a),angle(b){}

Tilt::Goal::Goal():mode_(Tilt::Goal::Mode::STOP),angle_min(0),angle_target(0),angle_max(0){}

Robot_inputs Tilt::Input_reader::operator()(Robot_inputs r,Tilt::Input in)const{
	r.analog[TILT_POT_LOC]=degrees_to_volts(in.angle);
	r.digital_io.in[TILT_LIM_LOC]=(in.top) ? Digital_in::_0 : Digital_in::_1;
	return r;
}

Tilt::Input Tilt::Input_reader::operator()(Robot_inputs const& r)const{
	return Tilt::Input{
		r.digital_io.in[TILT_LIM_LOC]==Digital_in::_0,
		volts_to_degrees(r.analog[TILT_POT_LOC])
	};
}

Tilt::Output Tilt::Output_applicator::operator()(Robot_outputs r)const{
	return r.pwm[TILT_ADDRESS];
}

Robot_outputs Tilt::Output_applicator::operator()(Robot_outputs r, Tilt::Output out)const{
	r.pwm[TILT_ADDRESS]=out;
	return r;
}

Tilt::Goal::Mode Tilt::Goal::mode()const{
	return mode_;
}

Tilt::Estimator::Estimator():last(0,0){}

std::array<double,3> Tilt::Goal::angle()const{
	assert(mode_==Tilt::Goal::Mode::GO_TO_ANGLE);
	return {angle_min,angle_target,angle_max};
}

Tilt::Goal Tilt::Goal::up(){
	Tilt::Goal a;
	a.mode_=Tilt::Goal::Mode::UP;
	return a;
}

Tilt::Goal Tilt::Goal::go_to_angle(std::array<double,3> angles){
	Tilt::Goal a;
	a.mode_=Tilt::Goal::Mode::GO_TO_ANGLE;
	a.angle_min=angles[0];
	a.angle_target=angles[1];
	a.angle_max=angles[2];
	return a;
}

Tilt::Goal Tilt::Goal::down(){
	Tilt::Goal a;
	a.mode_=Tilt::Goal::Mode::DOWN;
	return a;
}

Tilt::Goal Tilt::Goal::stop(){
	Tilt::Goal a;
	a.mode_=Tilt::Goal::Mode::STOP;
	return a;
}

std::ostream& operator<<(std::ostream& o, Tilt::Goal::Mode a){
	#define X(name) if(a==Tilt::Goal::Mode::name) return o<<"Tilt::Goal("#name")";
	TILT_GOAL_MODES
	#undef X
	nyi
}

std::ostream& operator<<(std::ostream& o, Tilt::Status_detail a){ 
	o<<"Tilt::Status_detail(";
	o<<"top:"<<a.top<<" angle:"<<a.angle;
	return o<<")";
}

std::ostream& operator<<(std::ostream& o, Tilt::Goal a){ 
	o<<"Tilt::Goal(";
	o<<" mode:"<<a.mode();
	if(a.mode()==Tilt::Goal::Mode::GO_TO_ANGLE)o<<"("<<a.angle()<<")";
	return o<<")";
}

std::ostream& operator<<(std::ostream& o, Tilt::Output_applicator){ return o<<"Tilt::Output_applicator()";} 
std::ostream& operator<<(std::ostream& o, Tilt::Input_reader){ return o<<"Tilt::Input_reader()";}
std::ostream& operator<<(std::ostream& o, Tilt::Estimator a){ return o<<"Tilt::Estimator( last:"<<a.get()<<")";}

std::ostream& operator<<(std::ostream& o, Tilt a){
	o<<"Tilt(";
	o<<" "<<a.output_applicator;
	o<<" "<<a.input_reader;
	o<<" "<<a.estimator;
	return o<<")";
}

#define CMP(name) if(a.name<b.name) return 1; if(b.name<a.name) return 0;

bool operator<(Tilt::Status_detail a, Tilt::Status_detail b){
	CMP(top)
	CMP(angle)
	return 0;
}

bool operator==(Tilt::Status_detail a,Tilt::Status_detail b){
	return a.top==b.top && a.angle==b.angle;
}

bool operator!=(Tilt::Status_detail a,Tilt::Status_detail b){ return !(a==b); }

bool operator<(Tilt::Goal a,Tilt::Goal b){
	CMP(mode())
	switch(a.mode()){
		default:
			std::cout<<a.mode();
			nyi
	}
}

bool operator==(Tilt::Goal a, Tilt::Goal b){ 
	return a.mode()==b.mode() && ((a.mode()==Tilt::Goal::Mode::GO_TO_ANGLE) ? a.angle()==b.angle() : true );
}

bool operator!=(Tilt::Goal a, Tilt::Goal b){ return !(a==b); }

bool operator==(Tilt::Output_applicator,Tilt::Output_applicator){ return true; }

bool operator==(Tilt::Input_reader,Tilt::Input_reader){ return true; }

bool operator==(Tilt::Estimator a,Tilt::Estimator b){ return a.last==b.last; }
bool operator!=(Tilt::Estimator a,Tilt::Estimator b){ return !(a==b); }

bool operator==(Tilt a, Tilt b){ return (a.output_applicator==b.output_applicator && a.input_reader==b.input_reader && a.estimator==b.estimator); }
bool operator!=(Tilt a, Tilt b){ return !(a==b); }

std::set<Tilt::Goal> examples(Tilt::Goal*){
	return {
		Tilt::Goal::down(),
		Tilt::Goal::stop(),
		Tilt::Goal::go_to_angle(make_tolerances(ANGLE_TOLERANCE)),
		Tilt::Goal::up()
	};
}

std::set<Tilt::Status_detail> examples(Tilt::Status_detail*){
	return {
		Tilt::Status_detail{1,0},
		Tilt::Status_detail{0,90},
		Tilt::Status_detail{0,120},
	};
}

std::set<Tilt::Output> examples(Tilt::Output*){
	return {0,90,120};
}

Tilt::Output control(Tilt::Status_detail status, Tilt::Goal goal){
	//const float MIN=.05;//minimum percent power
	double power=POWER*.75;
	switch(goal.mode()){
		case Tilt::Goal::Mode::DOWN:
			//if(status.angle>=volts_to_degrees(BOTTOM_VOLTAGE)) return 0;//If enabled, then the collector won't go below the bottom
			return power;
		case Tilt::Goal::Mode::UP:
			if(status.top) return 0;
			return -power;
		case Tilt::Goal::Mode::STOP:
			return 0;
		case Tilt::Goal::Mode::GO_TO_ANGLE:
			{
				double error=goal.angle()[1]-status.angle;
				double x=power_to_keep_up(status.angle) + error*.04;
				if(x<0 && status.top) return 0;
				return clip(x);
			}
		default:
			assert(0);
	}
}

Tilt::Status status(Tilt::Status_detail a){
	return a.angle;
}

bool ready(Tilt::Status status, Tilt::Goal goal){
	switch(goal.mode()){
		case Tilt::Goal::Mode::UP: return status<=ANGLE_TOLERANCE;
		case Tilt::Goal::Mode::DOWN: return status>=volts_to_degrees(BOTTOM_VOLTAGE);
		case Tilt::Goal::Mode::GO_TO_ANGLE: return (status>=goal.angle()[0] && status<=goal.angle()[2]);
		case Tilt::Goal::Mode::STOP: return true;
		default: assert(0);
	}
}

Tilt::Status_detail Tilt::Estimator::get()const {
	return last;
}

void Tilt::Estimator::update(Time, Tilt::Input in, Tilt::Output) {
	last=in;
}

std::array<double,3> make_tolerances(double d){
	return {d-ANGLE_TOLERANCE,d,d+ANGLE_TOLERANCE};
}

#ifdef TILT_TEST
#include "formal.h"

bool approx_eq(double a,double b){
	return fabs(a-b)<.001;
}

bool approx_eq(Tilt::Status_detail a,Tilt::Status_detail b){
	return a.top==b.top && approx_eq(a.angle,b.angle);
}

int main(){
	Tilt a;
	Tester_mode t;
	t.check_outputs_exhaustive = 0;
	t.input_exact=0;
	tester(a, t);

	for(double d=-90;d<=90;d+=30){
		std::cout<<d<<"\t"<<volts_to_degrees(degrees_to_volts(d))<<"\n";
	}
}

#endif
