#include "jag_interface.h"
#include <iostream>
#include <cmath>
#include "util.h"

using namespace std;

PID_coefficients::PID_coefficients():p(.3),i(.003),d(0){}

bool operator==(PID_coefficients a,PID_coefficients b){
	#define X(n) if(a.n!=b.n) return 0;
	X(p) X(i) X(d)
	#undef X
	return 1;
}

bool operator!=(PID_coefficients a,PID_coefficients b){
	return !(a==b);
}

bool operator<(PID_coefficients const& a,PID_coefficients const& b){
	#define X(name) if(a.name<b.name) return 1; if(b.name<a.name) return 0;
	X(p) X(i) X(d)
	#undef X
	return 0;
}

ostream& operator<<(ostream& o,PID_coefficients p){
	return o<<"PID_constants("<<p.p<<" "<<p.i<<" "<<p.d<<")";
}

Jaguar_output::Jaguar_output():speed(0),voltage(0),controlSpeed(0){}

Jaguar_output Jaguar_output::speedOut(double a){
	Jaguar_output j;
	j.controlSpeed = true;
	j.speed = a;
	return j;
}

Jaguar_output Jaguar_output::voltageOut(double a){
	Jaguar_output j;
	j.controlSpeed = false;
	j.voltage = a;
	return j;
}

Maybe<Jaguar_output> Jaguar_output::parse(std::string const& ){
	assert(0);
	#if 0
	Jaguar_output r;
	string s2=inside_parens(s);
	vector<string> v=split(s2,'=');
	if(v[0]=="speed"){
		vector<string> v2=split(s2);
		r=speedOut(0);
		#define X(name,index) r.name=atof(split(v2.at(index),'=').at(1));
		X(speed,0)
		X(pid.p,1)
		X(pid.i,2)
		X(pid.d,3)
		#undef X
	}else if(v[0]=="voltage"){
		if(v.size()!=2) return Maybe<Jaguar_output>();
		r=voltageOut(atof(v[1]));
	}else{
		return Maybe<Jaguar_output>();
	}
	return Maybe<Jaguar_output>(r);
	#endif
}

bool operator==(Jaguar_output a,Jaguar_output b){
	#define X(n) if(a.n!=b.n) return 0;
	X(pid)
	#undef X
	return a.speed==b.speed && a.voltage==b.voltage && a.controlSpeed==b.controlSpeed;
}

bool operator!=(Jaguar_output a,Jaguar_output b){
	return !(a==b);
}

bool operator<(Jaguar_output a,Jaguar_output b){
	#define X(name) if(a.name<b.name) return 1; if(b.name<a.name) return 0;
	X(pid)
	X(speed)
	X(voltage)
	X(controlSpeed)
	#undef X
	return 0;
}

ostream& operator<<(ostream& o,Jaguar_output a){
	o<<"Jaguar_output(";
	if(a.controlSpeed){
		o<<"speed="<<a.speed;
		#define X(n) o<<" "#n<<"="<<a.n;
		X(pid.p)
		X(pid.i)
		X(pid.d)
		#undef X
	}else{
		o<<"voltage="<<a.voltage;
	}
	return o<<")";
}

Jaguar_input::Jaguar_input():speed(0),current(0){}

Maybe<Jaguar_input> Jaguar_input::parse(string const& s){
	cout<<"s="<<s<<"\n";
	vector<string> v=split(inside_parens(s));
	if(v.size()!=2) return Maybe<Jaguar_input>();
	cout<<v<<"\n";
	Jaguar_input r;
	{
		vector<string> a=split(v[0],':');
		if(a.size()!=2) return Maybe<Jaguar_input>();
		r.speed=atof(a[1]);
	}
	{
		vector<string> a=split(v[1],':');
		if(a.size()!=2) return Maybe<Jaguar_input>();
		r.current=atof(a[1]);
	}
	return Maybe<Jaguar_input>(r);
}

bool operator==(Jaguar_input a,Jaguar_input b){
	return a.speed==b.speed && a.current==b.current;
}

bool operator!=(Jaguar_input a,Jaguar_input b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Jaguar_input a){
	o<<"Jaguar_input(";
	o<<"speed:"<<a.speed;
	o<<" current:"<<a.current;
	return o<<")";
}

bool approx_equal(Jaguar_input a,Jaguar_input b){
	return a.current==b.current && fabs(a.speed-b.speed)<140;//this threshold is totally arbitrary.
}

#ifdef JAG_INTERFACE_TEST
#include "util.h"

int main(){
	Jaguar_input a;
	cout<<a<<"\n";
	Jaguar_output b;
	cout<<b<<"\n";
	a.current=3;
	a.speed=2.33;
	/*assert(a==Jaguar_input::parse(as_string(a)));
	b=Jaguar_output::speedOut(3);
	b.pid.p=.444;
	assert(b==Jaguar_output::parse(as_string(b)));
	cout<<PID_coefficients()<<"\n";*/
}
#endif
