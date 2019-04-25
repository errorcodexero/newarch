#include "toplevel.h"
#include<iostream>
#include<cassert>
#include<math.h>
#include "../util/util.h"

using namespace std;

/*Toplevel::Toplevel():
	input_reader(this),
	estimator(this),
	output_applicator(this)
{}*/

bool operator==(Toplevel const& a,Toplevel const& b){
	#define X(NAME) if(a.NAME!=b.NAME) return 0;
	X(input_reader) X(estimator) X(output_applicator)
	#undef X
	return 1;
}

bool operator!=(Toplevel const& a,Toplevel const& b){ return !(a==b); }

Toplevel::Input_reader::Input_reader(){
}

Robot_inputs Toplevel::Input_reader::operator()(Robot_inputs all,Input in)const{
	#define X(A,B,C) all=B(all,in.B);
	TOPLEVEL_ITEMS
	#undef X
	return all;
}

Toplevel::Input Toplevel::Input_reader::operator()(Robot_inputs const& all)const{
	return Toplevel::Input{
		#define X(A,B,C) B(all),
		TOPLEVEL_ITEMS
		#undef X
	};
}

ostream& operator<<(ostream& o,Toplevel::Input_reader const&){
	return o<<"Toplevel::Input_reader";
}

bool operator==(Toplevel::Input_reader const&,Toplevel::Input_reader const&){
	return 1;//NOT QUITE RIGHT
}

bool operator!=(Toplevel::Input_reader const& a,Toplevel::Input_reader const& b){
	return !(a==b);
}

Toplevel::Status status(Toplevel::Status_detail const& a){
	/*return Toplevel::Status{
		#define X(A,B,C) status(a.B),
		TOPLEVEL_ITEMS
		#undef X
	};*/
	Toplevel::Status r;
	#define X(A,B,C) r.B=status(a.B);
	TOPLEVEL_ITEMS
	#undef X
	return r;
}

ostream& operator<<(ostream& o,Toplevel const& a){
	o<<"Toplevel(";
	#define X(NAME) o<<a.NAME<<" ";
	X(input_reader) X(estimator) X(output_applicator)
	#undef X
	return o<<")";
}

Robot_outputs Toplevel::Output_applicator::operator()(Robot_outputs r,Toplevel::Output const& a)const{
	#define X(A,B,C) r=B(r,a.B);
	TOPLEVEL_ITEMS
	#undef X
	return r;
}

Toplevel::Output Toplevel::Output_applicator::operator()(Robot_outputs const& a)const{
	Toplevel::Output r;
	#define X(A,B,C) r.B=B(a);
	TOPLEVEL_ITEMS
	#undef X
	return r;
}

bool operator!=(Toplevel::Output_applicator const&,Toplevel::Output_applicator const&){
	//could actually do something more detailed
	return 0;
}

ostream& operator<<(ostream& o,Toplevel::Output_applicator const&){
	return o<<"Toplevel::Output_applicator";
}

Toplevel::Output::Output():
	pump(Pump::Output::AUTO),
	drive(0.0,0.0,false,true),
	grabber(0.0),
	intake(0.0, 0.0),
	lifter({0.0,Lifter::Output::Gearing::HIGH,false}),
	lights(),
	wings(Wings::Output::LOCKED)
{}

bool operator<(Toplevel::Output const& a,Toplevel::Output const& b){
	#define X(A,B,C) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 0;
}

bool operator<(Toplevel::Status const& a,Toplevel::Status const& b){
	#define X(A,B,C) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 0;
}

ostream& operator<<(ostream& o,Toplevel::Output g){
	o<<"Output(";
	#define X(A,B,C) o<<" "#B<<":"<<g.B;
	TOPLEVEL_ITEMS
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Toplevel::Input const& a){
	o<<"Toplevel::Input(";
	#define X(A,B,C) o<<" "#B<<":"<<a.B;
	TOPLEVEL_ITEMS
	#undef X
	return o<<")";
}

Toplevel::Goal::Goal():
	#define X(A,B,C) B(C),
	TOPLEVEL_ITEMS
	#undef X
	dummy()
{}

ostream& operator<<(ostream& o,Toplevel::Goal g){
	o<<"Toplevel::Goal(";
	#define X(A,B,C) o<<" "#B<<":"<<g.B;
	TOPLEVEL_ITEMS
	#undef X
	return o<<")";
}

Toplevel::Status::Status():
	pump(Pump::Status::NOT_FULL),
	drive(*examples((Drivebase::Status*)nullptr).begin()),
	grabber(*examples((Grabber::Status*)nullptr).begin()),
	intake(*examples((Intake::Status*)nullptr).begin()),
	lifter(*examples((Lifter::Status*)nullptr).begin()),
	lights(*examples((Lights::Status*)nullptr).begin()),
	wings(*examples((Wings::Status*)nullptr).begin())
{}
bool operator==(Toplevel::Status a,Toplevel::Status b){
	#define X(A,name,C) if(a.name!=b.name) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 1;
}

bool operator!=(Toplevel::Status a,Toplevel::Status b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Toplevel::Status s){
	o<<"Status(";
	#define X(A,B,C) o<<" "#B<<":"<<s.B;
	TOPLEVEL_ITEMS
	#undef X
	return o<<")";
}

string remove_till_colon(string s){
	unsigned i=0;
	while(i<s.size() && s[i]!=':') i++;
	if(s[i]==':') i++;
	string r=s.substr(i,s.size());
	return r;
}

/*Maybe<Toplevel::Status> parse_status(string const& s){
	vector<string> v=split(inside_parens(s));
	if(v.size()!=7) return Maybe<Status>();
	Status r;
	//yes, there is a better way to do this; it's called a monad. (or exceptions)
	#define X(i) remove_till_colon(v[i])
	{
		Maybe<Pump::Status> m;//=parse_status(X(5));
		if(!m) return Maybe<Status>();
		r.pump=*m;
	}
	#undef X
	return Maybe<Status>(r);
}*/

void Toplevel::Estimator::update(Time time,Input in,Output out){
	#define X(A,B,C) B.update(time,in.B,out.B);
	TOPLEVEL_ITEMS
	#undef X
}

Toplevel::Status_detail Toplevel::Estimator::get()const{
	return Status_detail{
		#define X(A,B,C) B.get(),
		TOPLEVEL_ITEMS
		#undef X
	};
}

void Toplevel::Estimator::out(ostream& o)const{
	o<<"Estimator(";
	/*#define X(A,B,C) o<<parent->B.estimator
	TOPLEVEL_ITEMS
	#undef X*/
	o<<")";
}

bool operator==(Toplevel::Estimator a,Toplevel::Estimator b){
	#define X(A,B,C) if(a.B!=b.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 1;
}

bool operator!=(Toplevel::Estimator a,Toplevel::Estimator b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Toplevel::Estimator e){
	e.out(o);
	return o;
}

bool approx_equal(Toplevel::Estimator a,Toplevel::Estimator b){
	return a.get()==b.get();
}

bool operator==(Toplevel::Output const& a,Toplevel::Output const& b){
	#define X(A,B,C) if(a.B!=b.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 1;
}

bool operator<(Toplevel::Status_detail const& a,Toplevel::Status_detail const& b){
	#define X(A,B,C) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 0;
}

bool operator!=(Toplevel::Status_detail const& a,Toplevel::Status_detail const& b){
	return !(a==b);
}

Toplevel::Status_detail rand(Toplevel::Status_detail *a){
	auto e=examples(a);
	assert(e.size());
	return choose_random(e);
}

ostream& operator<<(ostream& o,Toplevel::Status_detail const& a){
	o<<"Toplevel::Status_detail(";
	#define X(A,B,C) o<<a.B<<" ";
	TOPLEVEL_ITEMS
	#undef X
	return o<<")";
}

bool operator!=(Toplevel::Output const& a,Toplevel::Output const& b){ return !(a==b); }

bool operator==(Toplevel::Status_detail const& a,Toplevel::Status_detail const& b){
	#define X(A,B,C) if(a.B!=b.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 1;
}

Toplevel::Output control(Toplevel::Status_detail status,Toplevel::Goal g){
	Toplevel::Output r;
	#define X(A,B,C) r.B=control(status.B,g.B);
	TOPLEVEL_ITEMS
	#undef X
	return r;
}

bool ready(Toplevel::Status status,Toplevel::Goal g){
	#define X(A,B,C) if(!ready(status.B,g.B)) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 1;
}

vector<string> not_ready(Toplevel::Status status,Toplevel::Goal g){
	vector<string> r;
	#define X(A,name,C) if(!ready(status.name,g.name)) r|=as_string(""#name);
	TOPLEVEL_ITEMS
	#undef X
	return r;
}

bool operator==(Toplevel::Input const& a,Toplevel::Input const& b){
	#define X(A,B,C) if(a.B!=b.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 1;
}

bool operator<(Toplevel::Input const& a,Toplevel::Input const& b){
	#define X(A,B,C) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 0;
}

bool operator<(Toplevel::Goal const& a,Toplevel::Goal const& b){
	#define X(A,B,C) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	TOPLEVEL_ITEMS
	#undef X
	return 0;
}

set<Toplevel::Status_detail> examples(Toplevel::Status_detail*){
	return {Toplevel::Status_detail{
		Pump::Status_detail{Pump::Status::FULL},
		*examples((Drivebase::Status_detail*)0).begin(),
		*examples((Grabber::Status_detail*)0).begin(),
		*examples((Intake::Status_detail*)0).begin(),
		*examples((Lifter::Status_detail*)0).begin(),
		*examples((Lights::Status_detail*)0).begin(),
		*examples((Wings::Status_detail*)0).begin()
	}};
}

set<Toplevel::Goal> examples(Toplevel::Goal*){
	Toplevel::Goal g;
	g.pump=Pump::Goal::OFF;
	return {Toplevel::Goal{},g};
}

set<Toplevel::Status> examples(Toplevel::Status*){ return {Toplevel::Status{}}; }

set<Toplevel::Input> examples(Toplevel::Input*){
	Toplevel::Input a{
		Pump::Input{},
		*examples((Drivebase::Input*)0).begin(),
		*examples((Grabber::Input*)0).begin(),
		*examples((Intake::Input*)0).begin(),
		*examples((Lifter::Input*)0).begin(),
		*examples((Lights::Input*)0).begin(),
		*examples((Wings::Input*)0).begin()
	};
	return {a};
}

set<Toplevel::Output> examples(Toplevel::Output*){
	Toplevel::Output a;
	/*return {Toplevel::Output{
		Pump::Output::AUTO,
	}};*/
	set<Toplevel::Output> r;
	r.insert(a);
	a.pump=Pump::Output::OFF;
	return r;
}

#ifdef TOPLEVEL_TEST
#include "formal.h"
#include "../util/input.h"
#include "../util/output.h"

bool approx_equal(float a, float b){
	return a==b;//TODO: why?
}

bool approx_equal(Toplevel::Status /*a*/,Toplevel::Status /*b*/){
	#define X(name) if(a.name!=b.name) return 0;
	//X(shooter_wheels)
	#undef X
	return 1;//approx_equal(a.orientation,b.orientation);
}

template<typename T>
bool approx_equal(T t,Maybe<T> m){
	if(!m) return 0;
	return approx_equal(t,*m);
}

pair<Digital_in,Digital_in> create_pair(Digital_in*){
	return make_pair(Digital_in::_0,Digital_in::_1);
}

pair<Driver_station_output,Driver_station_output> create_pair(Driver_station_output*){
	Driver_station_output a;
	for(unsigned i = 0; i < Driver_station_output::DIGITAL_OUTPUTS; i++){
		a.digital[i] = 1;
	}
	return make_pair(Driver_station_output{},a);
}

pair<Volt,Volt> create_pair(Volt*){
	return make_pair(0,1);
}

pair<double,double> create_pair(double*){
	return make_pair(0,1);
}

pair<Talon_srx_input,Talon_srx_input> create_pair(Talon_srx_input*){
	Talon_srx_input a;
	a.encoder_position=1;
	a.fwd_limit_switch=1;
	a.rev_limit_switch=1;
	a.a=1;
	a.b=1;
	a.velocity=1;
	return make_pair(Talon_srx_input{},a);
}

pair<Pump_output,Pump_output> create_pair(Pump_output*){
	Pump_output a;
	a.mode = Pump_output::OPEN_LOOP;
	return make_pair(Pump_output{},a);
}

pair<Pump_input,Pump_input> create_pair(Pump_input*){
	Pump_input a;
	a.pressure_switch_triggered = 1;
	return make_pair(Pump_input{},a);
}

pair<Pump::Input,Pump::Input> create_pair(Pump::Input*){
	return make_pair(Pump::Input::FULL,Pump::Input::NOT_FULL);
}

pair<bool,bool> create_pair(bool*){
	return make_pair(0,1);
}

pair<Robot_inputs,Robot_inputs> create_pair(Robot_inputs*){
	pair<Robot_inputs,Robot_inputs> r;
	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		auto p=create_pair((Digital_in*)0);
		r.first.digital_io.in[i]=p.first;
		r.second.digital_io.in[i]=p.second;
	}
	for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
		auto p=create_pair((Volt*)0);
		r.first.analog[i]=p.first;
		r.second.analog[i]=p.second;
	}
	for(unsigned i=0;i<Robot_inputs::TALON_SRX_INPUTS;i++){
		auto p=create_pair((Talon_srx_input*)0);
		r.first.talon_srx[i]=p.first;
		r.second.talon_srx[i]=p.second;
	}
	//current
	for(unsigned i=0;i<Robot_inputs::CURRENT;i++){
		auto p=create_pair((double*)0);
		r.first.current[i]=p.first;
		r.second.current[i]=p.second;
	}
	//pump
	{
		auto p=create_pair((Pump_input*)nullptr);
		r.first.pump=p.first;
		r.second.pump=p.second;
	}
	return r;
}

pair<Digital_out,Digital_out> create_pair(Digital_out*){
	return make_pair(Digital_out::one(),Digital_out::zero());
}

pair<Talon_srx_output,Talon_srx_output> create_pair(Talon_srx_output*){
	Talon_srx_output a;
	a.power_level=0;
	Talon_srx_output b;
	b.power_level=1;
	return make_pair(a,b);
}

pair<Robot_outputs,Robot_outputs> create_pair(Robot_outputs*){
	pair<Robot_outputs,Robot_outputs> r;
	for(unsigned i=0;i<Robot_outputs::PWMS;i++){
		auto p=create_pair((double*)0);
		r.first.pwm[i]=p.first;
		r.second.pwm[i]=p.second;
	}
	for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
		auto p=create_pair((Solenoid_output*)0);
		r.first.solenoid[i]=p.first;
		r.second.solenoid[i]=p.second;
	}
	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		auto p=create_pair((Digital_out*)0);
		r.first.digital_io[i]=p.first;
		r.second.digital_io[i]=p.second;
	}
	for(unsigned i=0;i<Robot_outputs::TALON_SRX_OUTPUTS;i++){
		auto p=create_pair((Talon_srx_output*)0);
		r.first.talon_srx[i]=p.first;
		r.second.talon_srx[i]=p.second;
	}
	{
		auto p = create_pair((Driver_station_output*)nullptr);
		r.first.driver_station = p.first;
		r.second.driver_station = p.second;
	}
	{//pump
		auto p = create_pair((Pump_output*)nullptr);
		r.first.pump=p.first;
		r.second.pump=p.second;
	}
	return r;
}

set<Input> different(Robot_inputs const& a,Robot_inputs const& b){
	set<Input> r;
	#define X(LEN,ITEM,EXP) \
		for(unsigned i=0;i<LEN;i++){\
			if(a.ITEM[i]!=b.ITEM[i]){\
				r|=Input::EXP(i);\
			}\
		}
	X(Robot_outputs::DIGITAL_IOS,digital_io.in,digital_io)
	X(Robot_inputs::ANALOG_INPUTS,analog,analog)
	X(Robot_inputs::TALON_SRX_INPUTS,talon_srx,talon_srx)
	X(Robot_inputs::CURRENT,current,current)
	#undef X
	if(a.pump!=b.pump) r|=Input::pump();
	return r;
}

template<typename T>
set<Input> inputs(T part){
	//need to make this happen both ways
	auto p=create_pair((Robot_inputs*)0);
	auto fwd=different(p.second,part.input_reader(p.second,part.input_reader(p.first)));
	auto rev=different(p.first,part.input_reader(p.first,part.input_reader(p.second)));
	return fwd|rev;
}

set<Output> different(Robot_outputs const& a,Robot_outputs const& b){
	set<Output> r;
	#define X(LEN,ITEM,EXP) \
		for(unsigned i=0;i<Robot_outputs::LEN;i++){\
			if(a.ITEM[i]!=b.ITEM[i]) r|=Output::EXP(i);\
		}
	X(PWMS,pwm,pwm)
	X(SOLENOIDS,solenoid,solenoid)
	X(DIGITAL_IOS,digital_io,digital_io)
	X(TALON_SRX_OUTPUTS,talon_srx,talon_srx)
	X(DRIVER_STATION_DIGITAL_OUTPUTS,driver_station.digital,driver_station)
	#undef X
	if(a.pump!=b.pump) r|=Output::pump();//TODO
	return r;
}

template<typename T>
set<Output> outputs(T part){
	auto p=create_pair((Robot_outputs*)0);
	auto fwd=different(p.second,part.output_applicator(p.second,part.output_applicator(p.first)));
	auto rev=different(p.first,part.output_applicator(p.first,part.output_applicator(p.second)));
	return fwd|rev;
}

int main(){
	Toplevel::Goal g;
	cout<<g<<"\n";
	Toplevel::Status status;
	cout<<status<<"\n";

	/*auto a=random_inputs();
	auto b=random_inputs();*/
	Toplevel topLevel;
	auto d=inputs(topLevel);
	cout<<d<<"\n";

	cout<<"Inputs:\n";

	//Toplevel t;
	#define X(A,B,C) A B;
	TOPLEVEL_ITEMS
	#undef X
	
	map<string,set<Input>> input_map;
	#define X(A,B,C) cout<<""#A<<inputs(B)<<"\n"; input_map[""#A]=inputs(B);
	TOPLEVEL_ITEMS
	#undef X

	auto find_input=[=](Input a)->string{
		set<string> found;
		for(auto p:input_map){
			if(p.second.count(a)){
				found|=p.first;
			}
		}
		switch(found.size()){
			case 0: return "UNUSED";
			case 1: return *found.begin();
			default:{
				stringstream ss;
				for(auto elem:found) ss<<"x:"<<elem<<" ";
				return ss.str();
			}// assert(0);
		}
	};

	for(auto a:inputs()){
		cout<<a<<"\t"<<find_input(a)<<"\n";
	}

	cout<<"Outputs:\n";
	auto f=outputs(topLevel);
	cout<<f<<"\n";

	map<string,set<Output>> output_map;
	#define X(A,B,C) cout<<""#A<<outputs(B)<<"\n"; output_map[""#A]=outputs(B);
	TOPLEVEL_ITEMS
	#undef X

	const string CONFLICT_MESSAGE = "----CONFLICT!----";

	auto find_output=[=](Output a)->string{
		set<string> found;
		for(auto p:output_map){
			if(p.second.count(a)){
				found|=p.first;
			}
		}
		switch(found.size()){
			case 0: return "UNUSED";
			case 1: return *found.begin();
			default:
				return CONFLICT_MESSAGE + as_string(found) + " ------ Check IO's, assigning the same output from multiple parts - EXITING ------";
		}
	};
	for(auto a:outputs()){
		cout<<a<<"\t"<<find_output(a)<<"\n";
		if(find_output(a).find(CONFLICT_MESSAGE) != string::npos){//fails the test if there are conflicts
			exit(1);//change this if you want to allow conflicting assignments
		}
	}

	//tester(Toplevel{});
	//Toplevel::Control control;
	/*Toplevel::Estimator est;
	cout<<est<<"\n";
	cout<<est.estimate()<<"\n";
	Pump::Status ps=Pump::Status::FULL;
	est.update(0,Toplevel::Input(),Toplevel::Output());
	est.update(10,0,Toplevel::Output(),ps,0);
	cout<<est.estimate()<<"\n";*/
	/*
	if we choose one of the modes and use all the built-in controls then we should after some time get to a status where we're ready.  
	*/
	//assert(approx_equal(status,parse_status(as_string(status))));
}
#endif
