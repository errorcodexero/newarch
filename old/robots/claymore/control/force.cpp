#include "force.h"
#include<iostream>
#include<cassert>

using namespace std;

Force::Force(){
	for(unsigned i=0;i<Robot_outputs::PWMS;i++){
		pwm[i]=0;
	}
	for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
		solenoid[i]=0;
	}
	for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
		relay[i]=0;
	}
	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		digital_io[i]=0;
	}
}

Robot_outputs Force::operator()(Robot_outputs r)const{
	for(unsigned i=0;i<Robot_outputs::PWMS;i++){
		if(pwm[i]){
			r.pwm[i]=values.pwm[i];
		}
	}
	for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
		if(solenoid[i]) r.solenoid[i]=values.solenoid[i];
	}
	for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
		if(relay[i]) r.relay[i]=values.relay[i];
	}
	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		if(digital_io[i]) r.digital_io[i]=values.digital_io[i];
	}
	return r;
}

bool operator==(Force a,Force b){
	if(a.values!=b.values) return 0;
	for(unsigned i=0;i<Robot_outputs::PWMS;i++){
		if(a.pwm[i]!=b.pwm[i]){
			return 0;
		}
	}
	for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
		if(a.solenoid[i]!=b.solenoid[i]){
			return 0;
		}
	}
	for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
		if(a.relay[i]!=b.relay[i]){
			return 0;
		}
	}
	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		if(a.digital_io[i]!=b.digital_io[i]){
			return 0;
		}
	}
	return 1;
}

bool operator!=(Force a,Force b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Force f){
	o<<"Force( ";
	for(unsigned i=0;i<Robot_outputs::PWMS;i++){
		if(f.pwm[i]){
			o<<"pwm"<<i<<"="<<(int)f.values.pwm[i]<<" ";
		}
	}
	for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
		if(f.solenoid[i]){
			o<<"solenoid"<<i<<"="<<f.values.solenoid[i]<<" ";
		}
	}
	for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
		if(f.relay[i]){
			o<<"relay"<<i<<"="<<f.values.relay[i]<<" ";
		}
	}
	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		if(f.digital_io[i]){
			o<<"dio"<<i<<"="<<f.values.digital_io[i]<<" ";
		}
	}
	return o<<")";
}
