#include "front.h"
#include <stdlib.h>
#include "../util/util.h"

using namespace std;

#define FRONT_ADDRESS 2
#define FRONT_SPEED 1
#define CLEAR_BALL_SPEED -.6

Front::Input::Input():ball(0){}

Front::Input::Input(bool b):ball(b){}

ostream& operator<<(ostream& o, Front::Goal a){
	#define X(name) if(a==Front::Goal::name)return o<<"Front::Goal("#name")";
	X(IN) X(OUT) X(OFF)
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o, Front::Input a){ return o<<"Front::Input(ball:"<<a.ball<<")";}
ostream& operator<<(ostream& o, Front){ return o<<"Front()";}

bool operator==(Front::Input a, Front::Input b){ return a.ball==b.ball;}
bool operator!=(Front::Input a, Front::Input b){ return !(a==b);}
bool operator<(Front::Input a, Front::Input b){ return a.ball<b.ball;}

bool operator==(Front::Input_reader,Front::Input_reader){ return 1;}
bool operator<(Front::Input_reader, Front::Input_reader){ return 0;}

bool operator==(Front::Estimator, Front::Estimator){ return 1;}
bool operator!=(Front::Estimator, Front::Estimator){ return 0;}

bool operator==(Front::Output_applicator, Front::Output_applicator){return 1;}

bool operator==(Front a, Front b){ return (a.input_reader==b.input_reader && a.estimator==b.estimator && a.output_applicator==b.output_applicator);}
bool operator!=(Front a, Front b){ return !(a==b);}

static const unsigned BALL_SENSOR_DIO=6;

Front::Input Front::Input_reader::operator()(Robot_inputs const& a)const{
	return Front::Input{a.digital_io.in[BALL_SENSOR_DIO]==Digital_in::_0};
}

Robot_inputs Front::Input_reader::operator()(Robot_inputs a, Front::Input b)const{
	a.digital_io.in[BALL_SENSOR_DIO]=b.ball?Digital_in::_0:Digital_in::_1;
	return a;
}

Robot_outputs Front::Output_applicator::operator()(Robot_outputs r, Front::Output out)const{
	/*r.relay[FRONT_ADDRESS]=[&]{
		switch(out){
			case Front::Output::OUT: return Relay_output::_01;
			case Front::Output::IN: return Relay_output::_10;
			default: return Relay_output::_00;
		}
	}();*/
	if(out.motor==Front::Goal::OUT) r.pwm[FRONT_ADDRESS]=-FRONT_SPEED;
	else if(out.motor==Front::Goal::OFF) r.pwm[FRONT_ADDRESS]=0;
	else if(out.motor==Front::Goal::IN) r.pwm[FRONT_ADDRESS]=FRONT_SPEED;
	else if(out.motor==Front::Goal::CLEAR_BALL) r.pwm[FRONT_ADDRESS]=CLEAR_BALL_SPEED;
	r.panel_output[Panel_outputs::BOULDER] = Panel_output(static_cast<int>(Panel_output_ports::BOULDER), out.ball_light);

	return r;
}

Front::Estimator::Estimator():input({0}){}

Front::Status_detail Front::Estimator::get()const{ return input;}

void Front::Estimator::update(Time,Front::Input input1,Front::Output){
	input=input1;
}

Front::Output Front::Output_applicator::operator()(Robot_outputs r)const{
	//return (r.relay[FRONT_ADDRESS]==Relay_output::_01? Front::Output::OUT : (r.relay[FRONT_ADDRESS]==Relay_output::_10? Front::Output::IN : Front::Output::OFF));
	auto motor=[&](){
		if(r.pwm[FRONT_ADDRESS]==-FRONT_SPEED)return Front::Goal::OUT;
		if(r.pwm[FRONT_ADDRESS]==0)return Front::Goal::OFF;
		if(r.pwm[FRONT_ADDRESS]==FRONT_SPEED)return Front::Goal::IN;
		if(r.pwm[FRONT_ADDRESS]==CLEAR_BALL_SPEED) return Front::Goal::CLEAR_BALL;
		assert(0);
	}();
	bool ball_sensor=r.panel_output[Panel_outputs::BOULDER].value;
	return Front::Output{motor,ball_sensor};
}
	
set<Front::Input> examples(Front::Input*){
	return {Front::Input{0},Front::Input{1}};
}

set<Front::Goal> examples(Front::Goal*){ 
	return {Front::Goal::OUT,Front::Goal::OFF,Front::Goal::IN};
}

Front::Output control(Front::Status_detail st, Front::Goal goal){
	return {goal,st.ball};
}

Front::Status status(Front::Status_detail a){ return a;}

bool ready(Front::Status, Front::Goal){ return 1;}

bool operator==(Front::Output a,Front::Output b){
	return a.motor==b.motor && a.ball_light==b.ball_light;
}

bool operator!=(Front::Output a,Front::Output b){ return !(a==b); }

bool operator<(Front::Output a,Front::Output b){
	if(a.motor<b.motor) return 1;
	if(b.motor<a.motor) return 0;
	return a.ball_light<b.ball_light;
}

ostream& operator<<(ostream& o,Front::Output a){
	return o<<"("<<a.motor<<","<<a.ball_light<<")";
}

set<Front::Output> examples(Front::Output*){
	set<Front::Output> r;
	for(auto g:examples((Front::Goal*)nullptr)){
		for(int i=0;i<2;i++){
			r|=Front::Output{g,(bool)i};
		}
	}
	return r;
}

#ifdef FRONT_TEST
#include "formal.h"

int main(){
	Front a;
	tester(a);
}

#endif
