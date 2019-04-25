#include "talon_srx_control.h"
#include "ctre/phoenix/MotorControl/SensorCollection.h"
#include "util.h"
#include <cmath>
#include <cassert>

using namespace std;

Talon_srx_control::Talon_srx_control():talon(NULL),out(),in(),since_query(0),mode(Mode::INIT){}
Talon_srx_control::Talon_srx_control(int CANBusAddress):talon(NULL),out(),in(),since_query(0),mode(Mode::INIT) {
	init(CANBusAddress);
}

Talon_srx_control::~Talon_srx_control(){
	delete talon;
}

void Talon_srx_control::init(int CANBusAddress){
	assert(!talon);
	assert(mode==Mode::INIT);
	talon = new ctre::phoenix::motorcontrol::can::WPI_TalonSRX(CANBusAddress);
	assert(talon);
	talon->SetSafetyEnabled(false);
}

void Talon_srx_control::set_inverted(){
	talon->SetInverted(true);
}

ostream& operator<<(ostream& o,Talon_srx_control::Mode a){
	#define X(NAME) if(a==Talon_srx_control::Mode::NAME) return o<<""#NAME;
	TALON_SRX_MODES
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Talon_srx_control a){
	o<<"Talon_srx_control( mode:"<<a.mode;
	if(!!a.talon) o<<" out:"<<a.out;/*" last_out:"<<a.last_out<<*/
	o<<" init:"<<!!a.talon;
	o<<" since_query:"<<a.since_query;
	o<<" in:"<<a.in;
	return o<<")";
}

bool pid_approx(PID_values a,PID_values b){
	const float TOLERANCE=.001;
	return fabs(a.p-b.p)<TOLERANCE && fabs(a.i-b.i)<TOLERANCE && fabs(a.d-b.d)<TOLERANCE && fabs(a.f-b.f)<TOLERANCE;
}

static const unsigned QUERY_LIM = 0;

void Talon_srx_control::set(Talon_srx_output a, bool enable) {
	static const float EXPIRATION=2.0;
	if(!enable || mode == Mode::INIT){
		if(mode!=Talon_srx_control::Mode::DISABLE){ 
			talon->Set(0);
			talon->SetSafetyEnabled(false);
			talon->Disable();
			mode=Talon_srx_control::Mode::DISABLE;
		} else if (since_query > QUERY_LIM){
			talon->Set(0);
		}
		return;
	}
	talon->SetNeutralMode(a.brake ? ctre::phoenix::motorcontrol::NeutralMode::Brake : ctre::phoenix::motorcontrol::NeutralMode::Coast);
	switch(a.mode){
		case Talon_srx_output::Mode::PERCENT:
			/* TODO:  was causing problems so it has been removed -- should we fix it and use it?
			if(!approx_equal(a.power_level,clip(a.power_level))){//make sure we're not setting the power to over 100% or under -100%
				cout<<endl<<"Power level set too high: "<<a.power_level<<endl;
				assert(0);
			}
			*/
			if(mode!=Talon_srx_control::Mode::PERCENT){
				talon->SetExpiration(EXPIRATION);
				talon->SetSafetyEnabled(true);
				talon->Set(ctre::phoenix::motorcontrol::ControlMode::PercentOutput, a.power_level);
				out=a;
				mode=Talon_srx_control::Mode::PERCENT;
			} else if((a.power_level!=out.power_level || since_query > QUERY_LIM) /*&& out!=last_out*/){
				talon->Set(a.power_level);
				out.power_level=a.power_level;
			}
			break;
		case Talon_srx_output::Mode::SPEED:
			if(mode!=Talon_srx_control::Mode::SPEED || !pid_approx(out.pid,a.pid)){
				talon->Config_kP(0, a.pid.p, 0);
				talon->Config_kI(0, a.pid.i, 0);
				talon->Config_kD(0, a.pid.d, 0);
				talon->Config_kF(0, a.pid.f, 0);
				talon->ConfigSelectedFeedbackSensor(ctre::phoenix::motorcontrol::FeedbackDevice::QuadEncoder, 0, 0); //TODO: change this so that we can use other feedback types
				talon->SetExpiration(EXPIRATION);
				talon->SetSafetyEnabled(true);
				talon->Set(ctre::phoenix::motorcontrol::ControlMode::Velocity, a.speed);
				out=a;
				mode=Talon_srx_control::Mode::SPEED;
			} else if((a.speed!=out.speed || since_query > QUERY_LIM) /*&& out!=last_out*/){ 
				talon->Set(a.speed);
				out.speed=a.speed;
			}
			break;
		default:
			nyi
	}
}

Talon_srx_input Talon_srx_control::get(){
	if(since_query > QUERY_LIM){
		// in.current=talon->GetBusVoltage(); //TODO: look into this again
		// in.velocity=talon->GetSelectedSensorVelocity(0);
		// ctre::phoenix::motorcontrol::SensorCollection collection = talon->GetSensorCollection();
		// in.a=collection.GetPinStateQuadA();
		// in.b=collection.GetPinStateQuadB();
		// in.encoder_position=collection.GetQuadraturePosition();
		// ctre::phoenix::motorcontrol::Faults faults;
		// talon->GetFaults(faults);
		// in.fwd_limit_switch=faults.ForwardLimitSwitch;
		// in.rev_limit_switch=faults.ReverseLimitSwitch;
		since_query=0;
	}
	since_query++;
	return in;
}

Talon_srx_controls::Talon_srx_controls():init_(false){}

void Talon_srx_controls::init(){
	if(!init_){
		for(unsigned int i=0; i<talons.size(); i++){
			talons[i].init(i+1);//2017 comp talons start at device ID 1
		}
		init_=true;
	}
}

void Talon_srx_controls::set_inverted(int id){
	talons[id].set_inverted();
}

void Talon_srx_controls::set(Checked_array<Talon_srx_output,Robot_outputs::TALON_SRX_OUTPUTS> const& a,Checked_array<bool,Robot_outputs::TALON_SRX_OUTPUTS> const& enable){
	init();
	for(unsigned int i=0; i<talons.size(); i++){
		talons[i].set(a[i],enable[i]);
	}
}

array<Talon_srx_input,Robot_inputs::TALON_SRX_INPUTS> Talon_srx_controls::get(){
	init();
	
	array<Talon_srx_input,Robot_inputs::TALON_SRX_INPUTS> inputs;
	for(unsigned int i=0; i<Robot_inputs::TALON_SRX_INPUTS; i++){
		inputs[i]=talons[i].get();
	}
	return inputs;
}

ostream& operator<<(ostream& o,Talon_srx_controls const& t){
	o<<"Talon_srx_controls(";
	o<< "init:"<<t.init_;
	for(unsigned int i=0;i<t.talons.size(); i++){
		//o<<" talon_srx_"<<i<<t.talons[i];
	}
	return o<<")";
}

