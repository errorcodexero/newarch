#include "meta.h"
#include<set>
#include "interface.h"
#include "util.h"

using namespace std;

#define ROBOT_OUTPUT(X)\
	X(PWM,Pwm,pwm)\
	X(SOLENOID,Solenoid_id,solenoid)\
	X(RELAY,Relay_id,relay)\
	X(DIGITAL_IO,Digital_io_id,digital_io)\
	X(TALON_SRX,Talon_srx_id,talon_srx)\
	X(DRIVER_STATION,Driver_station,driver_station)\
	X(PUMP,Pump,pump)

#define X(A,B,C) \
	struct B{}; \
	std::ostream& operator<<(ostream& o,B){ return o<<""#A; } \
	bool operator<(B,B){ return 0; }
X(DRIVER_STATION,Driver_station,driver_station)
X(PUMP,Pump,pump)
	X(ROBOT_MODE,Robot_mode_in,robot_mode)\
	X(NOW,Now,now)\
	X(JOYSTICK,Joystick,joystick)\
	X(PUMP,Pump_in,pump)
#undef X

#define NUMBERED(NAME)\
	struct NAME{\
		unsigned index;\
	};\
	ostream& operator<<(ostream& o,NAME a){\
		return o<<""#NAME<<"("<<a.index<<")";\
	}\
	bool operator<(NAME a,NAME b){ return a.index<b.index; }

NUMBERED(Pwm)
NUMBERED(Solenoid_id)
NUMBERED(Relay_id)
NUMBERED(Digital_io_id)
NUMBERED(Talon_srx_id)
NUMBERED(Analog_id)
NUMBERED(Current)

#define A_LIST(A,B,C) A,
struct Robot_output{
	enum class Type{
		ROBOT_OUTPUT(A_LIST)
	};

	private:
	Type type_;

	#define X(A,B,C) B C##_;
	ROBOT_OUTPUT(X)
	#undef X

	public:
	#define X(A,B,C) explicit Robot_output(B b):type_(Type::A),C##_(b){}
	ROBOT_OUTPUT(X)
	#undef X

	Type type()const{ return type_; }

	#define X(A,B,C) B const& C()const{ assert(type()==Type::A); return C##_; }
	ROBOT_OUTPUT(X)
	#undef X
};

ostream& operator<<(ostream& o,Robot_output const& a){
	#define X(A,B,C) if(a.type()==Robot_output::Type::A) return o<<a.C();
	ROBOT_OUTPUT(X)
	#undef X
	assert(0);
}

bool operator<(Robot_output const& a,Robot_output const& b){
	if(a.type()<b.type()) return 1;
	if(b.type()<a.type()) return 0;
	#define X(A,B,C) if(a.type()==Robot_output::Type::A) return a.C()<b.C();
	ROBOT_OUTPUT(X)
	#undef X
	assert(0);
}

#define ROBOT_INPUT(X)\
	X(ROBOT_MODE,Robot_mode_in,robot_mode)\
	X(NOW,Now,now)\
	X(JOYSTICK,Joystick,joystick)\
	X(DIGITAL_IO,Digital_io_id,digital_io)\
	X(ANALOG,Analog_id,analog)\
	X(TALON_SRX,Talon_srx_id,talon_srx)\
	X(DRIVER_STATION,Driver_station,driver_station)\
	X(CURRENT,Current,current)\
	X(PUMP,Pump_in,pump)

struct Robot_input{
	enum class Type{ROBOT_INPUT(A_LIST)};

	private:
	Type type_;

	#define X(A,B,C) B C##_;
	ROBOT_INPUT(X)
	#undef X

	public:
	#define X(A,B,C) explicit Robot_input(B b):type_(Type::A),C##_(b){}
	ROBOT_INPUT(X)
	#undef X

	Type type()const{ return type_; }

	#define X(A,B,C) B const& C()const{ assert(type()==Type::A); return C##_; }
	ROBOT_INPUT(X)
	#undef X
};

set<Robot_output> differences(Robot_outputs const& a,Robot_outputs const& b){
	set<Robot_output> r;
	#define CHECK_NUMBERED(A,B,C) \
		for(unsigned i=0;i<Robot_outputs::A##S;i++){\
			if(a.C[i]!=b.C[i]){\
				r|=Robot_output{B{i}};\
			}\
		}
	CHECK_NUMBERED(PWM,Pwm,pwm)
	CHECK_NUMBERED(SOLENOID,Solenoid_id,solenoid)
	CHECK_NUMBERED(RELAY,Relay_id,relay)
	CHECK_NUMBERED(DIGITAL_IO,Digital_io_id,digital_io)
	CHECK_NUMBERED(TALON_SRX_OUTPUT,Talon_srx_id,talon_srx)
	#undef CHECK_NUMBERED

	#define X(A,B,C) if(a.C!=b.C) r|=Robot_output{B{}};
	X(DRIVER_STATION,Driver_station,driver_station)\
	X(PUMP,Pump,pump)
	#undef X
	return r;
}

set<Robot_input> differences(Robot_inputs const& a,Robot_inputs const& b){
	set<Robot_input> r;
	#define CHECK_NUMBERED(A,B,C) \
		for(unsigned i=0;i<Robot_inputs::A;i++){\
			if(a.C[i]!=b.C[i]){\
				r|=Robot_input{B{i}};\
			}\
		}
	CHECK_NUMBERED(TALON_SRX_INPUTS,Talon_srx_id,talon_srx)
	CHECK_NUMBERED(ANALOG_INPUTS,Analog_id,analog)
	CHECK_NUMBERED(TALON_SRX_INPUTS,Talon_srx_id,talon_srx)
	CHECK_NUMBERED(CURRENT,Current,current)

	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		if(a.digital_io.in[i]!=b.digital_io.in[i]){
			r|=Robot_input{Digital_io_id{i}};
		}
	}
	for(unsigned i=0;i<Digital_inputs::ENCODERS;i++){
		auto av=a.digital_io.encoder[i];
		auto bv=b.digital_io.encoder[i];
		if(av!=bv){
			if(av) r|=Robot_input{Digital_io_id{unsigned(*av)}};
			if(bv) r|=Robot_input{Digital_io_id{unsigned(*bv)}};
		}
	}

	#define X(A,B,C) if(a.C!=b.C) r|=Robot_input{B{}};
	X(ROBOT_MODE,Robot_mode_in,robot_mode)
	X(NOW,Now,now)
	X(JOYSTICK,Joystick,joystick)
	X(DRIVER_STATION,Driver_station,driver_station)
	X(PUMP,Pump_in,pump)
	#undef X
	return r;
}

ostream& operator<<(ostream& o,Robot_input const& a){
	#define X(A,B,C) if(a.type()==Robot_input::Type::A) return o<<a.C();
	ROBOT_INPUT(X)
	#undef X
	assert(0);
}

bool operator<(Robot_input const& a,Robot_input const& b){
	if(a.type()<b.type()) return 1;
	if(b.type()<a.type()) return 0;
	#define X(A,B,C) if(a.type()==Robot_input::Type::A) return a.C()<b.C();
	ROBOT_INPUT(X)
	#undef X
	assert(0);
}

struct IO{
	enum class Type{IN,OUT};

	private:
	Type type_;
	Maybe<Robot_input> in_;
	Maybe<Robot_output> out_;

	public:
	explicit IO(Robot_input a):type_(Type::IN),in_(a){}
	explicit IO(Robot_output a):type_(Type::OUT),out_(a){}

	Type type()const{ return type_; }

	Robot_input const& in()const{
		assert(type()==Type::IN);
		return *in_;
	}

	Robot_output const& out()const{
		assert(type()==Type::OUT);
		return *out_;
	}
};

ostream& operator<<(ostream& o,IO const& a){
	switch(a.type()){
		case IO::Type::IN: return o<<a.in();
		case IO::Type::OUT: return o<<a.out();
		default: assert(0);
	}
}

bool operator<(IO const& a,IO const& b){
	if(a.type()<b.type()) return 1;
	if(b.type()<a.type()) return 0;
	switch(a.type()){
		case IO::Type::IN: return a.in()<b.in();
		case IO::Type::OUT: return a.out()<b.out();
		default: assert(0);
	}
}

#ifdef META_TEST
//#include "../control/tilt.h"
//#include "../control/front.h"

template<typename T>
set<IO> find_ios(T const& t){
	set<IO> r;
	auto a=examples((typename T::Input*)0);
	set<Robot_inputs> in;
	for(auto elem:a){
		Robot_inputs a;
		in|=t.input_reader(a,elem);
	}
	for(auto ex:in){
		auto d=differences(Robot_inputs{},ex);
		for(auto elem:d){
			r|=IO{elem};
		}
	}

	auto b=examples((typename T::Output*)0);
	set<Robot_outputs> outs;
	for(auto elem:b){
		Robot_outputs out;
		outs|=t.output_applicator(out,elem);
	}
	for(auto ex:outs){
		auto d=differences(Robot_outputs{},ex);
		for(auto elem:d) r|=IO{elem};
	}
	return r;
}

int main(){
	//Robot_outputs a,b;
	Robot_inputs a,b;
	//b=example((Robot_outputs*)0);
	//b.pwm[2]=.5;
	//b.solenoid[0]=1;
	b.joystick[0].axis[1]=1;
	b.current[12]=-.05;
	cout<<differences(a,b)<<"\n";

	IO io{Robot_output{Pwm{0}}};
	cout<<io<<"\n";
	#define X(NAME) { NAME x; cout<<""#NAME<<" ios:"<<find_ios(x)<<"\n"; }
	//X(Front) X(Tilt)
	#undef X
}
#endif
