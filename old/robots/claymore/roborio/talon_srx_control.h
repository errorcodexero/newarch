#ifndef TALON_SRX_CONTROL_H
#define TALON_SRX_CONTROL_H

#include "ctre/phoenix/MotorControl/CAN/WPI_TalonSRX.h"
#include "interface.h"
#include "checked_array.h"

class Talon_srx_control{
	private:
	ctre::phoenix::motorcontrol::can::WPI_TalonSRX *talon;
	
	Talon_srx_output out;
	//Talon_srx_output last_out;
	Talon_srx_input in;
	
	unsigned since_query; //TODO: should this be time based instead or something?
	
	public:
	#define TALON_SRX_MODES X(INIT) X(PERCENT) X(SPEED) X(DISABLE)
	enum class Mode{
		#define X(NAME) NAME,
		TALON_SRX_MODES
		#undef X
	};

	private:
	Mode mode;
	
	public:
	Talon_srx_control();
	~Talon_srx_control();
	explicit Talon_srx_control(int CANBusAddress);
	
	void init(int CANBusAddress);
	void set_inverted();
	void set(Talon_srx_output, bool);
	Talon_srx_input get();
	friend std::ostream& operator<<(std::ostream&,Talon_srx_control);
};

class Talon_srx_controls{
	private:
	std::array<Talon_srx_control,Robot_outputs::TALON_SRX_OUTPUTS> talons;
	bool init_;

	public:
	void init();
	void set_inverted(int);
	void set(Checked_array<Talon_srx_output,Robot_outputs::TALON_SRX_OUTPUTS> const&,Checked_array<bool,Robot_outputs::TALON_SRX_OUTPUTS> const&);
	std::array<Talon_srx_input,Robot_inputs::TALON_SRX_INPUTS> get();
	
	Talon_srx_controls();
	friend std::ostream& operator<<(std::ostream&,Talon_srx_controls const&);
};

std::ostream& operator<<(std::ostream&,Talon_srx_control::Mode);

#endif
