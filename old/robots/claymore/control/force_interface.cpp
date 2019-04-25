#include "force_interface.h"
#include<iostream>
#include "../util/util.h"

using namespace std;

Force_interface::Force_interface(){
	for(unsigned i=0;i<LEN;i++) recent[i]=B0;
}

void Force_interface::update(bool pwm,bool solenoid,bool relay,bool dio,bool b0,bool b1){
	bool b[BUTTONS]={pwm,solenoid,relay,dio,b0,b1};
	for(unsigned i=0;i<BUTTONS;i++){
		if(button[i](b[i])){
			update((Force_interface::Button)i);
		}
	}
}

int bit_value(Force_interface::Button b){
	switch(b){
		case Force_interface::B0: return 0;
		case Force_interface::B1: return 1;
		default: return -1;
	}
}

bool start(Force_interface::Button b){
	return !(b==Force_interface::B0 || b==Force_interface::B1);
}

int Force_interface::pwm(unsigned location,int value){
	if(location==15){
		int r=0;
		for(unsigned i=0;i<Robot_outputs::PWMS;i++){
			r|=pwm(i,value);
		}
		return r;
	}
	
	if(location>Robot_outputs::PWMS){
		return 1;
	}
	/*
	values:
	0=force to 0
	1=force to 1
	2=do not force
	3=force to -1
	(this makes sense if you think of it as a 2-bit signed number)
	*/
	if(value==2){
		force.pwm[location]=0;
	}else{
		force.pwm[location]=1;
		double v=0;
		double x=.5;
		if(value==1) v=x;
		if(value==3) v=-x;
		force.values.pwm[location] = v;
	}
	return 0;
}

int Force_interface::solenoid(unsigned location,int value){
	if(location==15){
		int r=0;
		for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
			r|=solenoid(i,value);
		}
		return r;
	}
	if(location>=Robot_outputs::SOLENOIDS) return 1;
	/*
	values:
	0=force to 0
	1=force to 1
	2=do not force
	3=error
	*/
	if(value==2){
		force.solenoid[location]=0;
	}else{
		force.solenoid[location]=1;
		if(value==3) return 2;
		force.values.solenoid[location]=value;
	}
	return 0;
}

int Force_interface::relay(unsigned location,int value){
	assert((value&0x3)==value);

	if(location==15){
		int r=0;
		for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
			r|=relay(i,value);
		}
		return r;
	}
	if(location>=Robot_outputs::RELAYS) return 1;
	/*
	values:
	0=force to 0
	1=force to fwd
	2=do not force
	3=force to reverse
	*/
	if(value==2){
		force.relay[location]=0;
	}else{
		force.relay[location]=1;
		force.values.relay[location]=[=](){
			if(value==0) return Relay_output::_00;
			if(value==1) return Relay_output::_10;
			if(value==3) return Relay_output::_01;
			assert(0);
		}();
	}
	return 0;
}

int Force_interface::dio(unsigned loc,int value){
	if(loc==15){
		int r=0;
		for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
			r|=dio(i,value);
		}
		return r;
	}
	if(loc>=Robot_outputs::DIGITAL_IOS) return 1;
	/*
	values;
	0=force to 0
	1=force to 1
	2=do not force
	3=force to input
	*/
	if(value==2){
		force.digital_io[loc]=0;
	}else{
		force.digital_io[loc]=1;
		Digital_out v;
		if(value==0) v=Digital_out::zero();
		if(value==1) v=Digital_out::one();
		if(value==3) v=Digital_out::input();
		force.values.digital_io[loc]=v;
	}
	return 0;
}

void Force_interface::update(Button b){
	for(unsigned i=LEN-1;i;i--){
		recent[i]=recent[i-1];
	}
	recent[0]=b;

	/*
	Output possibilities:(#channels)-(#levels)
	pwm-10-3 (just doing +/0/-)
	solenoid-8-2
	relay 8-4
	dio 14-3
	+1 for all
	+1 for don't force
	bits:
	pwm:4-2
	solenoid:4-2
	relay:4-3 or could just make it so there isn't a way to say both on and use that to not force.
	dio:4-2
	*/
	if(start(recent[LEN-1])){
		unsigned code=0;
		for(unsigned i=0;i<LEN-1;i++){
			int b=bit_value(recent[i]);
			if(b==-1){
				cout<<"extr\n";
				return;
			}
			code|=(b<<i);
		}
		unsigned location=code>>VALUE_BITS;
		unsigned value=code&0x3;
		int r=0;
		switch(recent[LEN-1]){
			case PWM:
				r=pwm(location,value);
				break;
			case SOLENOID:
				r=solenoid(location,value);
				break;
			case RELAY:
				r=relay(location,value);
				break;
			case DIO:
				r=dio(location,value);
				break;
			default:
				cerr<<"Force_interface: Internal error.\n";
				break;
		}
		if(r&1){
			cerr<<"Location "<<location<<" not in range.\n";
		}
	}
}

Robot_outputs Force_interface::operator()(Robot_outputs a)const{
	return force(a);
}

Force const& Force_interface::get()const{ return force; }

bool operator==(Force_interface a,Force_interface b){
	if(a.force!=b.force) return 0;
	for(unsigned i=0;i<Force_interface::BUTTONS;i++){
		if(a.button[i]!=b.button[i]){
			return 0;
		}
	}
	for(unsigned i=0;i<Force_interface::LEN;i++){
		if(a.recent[i]!=b.recent[i]){
			return 0;
		}
	}
	return 1;
}

bool operator!=(Force_interface a,Force_interface b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Force_interface::Button b){
	switch(b){
		#define X(name) case Force_interface::name: return o<<""#name;
		X(PWM)
		X(SOLENOID)
		X(RELAY)
		X(DIO)
		X(B0)
		X(B1)
		#undef X
		default: return o<<"ERROR";
	}
}

void button_short(ostream& o,Force_interface::Button b){
	switch(b){
		case Force_interface::PWM:
			o<<'p';
			break;
		case Force_interface::SOLENOID:
			o<<'s';
			break;
		case Force_interface::RELAY:
			o<<'r';
			break;
		case Force_interface::DIO:
			o<<'d';
			break;
		case Force_interface::B0:
			o<<'0';
			break;
		case Force_interface::B1:
			o<<'1';
			break;
		default:
			o<<'?';
	}
}

std::ostream& operator<<(std::ostream& o,Force_interface const& a){
	o<<"Force_interface(";
	o<<a.force<<" ";
	for(unsigned i=0;i<Force_interface::BUTTONS;i++){
		o<<a.button[i];
	}
	o<<" recent(";
	for(unsigned i=0;i<Force_interface::LEN;i++){
		//o<<a.recent[i]<<" ";
		button_short(o,a.recent[i]);
	}
	o<<")";
	return o<<")";
}

#ifdef FORCE_INTERFACE_TEST
#include<cassert>

int main(){
	Force_interface f;
	cout<<f<<"\n";
	#define F(a,b,c) f.update(a,0,0,0,b,c);
	#define W(a,b,c) F(0,0,0) F(a,b,c)
	W(1,1,1);
	cout<<f<<"\n";
	F(0,1,1);
	W(0,1,1);
	W(0,1,1);
	cout<<f<<"\n";
	F(0,1,0);
	cout<<f<<"\n";
	assert(f.get().pwm[5]);
	assert(f.get().values.pwm[5]>0);
	cout<<f(Robot_outputs())<<"\n";
	
	W(0,0,0)
	f.update(0,1,0,0,0,0);
	W(0,1,0)
	W(0,1,0)
	W(0,1,0)
	W(0,1,0)
	W(0,1,0)
	W(0,0,1)
	cout<<f<<"\n";
	
	W(0,0,0)
	f.update(0,0,1,0,0,0);
	W(0,0,1)
	W(0,0,1)
	W(0,0,1)
	W(0,0,1)
	W(0,0,1)
	W(0,0,1)
	//should set all relays to reverse mode.
	cout<<f<<"\n";
}
#endif
