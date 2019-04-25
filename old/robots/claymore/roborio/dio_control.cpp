#include "dio_control.h"
#include <iostream>
#include "DigitalOutput.h"
#include "DigitalInput.h"
#include "Encoder.h"
#include "../util/util.h"

using namespace std;

//Unfortunately, there's not a good way to test all this without WPIlib since it's just an interface to WPIlib.
std::ostream& operator<<(std::ostream& o, Encoder_control const& a){
	
	o << "Encoder_control (" << "Encoder: " << a.encoder << " Channels: " << a.channel_a << "," << a.channel_b << ")";
	
	
	return o;
}
std::ostream& operator<<(std::ostream& o,DIO_controls const& a){
	

	//cout << "channel: " << a.channel << /* "encoder: " << a.encoder << */ "init_: " << a.init_ << endl;
	o <<"DIO controls ( "  << " channel: " << a.channel << "encoder: " << a.encoder << "init_: " << a.init_ << ")" ;
	return o;
}

DIO_control::DIO_control():channel(-1),in(nullptr),out(nullptr){}
DIO_control::DIO_control(int i):channel(i),in(nullptr),out(nullptr){}
DIO_control::~DIO_control(){ free(); }

int DIO_control::set_channel(int i){
	if(channel!=-1) return 1;
	assert(i>=0);
	channel=i;
	return 0;
}

int DIO_control::set(Digital_out d){
	if(channel==-1) return 4;
	if(d.type()==Digital_out::Type::INPUT) return set_mode(Mode::IN);
	if(d.type()==Digital_out::Type::ENCODER) return set_mode(Mode::FREE);
	{
		int r=set_mode(Mode::OUT);
		if(r) return r;
	}
	if(!out) return 1;
	out->Set(d.type()==Digital_out::Type::_1);
	return 0;
}

Digital_in DIO_control::get()const{
	//return DI_0;
	if(!in){
		//cerr<<"Mode is actually: "<<mode<<"\n";
		return Digital_in::OUTPUT;
	}
	return in->Get()?Digital_in::_1:Digital_in::_0;
}

DIO_control::Mode DIO_control::mode()const{
	assert(!in || !out);
	if(in) return Mode::IN;
	if(out) return Mode::OUT;
	return Mode::FREE;
}

int DIO_control::set_mode(Mode m){
	assert(channel!=-1);
	if(m==mode()) return 0;
	
	{
		int r=free();
		if(r) return r;
	}

	switch(m){
	case Mode::FREE: return 0;
	case Mode::IN:
		in=new frc::DigitalInput(channel);
		break;
	case Mode::OUT:
		out=new frc::DigitalOutput(channel);
		break;
	default: return 1;
	}
	if(!in && !out) return 2;
	return 0;
}
	
int DIO_control::free(){
	delete in;
	in=nullptr;
	delete out;
	out=nullptr;
	return 0;
}

ostream& operator<<(ostream& o,DIO_control::Mode a){
	switch(a){
	case DIO_control::Mode::IN: return o<<"in";
	case DIO_control::Mode::OUT: return o<<"out";
	case DIO_control::Mode::FREE: return o<<"free";
	default: return o<<"error";
	}
}

ostream& operator<<(ostream& o,DIO_control const& a){
	o<<"DIO_control(";
	o<<a.mode()<<" ";
	o<<a.channel;
	return o<<")";
}

Encoder_control::Encoder_control():encoder(nullptr),channel_a(-1),channel_b(-1){}

Encoder_control::~Encoder_control(){
	delete encoder;
}

DIO_controls::DIO_controls():init_(0){
}

void DIO_controls::init(){
	if(!init_){
		for(unsigned i=0;i<channel.size();i++){
			channel[i].set_channel(i);
		}
		init_=1;
	}
}


void DIO_controls::set(Checked_array<Digital_out,Robot_outputs::DIGITAL_IOS> const& a){
	init();

	array<int,Digital_inputs::ENCODERS> channel_a,channel_b;
	for(auto& a:channel_a) a=-1;
	for(auto& b:channel_b) b=-1;
	for(unsigned int i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		auto x = a[i];
		if (x.type()==Digital_out::Type::ENCODER){
			if(x.input_a())
				channel_a[x.encoder_index()] = i;
			else
				channel_b[x.encoder_index()] = i;
				
				
			
				
		}
		
		
	}

	//free encoders
	for(unsigned i=0;i<Digital_inputs::ENCODERS;i++){
		auto ca=channel_a[i];
		auto cb=channel_b[i];
		auto &d=encoder[i];
		if(ca!=d.channel_a || cb!=d.channel_b){
			delete d.encoder;
			d.encoder=nullptr;
		}
	}

	int r=0;
	//set all the dios
	for(unsigned i=0;i<channel.size();i++){
		r|=channel[i].set(a[i]);
	}
	if(r)cout<<"r:"<<r<<"\n";

	//create new encoders
	for(unsigned i=0;i<Digital_inputs::ENCODERS;i++){
		auto ca=channel_a[i];
		auto cb=channel_b[i];
		auto& d=encoder[i];
		if(ca!=d.channel_a || cb!=d.channel_b){
			assert(!d.encoder);
			d.encoder=new frc::Encoder(ca,cb);
			d.channel_a=ca;
			d.channel_b=cb;
		}
	}
}

Digital_inputs DIO_controls::get(){
	init();

	Digital_inputs r;
	for(unsigned i=0;i<channel.size();i++){
		r.in[i]=channel[i].get();
	}
	for(unsigned i=0;i<encoder.size();i++){
		if(encoder[i].encoder){
			r.encoder[i]=encoder[i].encoder->Get();//FIXME
		}
	}
	return r;
}
