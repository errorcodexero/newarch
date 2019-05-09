#include "input.h"
#include<cassert>
#include<set>
#include "interface.h"

using namespace std;

Input::Input(Type t):type_(t){}
Input::Input(Type t,unsigned i):type_(t),index_(i){}

#define NUMBERED_TYPES \
	X(joystick,JOYSTICK,Robot_inputs::JOYSTICKS)\
	X(digital_io,DIGITAL_IO,Robot_outputs::DIGITAL_IOS)\
	X(encoder,ENCODER,Digital_inputs::ENCODERS)\
	X(analog,ANALOG,Robot_inputs::ANALOG_INPUTS)\
	X(talon_srx,TALON_SRX,Robot_inputs::TALON_SRX_INPUTS)\
	X(current,CURRENT,Robot_inputs::CURRENT)
//	X(can_jaguar,CAN_JAGUAR,Robot_outputs::CAN_JAGUARS)

bool numbered_type(Input::Type t){
	#define X(A,B,C) if(t==Input::Type::B) return 1;
	NUMBERED_TYPES
	#undef X
	return 0;
}

Input::Type Input::type()const{ return type_; }

unsigned Input::index()const{
	assert(numbered_type(type_));
	return index_;
}

#define X(NAME,TYPE,LEN) Input Input::NAME(unsigned i){\
	assert(i<LEN);\
	return Input(Type::TYPE,i);\
}
NUMBERED_TYPES
#undef X

Input Input::driver_station(){ return Input(Type::DRIVER_STATION); }
Input Input::pump(){ return Input(Type::PUMP); }

bool operator<(Input const& a,Input const& b){
	if(a.type()<b.type()) return 1;
	if(b.type()<a.type()) return 0;
	if(numbered_type(a.type())) return a.index()<b.index();
	return 0;
}

ostream& operator<<(ostream& o,Input::Type a){
	#define X(NAME) if(a==Input::Type::NAME) return o<<""#NAME;
	INPUT_TYPES
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Input const& a){
	o<<"Input("<<a.type();
	if(numbered_type(a.type())) o<<" "<<a.index();
	return o<<")";
}

set<Input> inputs(){
	set<Input> r;
	#define X(A,B,C) for(unsigned i=0;i<C;i++) r.insert(Input::A(i));
	NUMBERED_TYPES
	#undef X
	r.insert(Input::driver_station());
	return r;
}

#ifdef INPUT_TEST
int main(){
	for(auto a:inputs()){
		cout<<a<<"\n";
	}
	return 0;
}
#endif
