#include "interface.h"
#include <iostream>
#include <cassert>
#include <stdlib.h>
#include<iomanip>
#include "util.h"

using namespace std;

PID_values::PID_values():p(.5),i(0.002),d(.25),f(0){}//from testing with shooter 2016
//.5, .002 , .25 , 0 //Logan's
//.5 , -.005 , 0, 0 //Marshall's
//1 , .001 , 1 , 0 //Eric's

#define PID_ITEMS X(p) X(i) X(d) X(f)

bool operator==(PID_values const& a,PID_values const& b){
	#define X(NAME) if(a.NAME!=b.NAME) return 0;
	PID_ITEMS
	#undef X
	return 1;
}

bool operator<(PID_values const& a,PID_values const& b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	PID_ITEMS
	#undef X
	return 0;
}

Digital_out::Digital_out():type_(Type::INPUT){}

Digital_out::Type Digital_out::type()const{ return type_; }

int Digital_out::encoder_index()const{
	assert(type_==Type::ENCODER);
	return encoder_index_;
}

bool Digital_out::input_a()const{
	assert(type_==Type::ENCODER);
	return input_a_;
}

Digital_out Digital_out::input(){
	Digital_out r;
	r.type_=Digital_out::Type::INPUT;
	return r;
}

Digital_out Digital_out::one(){
	Digital_out r;
	r.type_=Digital_out::Type::_1;
	return r;
}

Digital_out Digital_out::zero(){
	Digital_out r;
	r.type_=Digital_out::Type::_0;
	return r;
}

Digital_out Digital_out::encoder(int encoder_index,bool input_a){
	Digital_out r;
	r.type_=Digital_out::Type::ENCODER;
	r.encoder_index_=encoder_index;
	r.input_a_=input_a;
	return r;
}

Panel_output::Panel_output(int p, bool v) {
	port = p;
	value = v;
}

bool operator==(Panel_output a,Panel_output b){
	return a.port==b.port && a.value==b.value;
}

std::ostream& operator<<(std::ostream& o,Digital_out a){
	switch(a.type()){
		case Digital_out::Type::INPUT:
			o<<"INPUT";
			break;
		case Digital_out::Type::_0:
			o<<"0";
			break;
		case Digital_out::Type::_1:
			o<<"1";
			break;
		case Digital_out::Type::ENCODER:
			o<<"encoder_index:"<<a.encoder_index();
			o<<"input_a:"<<a.input_a();
			break;
		default:
			o<<"?";
	}
	return o;
}

std::ostream& operator<<(std::ostream& o, PID_values const& a){
	return o<<"(p:"<<a.p<<" i:"<<a.i<<" d:"<<a.d<<" f:"<<a.f<<")";
}

std::ostream& operator<<(std::ostream& o, Talon_srx_input in){
	o<<"(encoder_position:"<<in.encoder_position<<" velocity:"<<in.velocity<<" fwd_limit_switch:"<<in.fwd_limit_switch<<" rev_limit_switch:"<<in.rev_limit_switch<<" a:"<<in.a<<" b:"<<in.b;
	return o<<")";
}

std::ostream& operator<<(std::ostream& o, Talon_srx_output::Mode a){
	if(a==Talon_srx_output::Mode::VOLTAGE) o<<"VOLTAGE";
	else if(a==Talon_srx_output::Mode::SPEED) o<<"SPEED";
	return o;
}

std::ostream& operator<<(std::ostream& o, Talon_srx_output a){
	o<<"(mode: "<<a.mode;
	o<<" pid:"<<a.pid;
	if(a.mode==Talon_srx_output::Mode::VOLTAGE) o<<" power_level:"<<a.power_level;
	else if(a.mode==Talon_srx_output::Mode::SPEED) o<<" speed:"<<a.speed;
	return o<<")";
}

std::ostream& operator<<(std::ostream& o, Panel_output in) {
	o<<"(port:"<<in.port<<" value:"<<in.value;
	return o<<")";
}

void terse(ostream& o, Digital_out d){
	//o<<d;
	switch(d.type()){
		case Digital_out::Type::INPUT:
			o<<'i';
			break;
		case Digital_out::Type::_0:
			o<<'0';
			break;
		case Digital_out::Type::_1:
			o<<'1';
			break;
		case Digital_out::Type::ENCODER:
			o<<'e';
			break;
		default:
			o<<'?';
			break;
	}
}

std::ostream& operator<<(std::ostream& o,Relay_output a){
	switch(a){
		#define X(name) case Relay_output::name: return o<<""#name;
		X(_00)
		X(_01)
		X(_10)
		X(_11)
		#undef X
		default: return o<<"?";
	}
}

void terse(ostream& o,Relay_output a){
	switch(a){
		case Relay_output::_00:
			o<<'0';
			break;
		case Relay_output::_01:
			o<<'R';
			break;
		case Relay_output::_10:
			o<<'F';
			break;
		case Relay_output::_11:
			o<<"+";
			break;
		default:
			o<<"?";
	}
}

vector<Relay_output> relay_outputs(){
	vector<Relay_output> r;
	r|=Relay_output::_00;
	r|=Relay_output::_01;
	r|=Relay_output::_10;
	r|=Relay_output::_11;
	return r;
}

Maybe<Relay_output> parse_relay_output(string s){
	//could have this try the terse encodings as well.
	return parse_enum<Relay_output>(relay_outputs(),s);
}

bool operator==(Talon_srx_input a,Talon_srx_input b){
	return a.encoder_position==b.encoder_position && a.fwd_limit_switch==b.fwd_limit_switch && a.rev_limit_switch==b.rev_limit_switch && a.current==b.current;
}

bool operator!=(Talon_srx_input a,Talon_srx_input b){
	return !(a==b);
}

bool operator<(Talon_srx_input a, Talon_srx_input b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(encoder_position) X(fwd_limit_switch) X(rev_limit_switch) X(current)
	#undef X
	return 0;
}

bool operator==(Talon_srx_output a,Talon_srx_output b){
	if(a.mode!=b.mode) return false;
	if(a.mode==Talon_srx_output::Mode::VOLTAGE) return a.power_level==b.power_level;
	if(a.mode==Talon_srx_output::Mode::SPEED) return a.speed==b.speed;
	return false;
}

bool operator!=(Talon_srx_output a,Talon_srx_output b){
	return !(a==b);
}

bool operator<(Talon_srx_output a, Talon_srx_output b){
	if(a.mode!=b.mode) return a.mode<b.mode;
	if(a.mode==Talon_srx_output::Mode::VOLTAGE) return a.power_level<b.power_level;
	if(a.mode==Talon_srx_output::Mode::SPEED) return a.speed<b.speed;
	return false;
}

bool operator==(Digital_out a, Digital_out b){
	if(a.type()!=b.type()) return 0;
	if(a.type()==Digital_out::Type::ENCODER){
		return a.encoder_index()==b.encoder_index() && a.input_a()==b.input_a();
	}
	return 1;
}

bool operator!=(Digital_out a, Digital_out b){
	return !(a==b);
}

bool operator<(Digital_out a, Digital_out b){
	if(a.type()<b.type()) return 1;
	if(b.type()<a.type()) return 0;
	if(a.type()==Digital_out::Type::ENCODER){
		if(a.encoder_index()<b.encoder_index()) return 1;
		if(b.encoder_index()<a.encoder_index()) return 0;
		return a.input_a()<b.input_a();
	}
	return 0;
}

Robot_outputs::Robot_outputs():pump_auto(1){
	for(unsigned i=0;i<PWMS;i++){
		pwm[i]=0;
	}
	for(unsigned i=0;i<SOLENOIDS;i++){
		solenoid[i]=0;
	}
	for(unsigned i=0;i<RELAYS;i++){
		relay[i]=Relay_output::_00;
	}
}

bool operator==(Robot_outputs a,Robot_outputs b){
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
	for(unsigned int i=0;i<Robot_outputs::TALON_SRX_OUTPUTS; i++){
		if(a.talon_srx[i]!=b.talon_srx[i]){
			return 0;
		}
	}
	/*for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
		if(a.jaguar[i]!=b.jaguar[i]){
			return 0;
		}
	}*/
	return a.driver_station==b.driver_station && a.pump_auto==b.pump_auto;
}

bool operator!=(Robot_outputs a,Robot_outputs b){
	return !(a==b);
}

bool operator<(Robot_outputs a,Robot_outputs b){
	for(unsigned i=0;i<Robot_outputs::PWMS;i++){
		if(a.pwm[i]<b.pwm[i]) return 1;
		if(b.pwm[i]<a.pwm[i]) return 0;
	}

	for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
		if(a.solenoid[i]<b.solenoid[i]) return 1;
		if(b.solenoid[i]<a.solenoid[i]) return 0;
	}

	for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
		if(a.relay[i]<b.relay[i]) return 1;
		if(b.relay[i]<a.relay[i]) return 0;
	}

	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		auto a1=a.digital_io[i];
		auto b1=b.digital_io[i];
		if(a1<b1) return 1;
		if(b1<a1) return 0;
	}
	
	for(unsigned i=0;i<Robot_outputs::TALON_SRX_OUTPUTS;i++){
		if(a.talon_srx[i]<b.talon_srx[i])return 1;
		if(b.talon_srx[i]<a.talon_srx[i])return 0;
	}
	
	/*for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
		auto a1=a.jaguar[i];
		auto b1=b.jaguar[i];
		if(a1<b1) return 1;
		if(b1<a1) return 0;
	}*/

	return a.pump_auto<b.pump_auto;
}

ostream& operator<<(ostream& o,Robot_outputs a){
	o<<"Robot_outputs(";
	o<<"pwm:";
	for(unsigned i=0;i<a.PWMS;i++){
		o<<(int)a.pwm[i]<<" ";
	}
	o<<" solenoid:";
	for(unsigned i=0;i<a.SOLENOIDS;i++){
		o<<a.solenoid[i];
	}
	o<<" relay:";
	for(unsigned i=0;i<a.RELAYS;i++){
		//o<<a.relay[i];
		terse(o,a.relay[i]);
	}
	o<<" dio:";
	for(unsigned i=0;i<a.DIGITAL_IOS;i++){
		//o<<a.digital_io[i];
		terse(o,a.digital_io[i]);
	}
	o<<" talon_srx:";
	for(unsigned i=0;i<a.Robot_outputs::TALON_SRX_OUTPUTS;i++){
		o<<a.talon_srx[i];
	}
	o<<" panel_output:";
	for(unsigned i=0;i<Panel_outputs::PANEL_OUTPUTS;i++){
		o<<a.panel_output[i];
	}
	/*o<<" jaguar:";
	for(unsigned i=0;i<a.CAN_JAGUARS;i++){
		o<<a.jaguar[i];
	}*/
	o<<" driver_station_output:"<<a.driver_station;
	o<<" pump_auto:"<<a.pump_auto;
	return o<<")";
}

Joystick_data::Joystick_data(){
	for(unsigned i=0;i<JOY_AXES;i++){
		axis[i]=0;
	}
	pov=-1;//Set to center by default
}

//int atoi(string s){ return ::atoi(s.c_str()); }

Maybe<Joystick_data> Joystick_data::parse(string const& s){
	//assert(0);
	#if 1
	string s2=inside_parens(s);
	//cout<<"got:"<<s2<<"\n";
	vector<string> v=split(s2,':');
	if(v.size()!=4) return Maybe<Joystick_data>();
	Joystick_data r;
	{
		r.pov=atoi(v[2].c_str());
	}
	{
		//cout<<"bs="<<v[1]<<"\n";
		vector<string> b=split(v[3]);
		//cout<<"list:"<<b.size()<<" "<<Joystick_data::BUTTONS<<"\n";
		for(unsigned i=0;i<JOY_BUTTONS;i++){
			if(i>=b.size()){
				return Maybe<Joystick_data>();
			}
			//cout<<"xx\n";
			r.button[i]=atoi(b[i].c_str());
		}
	}
	{
		vector<string> ax=split(v[1]);
		for(unsigned i=0;i<JOY_AXES;i++){
			if(i>ax.size()) return Maybe<Joystick_data>();
			r.axis[i]=atof(ax[i]);
		}
	}
	return Maybe<Joystick_data>(r);
	#endif
}

bool operator<(Joystick_data a,Joystick_data b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(axis) X(button) X(pov)
	#undef X
	return 0;
}

bool operator==(Joystick_data a,Joystick_data b){
	for(unsigned i=0;i<JOY_AXES;i++){
		if(a.axis[i]!=b.axis[i]){
			return 0;
		}
	}
	if(a.pov!=b.pov)return 0;
	for(unsigned i=0;i<JOY_BUTTONS;i++){
		if(a.button[i]!=b.button[i]){
			return 0;
		}
	}
	return 1;
}

bool operator!=(Joystick_data a,Joystick_data b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Joystick_data a){
	o<<"Joystick_data(";
	o<<"axes:";
	for(unsigned i=0;i<JOY_AXES;i++){
		//o<<setprecision(2)<<a.axis[i]<<" ";
		o<<setw(5)<<fixed<<setprecision(2)<<a.axis[i]<<" ";
	}
	o<<"pov:"<<a.pov<<" ";
	o<<"buttons:";
	for(unsigned i=0;i<JOY_BUTTONS;i++){
		o<<a.button[i]<<" ";
	}
	return o<<")";
}

Robot_mode::Robot_mode():autonomous(0),enabled(0){}

bool operator<(Robot_mode a,Robot_mode b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(autonomous) X(enabled)
	#undef X
	return 0;
}

bool operator==(Robot_mode a,Robot_mode b){
	return a.autonomous==b.autonomous && a.enabled==b.enabled;
}

bool operator!=(Robot_mode a,Robot_mode b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Robot_mode m){
	/*switch(m){
		#define X(name) case name: return o<<""#name;
		X(ROBOT_MODE_AUTO)
		X(ROBOT_MODE_TELE)
		#undef X
		default: return o<<"ERROR";
	}*/
	o<<"Robot_mode(";
	o<<"auto:"<<m.autonomous;
	o<<" enable:"<<m.enabled;
	return o<<")";
}

ostream& operator<<(ostream& o,Digital_in d){
	switch(d){
		#define X(name) case Digital_in::name: return o<<""#name;
		X(OUTPUT)
		X(_0)
		X(_1)
		#undef X
		default: return o<<"error";
	}
}

void terse(ostream& o,Digital_in d){
	switch(d){
		case Digital_in::OUTPUT:
			o<<'.';
			break;
		case Digital_in::_0:
			o<<'0';
			break;
		case Digital_in::_1:
			o<<'1';
			break;
		default:
			o<<'?';
	}
}

vector<Digital_in> digital_ins(){
	vector<Digital_in> r;
	r|=Digital_in::OUTPUT;
	r|=Digital_in::_0;
	r|=Digital_in::_1;
	return r;
}

Maybe<Digital_in> parse_digital_in(string s){
	//might want to also allow the terse versions
	return parse_enum(digital_ins(),s);
}

Digital_inputs::Digital_inputs(){
	for(auto&a:in){
		a=Digital_in::OUTPUT;
	}
}

bool operator<(Digital_inputs const& a,Digital_inputs const& b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(in) X(encoder)
	#undef X
	return 0;
}

bool operator==(Digital_inputs const& a,Digital_inputs const& b){
	return a.in==b.in && a.encoder==b.encoder;
}

bool operator!=(Digital_inputs const& a,Digital_inputs const& b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Digital_inputs const& a){
	o<<"Digital_inputs(";
	o<<a.in;
	o<<a.encoder;
	return o<<")";
}

Robot_inputs::Robot_inputs():
	now(0),orientation(0),pump(0)
{
	for(unsigned i=0;i<ANALOG_INPUTS;i++){
		//could make this be NAN instead
		analog[i]=0;
	}
	for(auto& a:current) a=0;
}

bool operator==(Robot_inputs a,Robot_inputs b){
	if(a.robot_mode!=b.robot_mode) return 0;
	if(a.now!=b.now) return 0;
	for(unsigned i=0;i<Robot_inputs::JOYSTICKS;i++){
		if(a.joystick[i]!=b.joystick[i]) return 0;
	}
	if(a.digital_io!=b.digital_io) return 0;
	for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
		if(a.analog[i]!=b.analog[i]){
			return 0;
		}
	}
	/*for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
		if(a.jaguar[i]!=b.jaguar[i]){
			return 0;
		}
	}*/
	for(unsigned i=0;i<Robot_inputs::TALON_SRX_INPUTS;i++){
		if(a.talon_srx[i]!=b.talon_srx[i]){
			return 0;
		}
	}
	for(unsigned i=0; i<Robot_inputs::CURRENT;i++){
		if(a.current[i]!=b.current[i]){
			return 0;
		}
	}
	if(a.driver_station!=b.driver_station) return 0;
	return a.orientation==b.orientation;
}

bool operator!=(Robot_inputs a,Robot_inputs b){
	return !(a==b);
}

bool operator<(Robot_inputs a,Robot_inputs b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(robot_mode)
	X(now)
	X(joystick)
	X(digital_io)
	X(analog)
	X(talon_srx)
	//X(jaguar) 
	X(driver_station)
	X(orientation)
	X(current)
	X(pump)
	#undef X
	return 0;
}

ostream& operator<<(ostream& o,Robot_inputs a){
	o<<"Robot_inputs(\n";
	o<<"  mode="<<a.robot_mode<<"\n";
	o<<"  now="<<a.now<<"\n";
	//o<<"joysticks:";
	for(unsigned i=0;i<a.JOYSTICKS;i++){
		o<<"  "<<a.joystick[i]<<"\n";
	}
	o<<"dio:"<<a.digital_io;
	/*for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		terse(o,a.digital_io[i]);
	}*/
	o<<" analog:";
	for(unsigned i=0;i<a.ANALOG_INPUTS;i++){
		o<<(i)<<" "<<a.analog[i]<<' ';
	}
	o<<" talon_srx:";
	for(unsigned i=0;i<Robot_inputs::TALON_SRX_INPUTS;i++){
		o<<a.talon_srx[i];
	}
	/*o<<" jaguar:";
	for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
		o<<a.jaguar[i];
	}*/
	o<<" currents:"<<a.current;	
	o<<" driver_station_inputs:"<<a.driver_station;
	o<<" orientation:"<<a.orientation;
	return o<<")";
}

#ifdef INTERFACE_TEST
int main(){
	cout<<Robot_outputs()<<"\n";
	cout<<Robot_inputs()<<"\n";
	cout<<Joystick_data()<<"\n";
	cout<<Jaguar_output()<<"\n";
	cout<<Talon_srx_output()<<"\n";
	cout<<Jaguar_output::speedOut(10)<<"\n";
	cout<<Jaguar_output::voltageOut(1.0)<<"\n";
	for(auto a:digital_ins()){
		assert(a==parse_digital_in(as_string(a)));
	}
	for(auto a:relay_outputs()){
		assert(a==parse_relay_output(as_string(a)));
	}
	assert(parse_relay_output("")==Maybe<Relay_output>());
	Joystick_data j;
	j.button[3]=1;
	j.pov=90;
	j.axis[5]=.3;
	assert(j==Joystick_data::parse(as_string(j)));
}
#endif
