#include "monitor.h"
#include "main.h"
#include "../util/interface.h"

using namespace std;

void print_diff(ostream& o,unsigned char a,unsigned char b){
	print_diff(o,(int)a,(int)b);
}

void print_diff(ostream& o,Toplevel::Status &a,Toplevel::Status const& b){
	#define X(name) print_diff(o,""#name ": ",a.name,b.name);
	//print_diff_approx(o,a.shooter_wheels,b.shooter_wheels);
	X(pump)
	//X(orientation)
	//print_diff_approx(o,a.orientation,b.orientation);
	#undef X
}

string Monitor<Toplevel::Estimator>::update(Toplevel::Estimator b){
	stringstream o;
	print_diff(o,status,::status(b.get()));
	return o.str();
}

string Monitor<Main>::update(Main b){
	stringstream o;
	#define X(name) print_diff(o,""#name ": ",data.name,b.name);
	#define Y(name) print_diff(o,data.name,b.name);
	X(force)
	//Y(gyro);
	//Y(est)
	//o<<est.update(b.est);
	#undef Y
	#undef X
	return o.str();
}

void print_diff(ostream& o,Driver_station_output &a,Driver_station_output const& b){
	#define X(name) print_diff(o,a.name,b.name);
	//X(lcd) skipping this for now since it really clutters the prinout right now since it shows shooter wheel speeds
	for(unsigned i=0;i<Driver_station_output::DIGITAL_OUTPUTS;i++){
		if(a.digital[i]!=b.digital[i]){
			o<<"Driver_station_output::digital["<<i<<"]:"<<a.digital[i]<<"->"<<b.digital[i]<<"\n";
			a.digital[i]=b.digital[i];
		}
	}
	#undef X
}

void print_diff(ostream& o,Robot_outputs &a,Robot_outputs const& b){
	for(unsigned i=0;i<Robot_outputs::PWMS;i++){
		if(a.pwm[i]!=b.pwm[i]){
			o<<"pwm"<<i<<" "<<a.pwm[i]<<"->"<<b.pwm[i]<<"\n";
			a.pwm[i]=b.pwm[i];
		}
	}
	for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
		if(a.solenoid[i]!=b.solenoid[i]){
			o<<"solenoid"<<i<<" "<<a.solenoid[i]<<"->"<<b.solenoid[i]<<"\n";
			a.solenoid[i]=b.solenoid[i];
		}
	}
	for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
		if(a.relay[i]!=b.relay[i]){
			o<<"relay"<<i<<" "<<a.relay[i]<<"->"<<b.relay[i]<<"\n";
			a.relay[i]=b.relay[i];
		}
	}
	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		if(a.digital_io[i]!=b.digital_io[i]){
			o<<"digital_io"<<i<<" "<<a.digital_io[i]<<"->"<<b.digital_io[i]<<"\n";
			a.digital_io[i]=b.digital_io[i];
		}
	}
	print_diff(o,a.driver_station,b.driver_station);
}

void print_diff(ostream& o,Robot_inputs &a,Robot_inputs const& b){
	#define X(name) print_diff(o,""#name ": ",a.name,b.name);
	X(robot_mode)
	//X(now)
	for(unsigned i=0;i<Robot_inputs::JOYSTICKS;i++){
		X(joystick[i])
	}
	/*for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		X(digital_io[i])
	}*/
	X(digital_io)
	for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
		X(analog[i])
	}
	X(driver_station)
	#undef X
}

#ifdef MONITOR_TEST
int main(){}
#endif
