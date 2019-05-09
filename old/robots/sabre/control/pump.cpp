#include "pump.h"
#include<iostream>
#include<cassert>
#include<vector>
#include "../util/util.h"

using namespace std;

Robot_inputs Pump::Input_reader::operator()(Robot_inputs all,Pump::Input in)const{
	all.pump=(in==Input::FULL);
	return all;
}

Pump::Input Pump::Input_reader::operator()(Robot_inputs all)const{
	return all.pump?Input::FULL:Input::NOT_FULL;
}

set<Pump::Status> examples(Pump::Status*){
	return {Pump::Status::FULL,Pump::Status::NOT_FULL};
}

set<Pump::Goal> examples(Pump::Goal*){
	return {Pump::Goal::AUTO,Pump::Goal::OFF};
}

Pump::Status status(Pump::Status_detail a){ return a; }

bool ready(Pump::Status,Pump::Goal){ return 1; }

Pump::Estimator::Estimator():status(Pump::Input::FULL){}

void Pump::Estimator::update(Time,Pump::Input in,Pump::Output){
	status=in;
}

Pump::Status Pump::Estimator::get()const{ return status; }

ostream& operator<<(ostream& o,Pump const& ){
	return o<<"Pump()";
}

ostream& operator<<(ostream& o,Pump::Goal a){
	switch(a){
		#define X(name) case Pump::Goal::name: return o<<""#name;
		X(AUTO)
		X(OFF)
		#undef X
		default: assert(0);
	}
}

ostream& operator<<(ostream& o,Pump::Status a){
	#define X(name) if(a==Pump::Status::name) return o<<""#name;
	X(FULL)
	X(NOT_FULL)
	#undef X
	assert(0);
}

vector<Pump::Status> status_list(){
	vector<Pump::Status> r;
	r|=Pump::Status::FULL;
	r|=Pump::Status::NOT_FULL;
	return r;
}

Maybe<Pump::Status> parse_status(string const& s){
	return parse_enum(status_list(),s);
}

Pump::Output control(Pump::Status,Pump::Goal g){
	if(g==Pump::Goal::OFF) return Pump::Output::OFF;
	return Pump::Output::AUTO;
}

Robot_outputs Pump::Output_applicator::operator()(Robot_outputs r,Pump::Output out)const{
	r.pump_auto=(out==Pump::Output::AUTO);
	return r;
}

Pump::Output Pump::Output_applicator::operator()(Robot_outputs out)const{
	return out.pump_auto?Pump::Output::AUTO:Pump::Output::OFF;
}

bool operator==(Pump::Estimator const& a,Pump::Estimator const& b){ return a.status==b.status; }
bool operator!=(Pump::Estimator const& a,Pump::Estimator const& b){ return a.status!=b.status; }

bool operator==(Pump const& a,Pump const& b){
	return a.estimator==b.estimator;
}

bool operator!=(Pump const& a,Pump const& b){ return !(a==b); }

#ifdef PUMP_TEST
#include "formal.h"

int main(){
	static const vector<Pump::Goal> GOALS{Pump::Goal::AUTO,Pump::Goal::OFF};
	for(auto a:status_list()){
		for(auto g:GOALS){
			cout<<a<<","<<g<<":"<<control(a,g)<<"\n";
		}
		assert(a==parse_status(as_string(a)));
	}

	tester(Pump());
}
#endif
