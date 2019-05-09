#include "shooter.h"
#include <cmath>
#include <cassert>

#define SHOOTER_WHEEL_LOC 0
#define BEAM_SENSOR_DIO 5

Shooter::Status_detail::Status_detail():speed(0){}
Shooter::Status_detail::Status_detail(double s):speed(s){}

Shooter::Estimator::Estimator():last({}),last_output({0,0,Talon_srx_output::Mode::VOLTAGE}){}

Shooter::Goal::Goal():mode(Shooter::Goal::Mode::VOLTAGE),value(0){}
Shooter::Goal::Goal(PID_values pid,Shooter::Goal::Mode m,double p):constants(pid),mode(m),value(p){}

Shooter::Output::Output():speed(0),voltage(0),mode(Talon_srx_output::Mode::VOLTAGE){}
Shooter::Output::Output(double s,double v,Talon_srx_output::Mode m):speed(s),voltage(v),mode(m){}

std::ostream& operator<<(std::ostream& o,Shooter::Goal::Mode a){
	#define X(name) if(a==Shooter::Goal::Mode::name) return o<<#name;
	SHOOTER_MODES
	#undef X
	assert(0);
}

std::ostream& operator<<(std::ostream& o,Shooter::Goal goal){
	o<<"Shooter::Goal(";
	o<<" const:"<<goal.constants;
	o<<" mode:"<<goal.mode;
	o<<" value:"<<goal.value;
	return o<<")";
}

std::ostream& operator<<(std::ostream& o,Shooter::Estimator a){ return o<<"Shooter::Estimator( last:"<<a.get()<<" last_output:"<<a.last_output<<")"; }
std::ostream& operator<<(std::ostream& o,Shooter::Input a){ return o<<"Shooter::Input( speed:"<<a.speed<<" enabled:"<<a.enabled<<")"; }
std::ostream& operator<<(std::ostream& o,Shooter::Status_detail a){ return o<<"Shooter::Status_detail( speed:"<<a.speed<<")"; }
std::ostream& operator<<(std::ostream& o,Shooter a){ return o<<"Shooter("<<a.estimator<<")"; }

std::ostream& operator<<(std::ostream& o,Shooter::Output out){
	o<<"Shooter::Output( ";
	if (out.mode==Talon_srx_output::Mode::SPEED) o<<"speed:"<<out.speed;
	else if(out.mode==Talon_srx_output::Mode::VOLTAGE) o<<"voltage:"<<out.voltage;
	return o<<" mode:"<<out.mode<<")";
}

bool operator==(Shooter::Input a,Shooter::Input b){ return a.speed==b.speed; }
bool operator!=(Shooter::Input a,Shooter::Input b){ return !(a==b); }
bool operator<(Shooter::Input a,Shooter::Input b){
	if(a.speed<b.speed) return true;
	if(b.speed<a.speed) return false;
	return a.enabled<b.enabled;
} 

bool operator<(Shooter::Status_detail a,Shooter::Status_detail b){
	return a.speed<b.speed;
}

bool operator==(Shooter::Goal a,Shooter::Goal b){
	 return a.constants==b.constants && a.mode==b.mode && a.value==b.value;
}

bool operator!=(Shooter::Goal a,Shooter::Goal b){ return !(a==b); }
bool operator<(Shooter::Goal a,Shooter::Goal b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(constants) X(mode) X(value)
	#undef X
	return 0;
}

bool operator==(Shooter::Output a,Shooter::Output b){
	return a.speed==b.speed && a.voltage==b.voltage && a.mode==b.mode;
}

bool operator!=(Shooter::Output a,Shooter::Output b){ return !(a==b); }
bool operator<(Shooter::Output a,Shooter::Output b){
	if(a.speed<b.speed) return true;
	if(b.speed<a.speed) return false;
	if(a.voltage<b.voltage) return true;
	if(b.voltage<a.voltage) return false;
	return a.mode<b.mode;
}
bool approx_equals(Shooter::Output a, Shooter::Output b){
	const double SPEED_TOLERANCE=50,VOLTAGE_TOLERANCE=.02;
	return a.mode==b.mode && fabs(a.speed-b.speed)<SPEED_TOLERANCE && fabs(a.voltage-b.voltage)<VOLTAGE_TOLERANCE;
}

bool operator==(Shooter::Status_detail a,Shooter::Status_detail b){ return (a.speed==b.speed); }
bool operator!=(Shooter::Status_detail a,Shooter::Status_detail b){ return !(a==b); }

bool operator<(Shooter::Input_reader,Shooter::Input_reader){ return false; }
bool operator==(Shooter::Input_reader,Shooter::Input_reader){ return true; }

bool operator==(Shooter::Estimator a,Shooter::Estimator b){ return a.last==b.last && a.last_output==b.last_output; }
bool operator!=(Shooter::Estimator a,Shooter::Estimator b){ return !(a==b); }

bool operator==(Shooter::Output_applicator,Shooter::Output_applicator){ return true; }

bool operator==(Shooter a,Shooter b){ return (a.input_reader==b.input_reader && a.estimator==b.estimator && a.output_applicator==b.output_applicator); }
bool operator!=(Shooter a,Shooter b){ return !(a==b); }

Shooter::Input Shooter::Input_reader::operator()(Robot_inputs const& r)const{
	return {r.talon_srx[SHOOTER_WHEEL_LOC].velocity,r.robot_mode.enabled};
}

Robot_inputs Shooter::Input_reader::operator()(Robot_inputs r,Shooter::Input in)const{
	r.talon_srx[SHOOTER_WHEEL_LOC].velocity = in.speed;
	r.robot_mode.enabled=in.enabled;
	return r;
}

Shooter::Output Shooter::Output_applicator::operator()(Robot_outputs const& r)const{
	Shooter::Output out;
	out.mode=r.talon_srx[SHOOTER_WHEEL_LOC].mode;
	out.voltage=r.talon_srx[SHOOTER_WHEEL_LOC].power_level;
	out.speed= r.talon_srx[SHOOTER_WHEEL_LOC].speed;
	return out;
}

Robot_outputs Shooter::Output_applicator::operator()(Robot_outputs r,Shooter::Output out)const{ 
	r.talon_srx[SHOOTER_WHEEL_LOC].mode=out.mode;
	r.talon_srx[SHOOTER_WHEEL_LOC].power_level = out.voltage;
	r.talon_srx[SHOOTER_WHEEL_LOC].speed = out.speed;
	return r;
}

Shooter::Status_detail Shooter::Estimator::get()const{
	return last;
}

void Shooter::Estimator::update(Time,Shooter::Input in,Shooter::Output output){
	last.speed=in.speed;
	if(output.mode==Talon_srx_output::Mode::VOLTAGE){
		last.speed=[&]{
			if(output.voltage==0.0) return 0.0;
			if(output.voltage==-.5) return CLIMB_RPM;
			if(output.voltage==-1.0) return GROUND_RPM;
			if(output.voltage==1.0) return FREE_SPIN_RPM;
			return CLIMB_RPM;
		}();
	} else if(output.mode==Talon_srx_output::Mode::SPEED){
		
	}
	last_output=output;
} 

std::set<Shooter::Input> examples(Shooter::Input*){
	return {
		{(int)GROUND_RPM,true},
		{(int)GROUND_RPM,false},
		{0,true},
		{0,false},
	}; 
}
std::set<Shooter::Goal> examples(Shooter::Goal*){
	std::set<Shooter::Goal> s;
	#define X(f) s|=Shooter::Goal{PID_values{},Shooter::Goal::Mode::SPEED,f};
	X(GROUND_RPM) X(0) X(CLIMB_RPM) X(FREE_SPIN_RPM)
	#undef X

	s|=Shooter::Goal{PID_values{},Shooter::Goal::Mode::VOLTAGE,0};
	s|=Shooter::Goal{PID_values{},Shooter::Goal::Mode::VOLTAGE,-1};
	return s;
}
std::set<Shooter::Status_detail> examples(Shooter::Status_detail*){
	return {{0},{GROUND_RPM},{CLIMB_RPM},{FREE_SPIN_RPM}};
}

std::set<Shooter::Output> examples(Shooter::Output*){
	std::set<Shooter::Output> s;
	#define X(POWER) s.insert({0,POWER,Talon_srx_output::Mode::VOLTAGE});
	X(-0.0) X(-1.0)
	#undef X
	#define X(RPM) s.insert({RPM,0,Talon_srx_output::Mode::SPEED});
	X(GROUND_RPM) X(0.0) X(CLIMB_RPM) X(FREE_SPIN_RPM)
	#undef X
	return s;
}

Shooter::Output control(Shooter::Status_detail, Shooter::Goal goal){
	Shooter::Output out;
	out.constants=goal.constants;
	switch(goal.mode) {
		case Shooter::Goal::Mode::SPEED:
			out.mode=Talon_srx_output::Mode::SPEED;
			out.speed=goal.value;
			out.voltage=0;
			break;
		case Shooter::Goal::Mode::VOLTAGE:
			out.mode=Talon_srx_output::Mode::VOLTAGE;
			out.voltage=goal.value;
			break;
		default: assert(0);
	}
	return out;
}

Shooter::Status status(Shooter::Status_detail a){
	return a;
}

bool approx_speed(double curr, double target) {
	const double PERCENT_TOLERANCE=.02;//From testing, this looks pretty good
	return (std::fabs(curr - target) < std::fabs(target)*PERCENT_TOLERANCE);
}

bool ready(Shooter::Status status,Shooter::Goal goal){
	switch(goal.mode){
		case Shooter::Goal::Mode::VOLTAGE: return true;
		case Shooter::Goal::Mode::SPEED: return approx_speed(status.speed,goal.value);
		default: assert(0);
	}
}

#ifdef SHOOTER_TEST
#include "formal.h"

int main(){
	Shooter a;
	tester(a);
}

#endif
