#include "lifter.h"
#include "util.h"
#include "subsystems.h"
#include "message_logger.h"
#include <cmath>
#include <sstream>

using namespace std;

#define LIFTER_ADDRESS_L 0
#define LIFTER_ADDRESS_R 1

#define LIFTER_SHIFTER 1
#define LIFTER_LOCK_SOLENOID 3

#define BOTTOM_HALL_EFFECT_ADDRESS 9
#define TOP_HALL_EFFECT_ADDRESS 8

#define ENCODER_ADDRESS 2
#define ENCODER_DIOS 4, 5

const double INCHES_PER_TICK_HIGH_GEAR = .08327; //(1.0 / 11.4201); //From Jeff
const double COLLECTOR_OFFSET = 11.375; //inches

LifterController Lifter::lifter_controller;

ostream& operator<<(ostream& o, Lifter::Goal::Mode a){
#define X(MODE) if(a==Lifter::Goal::Mode::MODE) return o<<""#MODE;
    LIFTER_GOAL_MODES
#undef X
		assert(0);
}

Lifter::Goal::Mode Lifter::Goal::mode()const{
    return mode_;
}

double Lifter::Goal::target()const{
    return target_;
}

double Lifter::Goal::tolerance()const{
    return tolerance_;
}

LifterController::Preset Lifter::Goal::preset_target()const{
    return preset_target_;
}

bool Lifter::Goal::high_power()const{
    return high_power_;
}

std::string Lifter::Goal::toString() const
{
	std::stringstream strm ;

	strm << *this ;
	return strm.str() ;
}

Lifter::Goal::Goal():mode_(Lifter::Goal::Mode::STOP),target_(0.0),tolerance_(0.0),preset_target_(LifterController::Preset::FLOOR),high_power_(false){}

Lifter::Goal Lifter::Goal::climb()
{
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::CLIMB;
    return a;
}

Lifter::Goal Lifter::Goal::maintain_climb(){
	Lifter::Goal a;
	a.mode_ = Lifter::Goal::Mode::MAINTAIN_CLIMB;
	return a;
}

Lifter::Goal Lifter::Goal::up(bool high_power = false){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::UP;
    a.high_power_ = high_power;
    return a;
}

Lifter::Goal Lifter::Goal::stop(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::STOP;
    return a;
}

Lifter::Goal Lifter::Goal::down(bool high_power = false){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::DOWN;
    a.high_power_ = high_power;
    return a;
}

Lifter::Goal Lifter::Goal::go_to_height(double target){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::GO_TO_HEIGHT;
    a.target_ = target;
    return a;
}

Lifter::Goal Lifter::Goal::go_to_preset(LifterController::Preset preset){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::GO_TO_PRESET;
    a.preset_target_ = preset;
    return a;
}

Lifter::Goal Lifter::Goal::background(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::BACKGROUND;
    return a;
}

Lifter::Goal Lifter::Goal::calibrate(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::CALIBRATE;
    return a;
}

Lifter::Goal Lifter::Goal::low_gear(){
    Lifter::Goal a;
    a.mode_ = Lifter::Goal::Mode::LOW_GEAR;
    return a;
}

Lifter::Output::Output(double p, Lifter::Output::Gearing g, bool l):power(p),gearing(g),lock(l){}
Lifter::Output::Output():Output(0,Lifter::Output::Gearing::HIGH,false){}

Lifter::Input::Input(bool b, bool t, int e):bottom_hall_effect(b),top_hall_effect(t),ticks(e){}
Lifter::Input::Input():Input(false,false,0){}

Lifter::Status_detail::Status_detail(bool bl, bool tl, int tk, double ti,double delt)
{
	at_bottom_limit = bl ;
	at_top_limit = tl ;
	ticks = tk ;
	time = ti ;
	dt = delt ;
}

Lifter::Status_detail::Status_detail()
{
	at_bottom_limit = false ;
	at_top_limit = false ;
	ticks = 0 ;
	time = 0.0 ;
	dt = 0.0 ;
}

Lifter::Estimator::Estimator(Lifter::Status_detail s)
{
	last = s ;
}

Lifter::Estimator::Estimator()
{
}

#define CMP(VAR)								\
    if(a.VAR < b.VAR) return true;				\
    if(b.VAR < a.VAR) return false; 	

bool operator==(Lifter::Output const& a,Lifter::Output const& b){
    return a.power == b.power && a.gearing == b.gearing && a.lock == b.lock;
}

bool operator!=(Lifter::Output const& a,Lifter::Output const& b){
    return !(a==b);
}
	
bool operator<(Lifter::Output const& a,Lifter::Output const& b){
    CMP(power)
		CMP(gearing)
		CMP(lock)
		return false;
}

ostream& operator<<(ostream& o, Lifter::Output const& a){
    o<<"(";
    o<<"power:"<<a.power;
    o<<" gearing:"<<a.gearing;
    o<<" lock:"<<a.lock;
    o<<")";
    return o;
}
	
bool operator==(Lifter::Input const& a, Lifter::Input const& b){
    return a.bottom_hall_effect == b.bottom_hall_effect && a.top_hall_effect == b.top_hall_effect && a.ticks == b.ticks;
}

bool operator!=(Lifter::Input const& a, Lifter::Input const& b){
    return !(a==b);
}

bool operator<(Lifter::Input const& a,Lifter::Input const& b){
    CMP(bottom_hall_effect)
		CMP(top_hall_effect)
		CMP(ticks)
		return false;
}

ostream& operator<<(ostream& o, Lifter::Input const& a){
    o<<"(";
    o<<"bottom_hall_effect:"<<a.bottom_hall_effect;
    o<<" top_hall_effect:"<<a.top_hall_effect;
    o<<" ticks:"<<a.ticks;
    o<<")";
    return o;
}

ostream& operator<<(ostream& o, Lifter::Status const& a){
#define X(STATUS) if(a==Lifter::Status::STATUS) return o<<""#STATUS;
    LIFTER_STATUSES
#undef X
		assert(0);
}

bool operator==(Lifter::Estimator const& a, Lifter::Estimator const& b){
    return true;
}

bool operator!=(Lifter::Estimator const& a, Lifter::Estimator const& b){
    return !(a==b);
}

bool operator==(Lifter::Goal const& a, Lifter::Goal const& b){
    return a.mode() == b.mode() && a.target() == b.target() && a.tolerance() == b.tolerance() && a.preset_target() == b.preset_target();
}

bool operator!=(Lifter::Goal const& a, Lifter::Goal const& b){
    return !(a==b);
}

bool operator<(Lifter::Goal const& a,Lifter::Goal const& b){
    if(a.mode() == b.mode() && a.mode() == Lifter::Goal::Mode::GO_TO_HEIGHT){
		CMP(target())
			CMP(tolerance())
			}
    return a.mode() < b.mode();
}

ostream& operator<<(ostream& o, Lifter::Goal const& a){
    o<<"Goal( "<<a.mode();
    if(a.mode() == Lifter::Goal::Mode::GO_TO_HEIGHT){
		o<<" "<<a.target();
		o<<" "<<a.tolerance();
    }
    o<<")";
    return o;
}

bool operator==(Lifter::Status_detail const& a,Lifter::Status_detail const& b)
{
	assert(false) ;
    return true ;
}

bool operator!=(Lifter::Status_detail const& a,Lifter::Status_detail const& b){
    return !(a==b);
}

bool operator<(Lifter::Status_detail const& a,Lifter::Status_detail const& b){
	assert(false) ;
	return false;
}

ostream& operator<<(ostream& o, Lifter::Status_detail const& a){
    o<<"(";
	o << "at_bottom_limit " << a.at_bottom_limit ;
	o << " at_top_limit " << a.at_top_limit ;
	o << " ticks " << a.ticks ;
	o << " time " << a.time ;
	o << " dt " << a.dt ;
    o<<")";
    return o;
}

ostream& operator<<(ostream& o, Lifter const& a){
    return o<<"Lifter("<<a.estimator.get()<<")";
}

#undef CMP

Robot_outputs Lifter::Output_applicator::operator()(Robot_outputs r, Lifter::Output const& out)const{
    r.pwm[LIFTER_ADDRESS_L] = -out.power;
    r.pwm[LIFTER_ADDRESS_R] = -out.power;
    r.solenoid[LIFTER_SHIFTER] = (out.gearing == Lifter::Output::Gearing::LOW) ;
    r.solenoid[LIFTER_LOCK_SOLENOID] = !out.lock;

    auto set_encoder=[&](unsigned int a, unsigned int b,unsigned int loc){
		r.digital_io[a] = Digital_out::encoder(loc,1);
		r.digital_io[b] = Digital_out::encoder(loc,0);
    };
    set_encoder(ENCODER_DIOS, ENCODER_ADDRESS);

    return r;
};

Lifter::Output Lifter::Output_applicator::operator()(Robot_outputs const& r)const{
    return {
		-r.pwm[LIFTER_ADDRESS_L], //assuming that left and right sides are set to the same value
			r.solenoid[LIFTER_SHIFTER] ? Lifter::Output::Gearing::LOW : Lifter::Output::Gearing::HIGH,
			r.solenoid[LIFTER_LOCK_SOLENOID]
			};
}

Robot_inputs Lifter::Input_reader::operator()(Robot_inputs r, Lifter::Input const& in)const{
    r.digital_io.in[BOTTOM_HALL_EFFECT_ADDRESS] = in.bottom_hall_effect ? Digital_in::_0 : Digital_in::_1; //Active low
    r.digital_io.in[TOP_HALL_EFFECT_ADDRESS] = in.top_hall_effect ? Digital_in::_0 : Digital_in::_1;
    r.digital_io.encoder[ENCODER_ADDRESS] = -in.ticks;
    return r;
}

Lifter::Input Lifter::Input_reader::operator()(Robot_inputs const& r)const{
    int enc_val = 9999;
    if(r.digital_io.encoder[ENCODER_ADDRESS])
		enc_val = *(r.digital_io.encoder[ENCODER_ADDRESS]);
    enc_val = -enc_val;
    Lifter::Input result = Lifter::Input(
		r.digital_io.in[BOTTOM_HALL_EFFECT_ADDRESS] == Digital_in::_0,
		r.digital_io.in[TOP_HALL_EFFECT_ADDRESS] == Digital_in::_0,
		enc_val
		);
    return result;
}

void Lifter::Estimator::update(Time const& now, Lifter::Input const& in, Lifter::Output const& out){
    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);

	last.ticks = in.ticks;

    last.at_bottom_limit = in.bottom_hall_effect;
    last.at_top_limit = in.top_hall_effect;

    last.dt = now - last.time;
    last.time = now;
    logger << "Lifter: Limit Switches: Top: " << in.top_hall_effect << "   Bottom: " << in.bottom_hall_effect << "\n";

    logger.endMessage();
}

Lifter::Status_detail Lifter::Estimator::get()const
{
    return last;
}

set<Lifter::Input> examples(Lifter::Input*)
{
    return {
		{false,false,0},
		{true,false,0},
		{false,true,0},
		{true,true,0},
			};
}

set<Lifter::Output> examples(Lifter::Output*){
    return {
		{0.0, Lifter::Output::Gearing::HIGH, false}, 
		{0.0, Lifter::Output::Gearing::LOW, false}
    };
}

set<Lifter::Status_detail> examples(Lifter::Status_detail*){
    return {
		{ false, false, 0, 0.0, 0.0 }
    };
}

set<Lifter::Status> examples(Lifter::Status*){
    return {Lifter::Status::CLIMBED,Lifter::Status::MIDDLE,Lifter::Status::BOTTOM,Lifter::Status::TOP,Lifter::Status::ERROR};
}

set<Lifter::Goal> examples(Lifter::Goal*){
    return {Lifter::Goal::climb(),Lifter::Goal::maintain_climb(),Lifter::Goal::up(),Lifter::Goal::stop(),Lifter::Goal::down(),Lifter::Goal::go_to_height(0.0)};
}

Lifter::Output control(Lifter::Status_detail const& status_detail, Lifter::Goal const& goal)
{
    // paramsInput* input_params = paramsInput::get() ;
    Lifter::Output out = {0.0, Lifter::Output::Gearing::HIGH, false};
	
    switch(goal.mode()){
    case Lifter::Goal::Mode::CLIMB:
		Lifter::lifter_controller.climb() ;
		break;
	case Lifter::Goal::Mode::MAINTAIN_CLIMB:
		Lifter::lifter_controller.maintain() ;
		break ;
    case Lifter::Goal::Mode::UP:
		Lifter::lifter_controller.up(goal.high_power()) ;
		break;
    case Lifter::Goal::Mode::DOWN:
		Lifter::lifter_controller.down(goal.high_power()) ;
		break;
    case Lifter::Goal::Mode::STOP:
		Lifter::lifter_controller.idle() ;
		break; 
    case Lifter::Goal::Mode::GO_TO_HEIGHT:
		Lifter::lifter_controller.moveToHeight(goal.target(), status_detail.time);
		break;
    case Lifter::Goal::Mode::GO_TO_PRESET:
		Lifter::lifter_controller.moveToHeight(goal.preset_target(), status_detail.time) ;
		break;
    case Lifter::Goal::Mode::CALIBRATE:
		Lifter::lifter_controller.calibrate() ;
		break;
    case Lifter::Goal::Mode::LOW_GEAR:
		Lifter::lifter_controller.idle() ;
		Lifter::lifter_controller.lowgear() ;
		break;
    default:
		nyi ;
	}

	LifterController::Gear g ;
	bool brake ;
	Lifter::lifter_controller.update(status_detail.ticks, status_detail.at_top_limit, status_detail.at_bottom_limit,
									 status_detail.time, status_detail.dt, out.power, g, brake);

	
	if (g == LifterController::Gear::LOW)
	{
		out.gearing = Lifter::Output::Gearing::LOW ;
	}
	else
	{
		out.gearing = Lifter::Output::Gearing::HIGH ;
	}
	
	out.lock = brake ;
	
    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_LIFTER);
    logger << "    Lifter status: " << out.power << "\n";
	logger << "    Lifter lock: " << (out.lock ? "locked" : "unlocked") << "\n" ;

    logger << "After: " << out.power << ", lock " << (out.lock ? "LOCKED" : "UNLOCKED") << "\n" ;
    logger.endMessage();

    return out;
}

Lifter::Status status(Lifter::Status_detail const& status_detail)
{
    return Lifter::Status::MIDDLE;
}

bool ready(Lifter::Status const& status,Lifter::Goal const& goal)
{
	bool ret = true ;
	
    switch(goal.mode()){
    case Lifter::Goal::Mode::CLIMB:
		ret = Lifter::lifter_controller.isClimbed() ;
		break ;
	case Lifter::Goal::Mode::MAINTAIN_CLIMB:
		ret = false;
		break;
    case Lifter::Goal::Mode::UP:
		ret = true ;
		break ;
    case Lifter::Goal::Mode::DOWN:
		ret = true ;
		break ;
    case Lifter::Goal::Mode::STOP:
		ret = true ;
		break ;
    case Lifter::Goal::Mode::CALIBRATE:
		ret = Lifter::lifter_controller.isCalibrated() ;
		break ;
    case Lifter::Goal::Mode::LOW_GEAR:
		ret = true ;
		break ;
    case Lifter::Goal::Mode::GO_TO_HEIGHT:
		ret = Lifter::lifter_controller.atHeight(goal.target());
		break ;
    case Lifter::Goal::Mode::GO_TO_PRESET:
		ret = Lifter::lifter_controller.atHeight(goal.preset_target());
		break ;
    default:
		assert(0) ;
		ret = true ;
		break ;
	}
	
	return ret ;
}

#ifdef LIFTER_TEST
#include "formal.h"

int main(){
    Lifter lifter;
    tester(lifter);
}

#endif
