#ifndef JAG_INTERFACE_H
#define JAG_INTERFACE_H

#include<iosfwd>
#include "maybe.h"

struct PID_coefficients{
	float p,i,d;

	PID_coefficients();
};
bool operator==(PID_coefficients,PID_coefficients);
bool operator!=(PID_coefficients,PID_coefficients);
std::ostream& operator<<(std::ostream&,PID_coefficients);

struct Jaguar_output{
	PID_coefficients pid;
	double speed,voltage;
	bool controlSpeed;
	//0 = voltage
	//1 = speed

	Jaguar_output();
	static Jaguar_output speedOut(double);
	static Jaguar_output voltageOut(double);
	static Maybe<Jaguar_output> parse(std::string const&);
};
bool operator==(Jaguar_output,Jaguar_output);
bool operator!=(Jaguar_output,Jaguar_output);
bool operator<(Jaguar_output,Jaguar_output);
std::ostream& operator<<(std::ostream&,Jaguar_output);

struct Jaguar_input{
	double speed,current;

	Jaguar_input();

	static Maybe<Jaguar_input> parse(std::string const&);
};
bool operator==(Jaguar_input,Jaguar_input);
bool operator!=(Jaguar_input,Jaguar_input);
std::ostream& operator<<(std::ostream&,Jaguar_input);
bool approx_equal(Jaguar_input,Jaguar_input);

#endif
