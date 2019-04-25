#ifndef PUMP_CONTROL_H
#define PUMP_CONTROL_H

#include "../util/interface.h"
#include "../util/checked_array.h"

namespace frc{
	class Compressor;
};

class Pump_control{
	private:
	frc::Compressor *compressor;
	
	Pump_output out;
	Pump_input in;
	
	public:
	Pump_control();
	~Pump_control();
	
	void set(Pump_output);
	Pump_input get();
	friend std::ostream& operator<<(std::ostream&,Pump_control);
};

#endif
