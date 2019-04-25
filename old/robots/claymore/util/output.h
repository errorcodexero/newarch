#ifndef OUTPUT_H
#define OUTPUT_H

#include<iosfwd>
#include<set>

#define OUTPUT_TYPES X(PWM) X(SOLENOID) X(RELAY) X(DIGITAL_IO) X(TALON_SRX) X(DRIVER_STATION) X(PUMP)

#define NUMBERED_TYPES\
	X(pwm,PWM,Robot_outputs::PWMS)\
	X(solenoid,SOLENOID,Robot_outputs::SOLENOIDS)\
	X(relay,RELAY,Robot_outputs::RELAYS)\
	X(digital_io,DIGITAL_IO,Robot_outputs::DIGITAL_IOS)\
	X(talon_srx,TALON_SRX,Robot_outputs::TALON_SRX_OUTPUTS)\
	X(driver_station,DRIVER_STATION,Driver_station_output::DIGITAL_OUTPUTS)

class Output{
	public:
	enum class Type{
		#define X(NAME) NAME,
		OUTPUT_TYPES
		#undef X
	};

	private:
	Type type_;
	unsigned index_;
	Output(Type,unsigned);

	public:
	Type type()const;
	unsigned index()const;

	#define X(A,B,C) static Output A(unsigned);
	NUMBERED_TYPES
	#undef X
	static Output pump();
};

bool operator<(Output const&,Output const&);
std::ostream& operator<<(std::ostream&,Output const&);
std::set<Output> outputs();

#endif
