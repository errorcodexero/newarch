#include "main.h"
#include <iostream>
#include <sstream>
#include <cassert>
#include <math.h>
#include <stdlib.h>
#include "toplevel.h"
#include "../util/util.h"
#include "../input/util.h"
#include <vector>
#include "../executive/teleop.h"

using namespace std;

static int print_count=0;

Main::Main(Executive start):
	mode(start),
	autonomous_start(0)
{}

Main::Main():Main(Executive{Teleop()}){}//start in teleop because it is safer (it starts autonomous if it needs to anyway)

Robot_outputs Main::operator()(const Robot_inputs in,ostream& /*print_stream*/){
	
	print_count++;
	static const unsigned MAIN_JOYSTICK_PORT = 0, GUNNER_JOYSTICK_PORT = 1;

	perf.update(in.now);

	Joystick_data driver_joystick=in.joystick[MAIN_JOYSTICK_PORT];
	Panel panel = interpret_oi(in.joystick[Panel::PORT]);
	if(!panel.in_use){
		panel = interpret_gamepad(in.joystick[GUNNER_JOYSTICK_PORT]);
	}

	Toplevel::Status_detail status = toplevel.estimator.get();
	
	bool autonomous_start_now=autonomous_start(in.robot_mode.autonomous && in.robot_mode.enabled);
	Executive next = mode.next_mode(Next_mode_info{in.robot_mode.autonomous,autonomous_start_now,status,since_switch.elapsed(),panel,in});
	since_switch.update(in.now,mode != next);
	mode=next;
		
	Toplevel::Goal goals = mode.run(Run_info{in,driver_joystick,panel,status});
	Toplevel::Output r_out = control(status,goals); 
	Robot_outputs r = toplevel.output_applicator(Robot_outputs{},r_out);
	
	force.update(
		driver_joystick.button[Gamepad_button::A],
		driver_joystick.button[Gamepad_button::LB],
		driver_joystick.button[Gamepad_button::RB],
		driver_joystick.button[Gamepad_button::BACK],
		driver_joystick.button[Gamepad_button::B],
		driver_joystick.button[Gamepad_button::X]
	);
	

	r=force(r);
	Toplevel::Input input = toplevel.input_reader(in);

	toplevel.estimator.update(
		in.now,
		input,
		toplevel.output_applicator(r)
	);
	
	log(in,status,r);

	#ifdef PRINT_OUTS
	if(in.ds_info.connected && (print_count % 10) == 0){
		//cout<<"mode: "<<mode<<"\n\n";
		//cout<<"panel:"<<panel<<"\n";
	}
	#endif

	return r;
}

bool operator==(Main const& a,Main const& b){
	return a.force==b.force && 
		a.perf==b.perf && 
		a.toplevel==b.toplevel && 
		a.since_switch==b.since_switch && 
		a.autonomous_start==b.autonomous_start;
}

bool operator!=(Main const& a,Main const& b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Main const& m){
	o<<"Main(";
	o<<" "<<m.force;
	o<<" "<<m.perf;
	o<<" "<<m.toplevel;
	o<<" "<<m.since_switch;
	o<<" "<<m.mode;
	return o<<")";
}

bool approx_equal(Main a,Main b){
	if(a.force!=b.force) return 0;
	if(a.toplevel!=b.toplevel) return 0;
	return 1;
}

#ifdef MAIN_TEST
#include<fstream>
#include "monitor.h"

template<typename T>
vector<T> uniq(vector<T> v){
	vector<T> r;
	for(auto a:v){
		if(!(r.size()&&r[r.size()-1]==a)){
			r|=a;
		}
	}
	return r;
}

int main(){}

#endif
