#include "driver_station_interface.h"
#include<iostream>
#include<sstream>
#include<stdlib.h>
#include "util.h"

using namespace std;

Driver_station_input::Driver_station_input():digital(0){
	for(unsigned i=0;i<ANALOG_INPUTS;i++) analog[i]=0;
}

Maybe<Driver_station_input> Driver_station_input::parse(string const& s){
	unsigned i;
	for(i=0;i<s.size() && s[i]!=':';i++) ;
	string s2=s.substr(i+1,s.size());
	vector<string> vs=split(s2);
	if(vs.size()!=9) return Maybe<Driver_station_input>();
	Driver_station_input r;
	for(unsigned i=0;i<ANALOG_INPUTS;i++){
		r.analog[i]=atof(vs[i]);
	}
	vector<string> t=split(vs.at(vs.size()-1),':');
	if(t.size()!=2){
		return Maybe<Driver_station_input>();
	}
	if(t[1].size()!=1+Driver_station_input::DIGITAL_INPUTS){
		//cout<<"wrong # of digitals\n";
		return Maybe<Driver_station_input>();
	}

	//cout<<"dig:"<<t[1];
	for(unsigned i=0;i<Driver_station_input::DIGITAL_INPUTS;i++){
		r.digital[i]=t[1][i]-'0';
	}

	return Maybe<Driver_station_input>(r);
}

Driver_station_input Driver_station_input::rand(){
	Driver_station_input r;
	for(unsigned i=0;i<ANALOG_INPUTS;i++){
		r.analog[i]=(::rand()%3300)/1000.0;
	}
	for(unsigned i=0;i<DIGITAL_INPUTS;i++){
		r.digital[i]=::rand()%2;
	}
	return r;
}

bool operator<(Driver_station_input a,Driver_station_input b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(analog) X(digital)
	#undef X
	return 0;
}

bool operator==(Driver_station_input a,Driver_station_input b){
	return a.analog==b.analog && a.digital==b.digital;
}

bool operator!=(Driver_station_input a,Driver_station_input b){
	return !(a==b);
}

float round(float f){
	return ((int)(f*10+.5))/10.0;
}

ostream& operator<<(ostream& o,Driver_station_input a){
	//o.precision(2);
	o<<"(";
	o<<"analog:";
	for(unsigned i=0;i<Driver_station_input::ANALOG_INPUTS;i++){
		o<<round(a.analog[i])<<" ";
	}
	o<<" digital:";
	for(unsigned i=0;i<Driver_station_input::DIGITAL_INPUTS;i++){
		o<<a.digital[i];
	}
	return o<<")";
}

bool operator==(Driver_station_output a,Driver_station_output b){
	for(unsigned i=0;i<Driver_station_output::DIGITAL_OUTPUTS;i++){
		if(a.digital[i]!=b.digital[i]){
			return 0;
		}
	}
	return 1;
}

bool operator!=(Driver_station_output a,Driver_station_output b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Driver_station_output a){
	o<<"(";
	o<<"digital:";
	for(unsigned i=0;i<Driver_station_output::DIGITAL_OUTPUTS;i++){
		o<<a.digital[i];
	}
	return o<<")";
}

#ifdef DRIVER_STATION_INTERFACE_TEST
int main(){
	Driver_station_input a;
	Driver_station_output b;
	//cout<<format_for_lcd("this\nthat dsf ljkdskjl sdjlf kljsdfkjl sklj djkl v dkljk dkljfsd ljksdljk");

	//Driver_station_input a;
	string s=as_string(a);
	cout<<"ss="<<s<<"\n";
	auto d=Driver_station_input::parse(as_string(a));
	cout<<"out="<<d<<"\n";
	assert(d==a);
	//cout<<a<<"\n";
	for(unsigned i=0;i<3;i++){
		a=Driver_station_input::rand();
		Maybe<Driver_station_input> m=Driver_station_input::parse(as_string(a));
		assert(as_string(a)==as_string(m));
	}
}
#endif
