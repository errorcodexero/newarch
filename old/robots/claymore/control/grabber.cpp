#include "grabber.h"
#include "subsystems.h"
#include "message_logger.h"
#include <cmath>

using namespace std;

#define GRABBER_ADDRESS 2

#define ENCODER_ADDRESS 3
#define ENCODER_DIOS 6, 7
#define CUBE_SENSOR_ADDRESS 12
#define LIMIT_SWITCH_ADDRESS 11

const double DEGREES_PER_TICK = 90.0 / 144.0;

GrabberController Grabber::grabber_controller;

ostream& operator<<(ostream& o, Grabber::Goal::Mode a){
#define X(MODE) if(a==Grabber::Goal::Mode::MODE) return o<<""#MODE;
    GRABBER_GOAL_MODES
#undef X
	assert(0);
}

ostream& operator<<(ostream& o, Grabber::Goal a){
    o<<"(";
    o<<"mode:"<<a.mode();
    if(a.mode() == Grabber::Goal::Mode::GO_TO_ANGLE){
	o<<" target:"<<a.target();
    }
    o<<")";
    return o;
}

Grabber::Goal::Mode Grabber::Goal::mode()const{
    return mode_;
}

double Grabber::Goal::target()const{
    return target_;
}

GrabberController::Preset Grabber::Goal::preset_target()const{
    return preset_target_;
}

Grabber::Goal::Goal():mode_(Grabber::Goal::Mode::IDLE),target_(0.0),preset_target_(GrabberController::Preset::CLOSED){}

Grabber::Goal Grabber::Goal::idle(){
    Grabber::Goal a;
    a.mode_ = Grabber::Goal::Mode::IDLE;
    return a;
}

Grabber::Goal Grabber::Goal::hold(){
    Grabber::Goal a;
    a.mode_ = Grabber::Goal::Mode::HOLD;
    return a;
}

Grabber::Goal Grabber::Goal::clamp(){
	Grabber::Goal a;
	a.mode_ = Grabber::Goal::Mode::CLAMP;
	return a;
}

Grabber::Goal Grabber::Goal::open(){
    Grabber::Goal a;
    a.mode_ = Grabber::Goal::Mode::OPEN;
    return a;
}

Grabber::Goal Grabber::Goal::close(){
    Grabber::Goal a;
    a.mode_ = Grabber::Goal::Mode::CLOSE;
    return a;
}

Grabber::Goal Grabber::Goal::go_to_angle(double target){
    Grabber::Goal a;
    a.mode_ = Grabber::Goal::Mode::GO_TO_ANGLE;
    a.target_ = target;
    return a;
}

Grabber::Goal Grabber::Goal::go_to_preset(GrabberController::Preset preset){
    Grabber::Goal a;
    a.mode_ = Grabber::Goal::Mode::GO_TO_PRESET;
    a.preset_target_ = preset;
    return a;
}

Grabber::Goal Grabber::Goal::calibrate(){
    Grabber::Goal a;
    a.mode_ = Grabber::Goal::Mode::CALIBRATE;
    return a;
}

Grabber::Input::Input(int t, bool hc, bool ls):ticks(t),has_cube(hc),limit_switch(ls){}
Grabber::Input::Input():Input(0, false, false){}

Grabber::Status_detail::Status_detail(bool hc, bool ol, bool il, double a, double t, double dt):has_cube(hc),outer_limit(ol),inner_limit(il),angle(a),time(t),dt(dt){}
Grabber::Status_detail::Status_detail():Status_detail(false, false, false, 0.0, 0.0, 0.0){}

Grabber::Estimator::Estimator():last(){}

std::set<Grabber::Goal> examples(Grabber::Goal*){
    return {
		Grabber::Goal::idle(),
		Grabber::Goal::hold(),
		Grabber::Goal::open(),
		Grabber::Goal::close(),
		Grabber::Goal::go_to_angle(0.0),
		Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED),
		Grabber::Goal::calibrate(),
	};
}

std::set<Grabber::Output> examples(Grabber::Output*){
    return {-0.4,0.0,0.4};
}

std::set<Grabber::Input> examples(Grabber::Input*){
    return {
		{0, false, false},
		{1, false, false},
		{0, false, true},
		{1, false, true},
		{0, true, false},
		{1, true, false},
		{0, true, true},
		{1, true, true}
    };
}

std::ostream& operator<<(std::ostream& o,Grabber::Input a){
    o<<"(";
    o<<"ticks:"<<a.ticks;
    o<<"has_cube:"<<a.has_cube;
    o<<"limit_switch:"<<a.limit_switch;
    o<<")";
    return o;
}

std::set<Grabber::Status_detail> examples(Grabber::Status_detail*){
    return {
		{false, false, false, 0.0, 0.0, 0.0},
		{false, true, false, 0.0, 0.0, 0.0},
		{true, false, false, 0.0, 0.0, 0.0},
		{true, true, false, 0.0, 0.0, 0.0}
    };
}

std::ostream& operator<<(std::ostream& o,Grabber::Status_detail a){
    o<<"(";
    o<<"angle:"<<a.angle;
    o<<"time:"<<a.time;
    o<<"dt:"<<a.dt;
    o<<"has_cube:"<<a.has_cube;
    o<<")";
    return o;
}

#define CMP(VAR)				\
    if(a.VAR < b.VAR) return true;		\
    if(b.VAR < a.VAR) return false; 

bool operator<(Grabber::Status_detail a,Grabber::Status_detail b){
    CMP(angle)
	return false;
}

bool operator==(Grabber::Status_detail a,Grabber::Status_detail b){
    return a.angle == b.angle && a.time == b.time && a.dt == b.dt && a.has_cube == b.has_cube && a.outer_limit == b.outer_limit && a.inner_limit == b.inner_limit;
}

bool operator!=(Grabber::Status_detail a, Grabber::Status_detail b){
    return !(a==b);
}

bool operator<(Grabber::Goal a,Grabber::Goal b){
    CMP(mode())
	if(a.mode() == Grabber::Goal::Mode::GO_TO_ANGLE){
	    CMP(target())
		}
    return false;
}

std::ostream& operator<<(std::ostream& o,Grabber const&){
    return o<<"Grabber()";
}

bool operator<(Grabber::Input a,Grabber::Input b){
    CMP(ticks)
	return false;
}

bool operator==(Grabber::Input a,Grabber::Input b){
    return a.ticks == b.ticks && a.has_cube == b.has_cube && a.limit_switch == b.limit_switch;
}

bool operator!=(Grabber::Input a, Grabber::Input b){
    return !(a==b);
}

bool operator==(Grabber::Estimator a, Grabber::Estimator b){
    return a.last==b.last;
}

bool operator!=(Grabber::Estimator a, Grabber::Estimator b){
    return !(a==b);
}

ostream& operator<<(ostream& o, Grabber::Estimator a){
    return o<<"(last:"<<a.last<<")";
}

bool operator==(Grabber,Grabber){
    return 1;
}

bool operator!=(Grabber a, Grabber b){
    return !(a==b);
}

#undef CMP

Grabber::Input Grabber::Input_reader::operator()(Robot_inputs const& r) const{
    int enc_val = 9999;
    if(r.digital_io.encoder[ENCODER_ADDRESS])
	enc_val = *(r.digital_io.encoder[ENCODER_ADDRESS]);
    return Grabber::Input(
	enc_val,
	r.digital_io.in[CUBE_SENSOR_ADDRESS] == Digital_in::_0,
	r.digital_io.in[LIMIT_SWITCH_ADDRESS] == Digital_in::_0
	);
}

Robot_inputs Grabber::Input_reader::operator()(Robot_inputs r, Grabber::Input in) const{
    r.digital_io.encoder[ENCODER_ADDRESS] = in.ticks;
    r.digital_io.in[CUBE_SENSOR_ADDRESS] = in.has_cube ? Digital_in::_0 : Digital_in::_1;
    r.digital_io.in[LIMIT_SWITCH_ADDRESS] = in.limit_switch ? Digital_in::_0 : Digital_in::_1;
    return r;
}

Robot_outputs Grabber::Output_applicator::operator()(Robot_outputs r, Grabber::Output o)const{
    r.pwm[GRABBER_ADDRESS] = o;

    auto set_encoder=[&](unsigned int a, unsigned int b,unsigned int loc){
	r.digital_io[a] = Digital_out::encoder(loc,1);
	r.digital_io[b] = Digital_out::encoder(loc,0);
    };
    set_encoder(ENCODER_DIOS, ENCODER_ADDRESS);

    return r;
}

Grabber::Output Grabber::Output_applicator::operator()(Robot_outputs const& r)const{
    return r.pwm[GRABBER_ADDRESS];
}

void Grabber::Estimator::update(Time time,Grabber::Input input,Grabber::Output out){
    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER);

    logger<<"Grabber:\n";

    paramsInput* input_params = paramsInput::get() ;

    if(Grabber::grabber_controller.isCalibrating()) {
		ticks_history.push_back(input.ticks);
		logger << "CALIBRATING:" ;
		for(const auto &v : ticks_history)
			logger << " " << v ;
		
		size_t samples = static_cast<int>(input_params->getValue("grabber:samples", 5.0) + 0.5) ;
		if(ticks_history.size() > samples)
			ticks_history.pop_front();
		
		double delta = ticks_history.back() - ticks_history.front() ;
		double thresh = input_params->getValue("grabber:calibrate_threshold", 0.1) ;
		logger << ", samples " << samples ;
		logger << ", delta " << delta ;
		logger << ", thresh " << thresh ;
		
		if (ticks_history.size() == samples && fabs(delta) < thresh) {
			encoder_offset = input.ticks;
			Grabber::grabber_controller.hold() ;
			logger << "DONE CALIBRATING - switching to HOLD state" << "\n";
		}	
    }

    last.angle = (input.ticks - encoder_offset) * DEGREES_PER_TICK;

    last.has_cube = input.has_cube;

    last.outer_limit = input.limit_switch || last.angle > input_params->getValue("grabber:angle:stowed", 90.0);
    last.inner_limit = last.angle < input_params->getValue("grabber:angle:closed", 0.0);

    last.dt = time - last.time;
    last.time = time;

    logger << "Ticks: " << input.ticks << " Angle: " << last.angle << "\n";
    logger << "Inner Limit: " << last.inner_limit << " Outer Limit: " << last.outer_limit << " Has Cube: " << last.has_cube << "\n";

    logger.endMessage();
}

Grabber::Status Grabber::Estimator::get()const{
    return last;
}

Grabber::Output control(Grabber::Status_detail status,Grabber::Goal goal){
    Grabber::Output out = 0.0;

    //
    // We defer all of the work to the grabber_controller object
    //
    switch(goal.mode()){
    case Grabber::Goal::Mode::IDLE:
		Grabber::grabber_controller.setIdle() ;
		break;
    case Grabber::Goal::Mode::HOLD:
		Grabber::grabber_controller.hold() ;
		break;
	case Grabber::Goal::Mode::CLAMP:
		Grabber::grabber_controller.clamp() ;
		break;
    case Grabber::Goal::Mode::CLOSE:
		Grabber::grabber_controller.close() ;
		break;
    case Grabber::Goal::Mode::OPEN:
		Grabber::grabber_controller.open() ;
		break;	
    case Grabber::Goal::Mode::GO_TO_ANGLE:
		Grabber::grabber_controller.moveToAngle(goal.target(), status.time) ;
		break;
    case Grabber::Goal::Mode::GO_TO_PRESET:
		Grabber::grabber_controller.moveToAngle(goal.preset_target(), status.time) ;
		break ;
    case Grabber::Goal::Mode::CALIBRATE:
		break;
    default:
		assert(0);
    }

    //
    // We call update all the time now that all of the work is in the grabber_controller
    //
    Grabber::grabber_controller.update(status.angle, status.has_cube, status.time, status.dt, out);

	messageLogger &logger = messageLogger::get();
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_GRABBER);
	logger << "Grabber output: " << out;
	logger.endMessage();


    return out;
}

Grabber::Status status(Grabber::Status s){
    return s;
}

bool ready(Grabber::Status status,Grabber::Goal goal){
    switch(goal.mode()){
    case Grabber::Goal::Mode::IDLE:
    case Grabber::Goal::Mode::HOLD:
    case Grabber::Goal::Mode::OPEN:
    case Grabber::Goal::Mode::CLOSE:
		return true;
	case Grabber::Goal::Mode::CLAMP:
		return status.angle < paramsInput::get()->getValue("grabber:clamp_angle", 20.0);
    case Grabber::Goal::Mode::GO_TO_ANGLE:
    case Grabber::Goal::Mode::GO_TO_PRESET:
		return Grabber::grabber_controller.done();
    case Grabber::Goal::Mode::CALIBRATE:
		return !Grabber::grabber_controller.isCalibrating();
    default:
	assert(0);
    }
}

#ifdef GRABBER_TEST
#include "formal.h"

int main(){
    Grabber a;
    tester(a);
}
#endif
