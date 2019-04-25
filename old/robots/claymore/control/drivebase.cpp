#include "drivebase.h"
#include <iostream>
#include <math.h>
#include "util.h"
#include "robot_constants.h"
#include "motion_profile.h"
#include "message_logger.h"
#include "point.h"
#include <fstream>

using namespace std;

//these are all off by one
#ifdef THEREMIN
#define L_MOTOR_LOC_1 0
#define L_MOTOR_LOC_2 1
#define R_MOTOR_LOC_1 2
#define R_MOTOR_LOC_2 3
#endif

#ifdef CLAYMORE
#define L_MOTOR_LOC_1 0
#define L_MOTOR_LOC_2 1
#define L_MOTOR_LOC_3 2
#define R_MOTOR_LOC_1 3
#define R_MOTOR_LOC_2 4
#define R_MOTOR_LOC_3 5

#define SHIFTER_SOLENOID 0
#endif


DrivebaseController Drivebase::drivebase_controller;

unsigned pdb_location(Drivebase::Motor m){
	#define X(NAME,INDEX) if(m==Drivebase::NAME) return INDEX;
	//WILL NEED CORRECT VALUES
#ifdef THEREMIN
	X(LEFT1,0)
	X(LEFT2,1)
	X(RIGHT1,2)
	X(RIGHT2,13)
#endif

#ifdef CLAYMORE
	X(LEFT1,13)
	X(LEFT2,14)
	X(LEFT3,15)
	X(RIGHT1,0)
	X(RIGHT2,1)
	X(RIGHT3,2)
#endif
	#undef X
	assert(0);
	//assert(m>=0 && m<Drivebase::MOTORS);
}

int encoderconv(Maybe_inline<Encoder_output> encoder){
	if(encoder) return *encoder;
	return 10000;
}

const unsigned int TICKS_PER_REVOLUTION=200;
const double ENCODER_TO_WHEEL_RATIO = 7.5;
const double WHEEL_CIRCUMFERENCE=Robot_constants::DRIVE_WHEEL_DIAMETER*PI;//inches
const double INCHES_PER_TICK=WHEEL_CIRCUMFERENCE/(double)TICKS_PER_REVOLUTION / ENCODER_TO_WHEEL_RATIO ;

double ticks_to_inches(const int ticks){
	return ticks*INCHES_PER_TICK;
}

Drivebase::Distances ticks_to_inches(const Drivebase::Encoder_ticks ticks){
	Drivebase::Distances d = {0.0,0.0};
	#define X(TYPE,SIDE) d.SIDE = ticks_to_inches(ticks.SIDE);
	DISTANCES_ITEMS(X)
	#undef X
	return d;
}

int inches_to_ticks(const double inches){
	return (int)(inches/(INCHES_PER_TICK));
}

#define L_ENCODER_PORTS 0,1
#define R_ENCODER_PORTS 2,3
#define L_ENCODER_LOC 0
#define R_ENCODER_LOC 1

Robot_inputs Drivebase::Input_reader::operator()(Robot_inputs all,Input in)const{
	all.digital_io.encoder[L_ENCODER_LOC] = -inches_to_ticks(in.distances.l);
	all.digital_io.encoder[R_ENCODER_LOC] = inches_to_ticks(in.distances.r);
	all.navx.angle = in.angle;
	return all;
}

Drivebase::Input Drivebase::Input_reader::operator()(Robot_inputs const& in)const{	
	Drivebase::Input input = Drivebase::Input(
		{
			-ticks_to_inches(encoderconv(in.digital_io.encoder[L_ENCODER_LOC])),
			ticks_to_inches(encoderconv(in.digital_io.encoder[R_ENCODER_LOC]))
		},
		in.navx.angle
	);
	
	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DRIVEBASE_RAW_DATA);
	logger << "Left ticks: " << encoderconv(in.digital_io.encoder[L_ENCODER_LOC]);
	logger << " Right ticks: " << encoderconv(in.digital_io.encoder[R_ENCODER_LOC]);
	logger << "\nLeft Distance: " << input.distances.l;
	logger << " Right Distance: " << input.distances.r;
	logger << "\nAngle: " << input.angle;
	logger.endMessage();

	return input;
}

Drivebase::Encoder_ticks operator+(Drivebase::Encoder_ticks const& a,Drivebase::Encoder_ticks const& b){
	Drivebase::Encoder_ticks sum = {
		#define X(TYPE,SIDE) 0,
		ENCODER_TICKS(X)
		#undef X
	};
	#define X(TYPE,SIDE) sum.SIDE = a.SIDE + b.SIDE;
	ENCODER_TICKS(X)
	#undef X
	return sum;
}


Drivebase::Distances operator+(Drivebase::Distances const& a,Drivebase::Distances const& b){
	Drivebase::Distances sum = {
		#define X(TYPE,SIDE) 0,
		DISTANCES_ITEMS(X)
		#undef X
	};
	#define X(TYPE,SIDE) sum.SIDE = a.SIDE + b.SIDE;
	DISTANCES_ITEMS(X)
	#undef X
	return sum;
}

Drivebase::Distances& operator+=(Drivebase::Distances& a,Drivebase::Distances const& b){
	#define X(TYPE,SIDE) a.SIDE += b.SIDE;
	DISTANCES_ITEMS(X)
	#undef X
	return a;
}

Drivebase::Encoder_ticks operator-(Drivebase::Encoder_ticks const& a){
	Drivebase::Encoder_ticks opposite = {
		#define X(TYPE,SIDE) -a.SIDE,
		ENCODER_TICKS(X)
		#undef X
	};
	return opposite;
}

Drivebase::Encoder_ticks operator-(Drivebase::Encoder_ticks const& a,Drivebase::Encoder_ticks const& b){
	return a + (-b);
}

bool operator==(Drivebase::Distances const& a,Drivebase::Distances const& b){
	#define X(TYPE,SIDE) if(a.SIDE != b.SIDE) return false;
	DISTANCES_ITEMS(X)
	#undef X
	return true;
}

bool operator!=(Drivebase::Distances const& a,Drivebase::Distances const& b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Drivebase::Distances const& a){
	o<<"Distances(";
	#define X(TYPE,SIDE) o<</*""#SIDE<<":"<<*/a.SIDE<<" ";
	DISTANCES_ITEMS(X)
	#undef X
	return o<<")";
}

bool operator<(Drivebase::Distances const& a,Drivebase::Distances const& b){
	#define X(TYPE,SIDE) if(a.SIDE >= b.SIDE) return false;
	DISTANCES_ITEMS(X)
	#undef X
	return true;
}

Drivebase::Distances fabs(Drivebase::Distances const& a){
	Drivebase::Distances pos = {
		#define X(TYPE,SIDE) fabs(a.SIDE),
		DISTANCES_ITEMS(X)
		#undef X
	};
	return pos;
}


Drivebase::Distances operator-(Drivebase::Distances const& a){
	Drivebase::Distances opposite = {
		#define X(TYPE,SIDE) -a.SIDE,
		DISTANCES_ITEMS(X)
		#undef X
	};
	return opposite;
}


Drivebase::Distances operator-(Drivebase::Distances const& a,Drivebase::Distances const& b){
	return a + (-b);
}

Drivebase::Encoder_ticks::Encoder_ticks():l(0),r(0){}
Drivebase::Distances::Distances():l(0),r(0){}
Drivebase::Distances::Distances(double d):l(d),r(d){}

IMPL_STRUCT(Drivebase::Encoder_ticks::Encoder_ticks,ENCODER_TICKS)
IMPL_STRUCT(Drivebase::Speeds::Speeds,SPEEDS_ITEMS)
IMPL_STRUCT(Drivebase::Distances::Distances,DISTANCES_ITEMS)

IMPL_STRUCT(Drivebase::Status::Status,DRIVEBASE_STATUS)
IMPL_STRUCT(Drivebase::Input::Input,DRIVEBASE_INPUT)
IMPL_STRUCT(Drivebase::Output::Output,DRIVEBASE_OUTPUT)

CMP_OPS(Drivebase::Encoder_ticks,ENCODER_TICKS)
CMP_OPS(Drivebase::Speeds,SPEEDS_ITEMS)

CMP_OPS(Drivebase::Input,DRIVEBASE_INPUT)

CMP_OPS(Drivebase::Status,DRIVEBASE_STATUS)

set<Drivebase::Status> examples(Drivebase::Status*){
	return {Drivebase::Status{
		{0.0,0.0},
		{0,0},
		0.0,
		false,
		0.0,
		0.0
	}};
}

set<Drivebase::Goal> examples(Drivebase::Goal*){
	return {
		Drivebase::Goal::rotate(),
		Drivebase::Goal::drive_straight(),
		Drivebase::Goal::absolute(0,0),
		Drivebase::Goal::absolute(1,1)
	};
}

std::ostream& operator<<(std::ostream& o, Drivebase::Goal::Mode a){
	#define X(name) if(a==Drivebase::Goal::Mode::name) return o<<""#name;
	DRIVEBASE_GOAL_MODES
	#undef X
	nyi
}

std::ostream& operator<<(std::ostream& o, Drivebase::Goal::Gear a) {
	switch(a) {
	case Drivebase::Goal::Gear::LOW:
		return o << "LOW";
	case Drivebase::Goal::Gear::HIGH:
		return o << "HIGH";
	case Drivebase::Goal::Gear::AUTO:
		return o << "AUTO";
	default:
		assert(false);
	}
}

Drivebase::Goal::Goal():mode_(Drivebase::Goal::Mode::ABSOLUTE),left_(0),right_(0),gear_(Gear::AUTO),brake_(true){}

Drivebase::Goal::Mode Drivebase::Goal::mode()const{
	return mode_;
}

double Drivebase::Goal::right()const{
	assert(mode_ == Drivebase::Goal::Mode::ABSOLUTE);
	return right_;
}

double Drivebase::Goal::left()const{
	assert(mode_ == Drivebase::Goal::Mode::ABSOLUTE);
	return left_;
}

Drivebase::Goal::Gear Drivebase::Goal::gear()const{
	assert(mode_ == Drivebase::Goal::Mode::ABSOLUTE);
	return gear_;
}

bool Drivebase::Goal::brake()const{
	return brake_;
}

Drivebase::Goal Drivebase::Goal::absolute(double left, double right, Gear gear, bool brake){
	Drivebase::Goal a;
	a.mode_ = Drivebase::Goal::Mode::ABSOLUTE;
	a.left_ = left;
	a.right_ = right;
	a.gear_ = gear;
	a.brake_ = brake;
	return a;
}

Drivebase::Goal Drivebase::Goal::drive_straight(){
	Drivebase::Goal a;
	a.mode_ = Drivebase::Goal::Mode::DRIVE_STRAIGHT;
	return a;
}

Drivebase::Goal Drivebase::Goal::rotate(){
	Drivebase::Goal a;
	a.mode_ = Drivebase::Goal::Mode::ROTATE;
	return a;
}

ostream& operator<<(ostream& o,Drivebase::Goal const& a){
	o << "Drivebase::Goal(" << a.mode();
	switch(a.mode()){
		case Drivebase::Goal::Mode::ROTATE:
			break;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
			break;
		case Drivebase::Goal::Mode::ABSOLUTE:
			o << " " << a.left() << " " << a.right() << a.gear();
			break;
		default: 
			nyi
	}
	o << " " << a.brake(); 
	o << ")";
	return o;
}


bool operator==(Drivebase::Goal const& a,Drivebase::Goal const& b){
	#define X(NAME) if(a.NAME != b.NAME) return false;
	X(mode())
	switch(a.mode()){
		case Drivebase::Goal::Mode::ABSOLUTE:
			X(left())
			X(right())
			X(gear())
			break;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
		case Drivebase::Goal::Mode::ROTATE:
			break;
		default:
			nyi
	}
	X(brake())
	#undef X
	return true;	
}


#define CMP(name) if(a.name<b.name) return 1; if(b.name<a.name) return 0;

bool operator<(Drivebase::Goal const& a,Drivebase::Goal const& b){
	CMP(mode())
	switch(a.mode()){
		case Drivebase::Goal::Mode::ABSOLUTE:
			CMP(left())
			CMP(right())
			CMP(gear())
			break;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
		case Drivebase::Goal::Mode::ROTATE:
			break;
		default:
			nyi
	}
	CMP(brake())
	return 0;
}

CMP_OPS(Drivebase::Output,DRIVEBASE_OUTPUT)

set<Drivebase::Output> examples(Drivebase::Output*){
	return {
		Drivebase::Output{0,0,false,false},
		Drivebase::Output{1,1,false,false}
	};
}

set<Drivebase::Input> examples(Drivebase::Input*){
	return {Drivebase::Input{
		{0,0},0.0
	}};
}

Drivebase::Estimator::Estimator():last({{0,0},{0,0},0.0,false,0.0,0.0}){}

Drivebase::Status_detail Drivebase::Estimator::get()const{
	return last;
}

ostream& operator<<(ostream& o,Drivebase::Output_applicator){
	return o<<"output_applicator";
}

ostream& operator<<(ostream& o,Drivebase const& a){
	return o<<"Drivebase("<<a.estimator.get()<<")";
}

double get_output(Drivebase::Output out,Drivebase::Motor m){
	#define X(NAME,POSITION) if(m==Drivebase::NAME) return out.POSITION;
#ifdef THEREMIN
	X(LEFT1,l)
	X(LEFT2,l)
	X(RIGHT1,r)
	X(RIGHT2,r)
#endif

#ifdef CLAYMORE
	X(LEFT1,l)
	X(LEFT2,l)
	X(LEFT3,l)

	X(RIGHT1,r)
	X(RIGHT2,r)
	X(RIGHT3,r)
#endif
	#undef X
	assert(0);
}

void Drivebase::Estimator::update(Time now,Drivebase::Input in,Drivebase::Output out){
	//paramsInput* input_params = Drivebase::drivebase_controller.getParams();

	last.distances = in.distances;
	last.angle = in.angle;

	speed_timer.update(now,true);
	static const double POLL_TIME = .05;//seconds
	if(speed_timer.done()){
		last.speeds.l = (last.distances.l-in.distances.l)/POLL_TIME;
		last.speeds.r = (last.distances.r-in.distances.r)/POLL_TIME;
		speed_timer.set(POLL_TIME);
	}

	/*if(last_shifter_output != out.high_gear) {
		double shift_separation_time = input_params->getValue("drivebase:shifter:shift_separation_time", 2.0);
		shift_timer.set(shift_separation_time);
	}
	shift_timer.update(now, true);
	last_shifter_output = out.high_gear;

	double turn_multiplier = input_params->getValue("drivebase:shifter:turn_multiplier", 1.2);
	bool turning = (last.speeds.r > last.speeds.l * turn_multiplier) || (last.speeds.l > last.speeds.r * turn_multiplier);

	if(shift_timer.done() && !turning) {
		double slow_shift_threshold = input_params->getValue("drivebase:shifter:slow_threshold", 5.0);
		double fast_shift_threshold = input_params->getValue("drivebase:shifter:fast_threshold", 5.0);
		
		double speed_average = (last.speeds.l + last.speeds.r) / 2.0;
		if(speed_average < slow_shift_threshold)
			last.high_gear_recommended = false;
		if(speed_average > fast_shift_threshold)
			last.high_gear_recommended = true;
	}*/
	last.high_gear_recommended = false;
	
	last.dt = now - last.now;
	last.now = now;
}

Robot_outputs Drivebase::Output_applicator::operator()(Robot_outputs robot,Drivebase::Output b)const{
#ifdef THEREMIN
	robot.talon_srx[L_MOTOR_LOC_1].power_level = b.l;
	robot.talon_srx[L_MOTOR_LOC_2].power_level = b.l;
	robot.talon_srx[R_MOTOR_LOC_1].power_level = b.r;
	robot.talon_srx[R_MOTOR_LOC_2].power_level = b.r;

	robot.talon_srx[L_MOTOR_LOC_1].brake = b.brake;
	robot.talon_srx[L_MOTOR_LOC_2].brake = b.brake;
	robot.talon_srx[R_MOTOR_LOC_1].brake = b.brake;
	robot.talon_srx[R_MOTOR_LOC_2].brake = b.brake;
#endif

#ifdef CLAYMORE
	robot.talon_srx[L_MOTOR_LOC_1].power_level = b.l;
	robot.talon_srx[L_MOTOR_LOC_2].power_level = b.l;
	robot.talon_srx[L_MOTOR_LOC_3].power_level = b.l;
	robot.talon_srx[R_MOTOR_LOC_1].power_level = b.r;
	robot.talon_srx[R_MOTOR_LOC_2].power_level = b.r;
	robot.talon_srx[R_MOTOR_LOC_3].power_level = b.r;

	robot.talon_srx[L_MOTOR_LOC_1].brake = b.brake;
	robot.talon_srx[L_MOTOR_LOC_2].brake = b.brake;
	robot.talon_srx[L_MOTOR_LOC_3].brake = b.brake;
	robot.talon_srx[R_MOTOR_LOC_1].brake = b.brake;
	robot.talon_srx[R_MOTOR_LOC_2].brake = b.brake;
	robot.talon_srx[R_MOTOR_LOC_3].brake = b.brake;
#endif

	robot.solenoid[SHIFTER_SOLENOID] = !b.high_gear;

	auto set_encoder=[&](unsigned int a, unsigned int b,unsigned int loc){
		robot.digital_io[a] = Digital_out::encoder(loc,1);
		robot.digital_io[b] = Digital_out::encoder(loc,0);
	};
	set_encoder(L_ENCODER_PORTS,L_ENCODER_LOC);
	set_encoder(R_ENCODER_PORTS,R_ENCODER_LOC);	

	return robot;
}

Drivebase::Output Drivebase::Output_applicator::operator()(Robot_outputs robot)const{
	//assuming both motors on the same side are set to the same value//FIXME ?
	return Drivebase::Output{	
		robot.talon_srx[L_MOTOR_LOC_1].power_level,
		-robot.talon_srx[R_MOTOR_LOC_1].power_level,
		!robot.solenoid[SHIFTER_SOLENOID],
		robot.talon_srx[L_MOTOR_LOC_1].brake
	};
}

bool operator==(Drivebase::Output_applicator const&,Drivebase::Output_applicator const&){
	return true;
}

bool operator==(Drivebase::Estimator const& a,Drivebase::Estimator const& b){
	return a.last == b.last && a.speed_timer == b.speed_timer;
}

bool operator!=(Drivebase::Estimator const& a,Drivebase::Estimator const& b){
	return !(a==b);
}

bool operator==(Drivebase const& a,Drivebase const& b){
	return a.estimator==b.estimator && a.output_applicator==b.output_applicator;
}

bool operator!=(Drivebase const& a,Drivebase const& b){
	return !(a==b);
}

Drivebase::Output control(Drivebase::Status status,Drivebase::Goal goal){
	Drivebase::Output out(0.0, 0.0, false, goal.brake());

	switch(goal.mode()){
		case Drivebase::Goal::Mode::ABSOLUTE:
			out = Drivebase::Output{goal.left(), goal.right(), false, goal.brake()};

			{
			messageLogger &logger = messageLogger::get();
			logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DRIVEBASE_RAW_DATA);
			logger << "Left Output: " << out.l;
			logger << " Right Output: " << out.r;
			logger.endMessage();
			}

			if(goal.gear() == Drivebase::Goal::Gear::AUTO)
				out.high_gear = status.high_gear_recommended;
			else
				out.high_gear = goal.gear() == Drivebase::Goal::Gear::HIGH;
	
			Drivebase::drivebase_controller.idle(status.distances.l, status.distances.r, status.angle, status.dt, status.now) ;
			break;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
		case Drivebase::Goal::Mode::ROTATE:
		   	Drivebase::drivebase_controller.update(status.distances.l, status.distances.r, status.angle, status.dt, status.now, out.l, out.r, out.high_gear) ;
			break;
		default:
			nyi
	}
	return out;
}

Drivebase::Status status(Drivebase::Status a){ return a; }

bool ready(Drivebase::Status status,Drivebase::Goal goal){
	switch(goal.mode()){
		case Drivebase::Goal::Mode::ABSOLUTE:
			return true;
		case Drivebase::Goal::Mode::DRIVE_STRAIGHT:
		case Drivebase::Goal::Mode::ROTATE:
			return Drivebase::drivebase_controller.done();
		default:
			nyi
	}
}

#ifdef DRIVEBASE_TEST
#include "formal.h"
#include <unistd.h>
#include <fstream>

//temp
using Inch = double;

static const Inch ROBOT_WIDTH = 28; //inches, ignores bumpers //TODO: finds some way of dealing with constants like this and wheel diameter

struct Drivebase_sim{
	using Input=Drivebase::Input;
	using Output=Drivebase::Output;
	
	Point position; //x,y are in distance in feet, theta is in radians (positive is counterclockwise from straight forward)
	Time last_time;
	Drivebase::Distances distances;

	void update(Time t,bool enable,Output out){
		static const double POWER_TO_SPEED = 6.5 * 12;//speed is in/s assuming low gear
		Time dt=t-last_time;
		last_time=t;
		if(!enable) return;
		Drivebase::Speeds speeds = {out.l * POWER_TO_SPEED, out.r * POWER_TO_SPEED};
		Drivebase::Distances ddistances = {speeds.l * dt, speeds.r * dt};
		double avg_dist_traveled = mean(ddistances.l,ddistances.r);
		double dtheta = ((out.l-out.r)*POWER_TO_SPEED*dt)/ROBOT_WIDTH;
		/*
			Angle is calculated as the difference between the two sides' powers divided by 2 --   (out.l - out.r) / 2
			That is then converted to a distance   --   ((out.l - out.r) / 2) * POWER_TO_SPEED * dt
			That distance is then converted to an angle -- ((((out.l - our.r) / 2) * POWER_TO_SPEED * dt) * 2) / ROBOT_WIDTH
		*/
		double dy = avg_dist_traveled * cosf(position.theta);
		double dx = avg_dist_traveled * sinf(position.theta);
		distances += ddistances;
		position += {dx,dy,dtheta};
	}
	Input get()const{
		auto d = Digital_in::_0;
		auto p = make_pair(d,d);
		Drivebase::Input in = {Drivebase::Input{
			{0,0,0,0},p,p,distances,0.0
		}};
		return in;
	}

	Drivebase_sim():position({}),last_time(0),distances({0,0}){}

};

ostream& operator<<(ostream& o,Drivebase_sim const& a){
	return o << "Drivebase_sim(" << a.position << ")";
}

///end temp

int main(){
	{
		Drivebase d;//TODO: re-enable
		tester(d);
	}
	/*
	{
		Drivebase::Encoder_ticks a = {100,100}, b = {10,10};
		Drivebase::Encoder_ticks diff = a - b, sum = a + b, opp = -a;
		cout<<"\na:"<<a<<" b:"<<b<<" diff:"<<diff<<" sum:"<<sum<<" opp:"<<opp<<"\n";
	}
	*/
	{
		cout<<"\n==========================================================\n";
		Drivebase_sim drive_sim;
		Drivebase drive;
		const Time TIMESTEP = .020;//sec
		const Time MAX_TEST_LENGTH = 15;//sec
		
		Drivebase::Goal goal = Drivebase::Goal::distances({100,100});//inches
		
		const bool ENABLED = true;
		
		for(Time t = 0; t < MAX_TEST_LENGTH; t += TIMESTEP){
			Drivebase::Status_detail status = drive.estimator.get();
			Drivebase::Output out = control(status,goal);
			
			drive_sim.update(t,ENABLED,out);
			
			Drivebase::Input input = drive_sim.get();
			
			drive.estimator.update(t,input,out);
			
			cout<<"t:"<<t<<"\tgoal:"<<goal<<"\tstatus:"<<status<<"\n";
			
			if(ready(status,goal)){
				cout<<"t:"<<t<<"\tgoal "<<goal<<" reached with status "<<status<<".\nFinishing\n";
				break;
			}
		}
	}
}
#endif
