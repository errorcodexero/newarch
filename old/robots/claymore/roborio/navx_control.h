#ifndef NAVX_CONTROL_H
#define NAVX_CONTROL_H

#include "interface.h"
#include "SPI.h"

class AHRS;

class Navx_control{
	private:
	AHRS* ahrs;
	Navx_input in;

	Navx_output out;

	public:
	Navx_control();
	Navx_control(frc::SPI::Port);
	~Navx_control();

	Navx_input get();
	void set(Navx_output);
	void init(frc::SPI::Port);
	friend std::ostream& operator<<(std::ostream&,Navx_control);
};

#endif
