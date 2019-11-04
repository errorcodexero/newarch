#include "i2c_control.h"
#include "../util/util.h"
#include <cmath>
#include <cassert>

using namespace std;

I2C_control::I2C_control():i2c(NULL),i2c_port(I2C_port::ONBOARD){}
I2C_control::I2C_control(int address):i2c(NULL),i2c_port(I2C_port::ONBOARD) {
	init(I2C_port::ONBOARD,address);
}
I2C_control::I2C_control(I2C_port port,int address):i2c(NULL),i2c_port(I2C_port::ONBOARD) {
	init(port,address);
}

I2C_control::~I2C_control(){
	delete i2c;
}

void I2C_control::init(I2C_control::I2C_port port,int address){
	assert(!i2c);
	frc::I2C::Port frc_port = [&]{
		switch(port){
			case I2C_control::I2C_port::ONBOARD:
				return frc::I2C::Port::kOnboard;
			case I2C_control::I2C_port::MXP:
				return frc::I2C::Port::kMXP;
			default:
				nyi
		}
	}();
	i2c = new frc::I2C(frc_port,address);
	assert(i2c);
}

ostream& operator<<(ostream& o,I2C_control::I2C_port a){
	#define X(NAME) if(a==I2C_control::I2C_port::NAME) return o<<""#NAME;
	I2C_PORTS
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,I2C_control a){
	o<<"I2C_control(port:"<<a.i2c_port;
	o<<" init:"<<!!a.i2c;
	return o<<")";
}

void I2C_control::write(I2C_io const& a)const{
	uint8_t to_send[a.data.size()];
	for(unsigned i = 0; i < a.data.size(); i++){
		to_send[i] = a.data[i];
	}

	i2c -> Transaction(to_send, a.data.size() /*a.data.size()*/ /*sizeof(to_send)/sizeof(*to_send)*/, NULL, 0);
}

void I2C_control::read(I2C_io const& a)const{
	uint8_t * to_read = new uint8_t[a.data.size()];
	i2c -> Transaction(NULL, 0, to_read, a.data.size());
}

