#include "pump_control.h"
#include "Compressor.h"
#include "../util/util.h"
#include <cmath>
#include <cassert>

using namespace std;

Pump_control::Pump_control():compressor(new frc::Compressor),out(),in(){}

Pump_control::~Pump_control(){
	delete compressor;
}

ostream& operator<<(ostream& o,Pump_control a){
	o<<"Pump_control(";
	if(!!a.compressor) o<<" out:"<<a.out;
	o<<" init:"<<!!a.compressor;
	o<<" in:"<<a.in;
	return o<<")";
}

void Pump_control::set(Pump_output a) {
	compressor -> SetClosedLoopControl(a.mode == Pump_output::Mode::CLOSED_LOOP);
}

Pump_input Pump_control::get(){
	in.pressure_switch_triggered = compressor -> GetPressureSwitchValue();
	return in;
}

