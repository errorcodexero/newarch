#ifndef INTERFACE_H
#define INTERFACE_H

#include "driver_station_interface.h"
#include "maybe_inline.h"
#include "checked_array.h"
#include "params_parser.h"
#include "quick.h"
#include "unittype.h"
#include <iosfwd>
#include <bitset>
#include <set>

typedef bool Solenoid_output;

using Pwm_output = double;

enum class Relay_output{_00,_01,_10,_11};
std::ostream& operator<<(std::ostream&,Relay_output);

class Digital_out{
	public:
	enum class Type{INPUT,_1,_0,ENCODER};

	private:
	Type type_;
	int encoder_index_;
	bool input_a_;

	public:
	Digital_out();

	Type type()const;
	int encoder_index()const;
	bool input_a()const;

	static Digital_out input();
	static Digital_out one();
	static Digital_out zero();
	static Digital_out encoder(int encoder_index,bool input_a);
};

struct PID_values{
	float p,i,d,f;
	PID_values();
};

std::ostream& operator<<(std::ostream&,PID_values const&);
bool operator==(PID_values const&,PID_values const&);
bool operator!=(PID_values const&,PID_values const&);
bool operator<(PID_values const&,PID_values const&);

struct Talon_srx_input{
	#define TALON_SRX_INPUT_ITEMS(X) \
		X(int,encoder_position) \
		X(bool,fwd_limit_switch) \
		X(bool,rev_limit_switch) \
		X(bool,a) \
		X(bool,b) \
		X(int,velocity) \
		X(double,current) 

	STRUCT_MEMBERS(TALON_SRX_INPUT_ITEMS)

	IMPL_STRUCT_DECLARE(Talon_srx_input,TALON_SRX_INPUT_ITEMS)	

	Talon_srx_input();
};

struct Talon_srx_output{
	#define TALON_SRX_OUTPUT_MODES \
		X(PERCENT) \
		X(SPEED)
	
	enum class Mode{
		#define X(NAME) NAME,
		TALON_SRX_OUTPUT_MODES
		#undef X
	};//percent means percent voltage on the in terminals on the talon

	#define TALON_SRX_OUTPUT_ITEMS(X) \
		X(PID_values,pid) \
		X(double,power_level) \
		X(double,speed) \
		X(bool,brake) \
		X(Mode,mode) 

	STRUCT_MEMBERS(TALON_SRX_OUTPUT_ITEMS)
	
	IMPL_STRUCT_DECLARE(Talon_srx_output,TALON_SRX_OUTPUT_ITEMS)

	Talon_srx_output();

	static Talon_srx_output percent(double);
	static Talon_srx_output closed_loop(double);
};

std::ostream& operator<<(std::ostream&,Talon_srx_output::Mode);
std::ostream& operator<<(std::ostream&,Talon_srx_output);
bool operator==(Talon_srx_output,Talon_srx_output);
bool operator!=(Talon_srx_output,Talon_srx_output);
bool operator<(Talon_srx_output,Talon_srx_output);

std::ostream& operator<<(std::ostream&,Talon_srx_input);

bool operator<(Talon_srx_input,Talon_srx_input);
bool operator==(Talon_srx_input,Talon_srx_input);
bool operator!=(Talon_srx_input,Talon_srx_input);

#define NAVX_INPUT_ITEMS(X) \
		X(double,pitch,GetPitch) \
		X(double,roll,GetRoll) \
		X(double,yaw,GetYaw) \
		X(double,compass_heading,GetCompassHeading) \
		X(bool,calibrating,IsCalibrating) \
		X(bool,connected,IsConnected) \
		X(double,update_count,GetUpdateCount) \
		X(long,last_sensor_timestamp,GetLastSensorTimestamp) \
		X(double,world_linear_accel_x,GetWorldLinearAccelX) \
		X(double,world_linear_accel_y,GetWorldLinearAccelY) \
		X(double,world_linear_accel_z,GetWorldLinearAccelZ) \
		X(bool,moving,IsMoving) \
		X(bool,rotating,IsRotating) \
		X(double,barometric_pressure,GetBarometricPressure) \
		X(double,altitude,GetAltitude) \
		X(bool,valid_altitude,IsAltitudeValid) \
		X(double,fused_heading,GetFusedHeading) \
		X(bool,magnetic_disturbance,IsMagneticDisturbance) \
		X(bool,magnetometer_calibrated,IsMagnetometerCalibrated) \
		X(double,quaternion_w,GetQuaternionW) \
		X(double,quaternion_x,GetQuaternionX) \
		X(double,quaternion_y,GetQuaternionY) \
		X(double,quaternion_z,GetQuaternionZ) \
		X(double,velocity_x,GetVelocityX) \
		X(double,velocity_y,GetVelocityY) \
		X(double,velocity_z,GetVelocityZ) \
		X(double,displacement_x,GetDisplacementX) \
		X(double,displacement_y,GetDisplacementY) \
		X(double,displacement_z,GetDisplacementY) \
		X(double,angle,GetAngle) \
		X(double,rate,GetRate) \
		X(double,angle_adjustment,GetAngleAdjustment) \
		X(double,raw_gyro_x,GetRawGyroX) \
		X(double,raw_gyro_y,GetRawGyroY) \
		X(double,raw_gyro_z,GetRawGyroZ) \
		X(double,raw_accel_x,GetRawAccelX) \
		X(double,raw_accel_y,GetRawAccelY) \
		X(double,raw_accel_z,GetRawAccelZ) \
		X(double,raw_mag_x,GetRawMagX) \
		X(double,raw_mag_y,GetRawMagY) \
		X(double,raw_mag_z,GetRawMagZ) \
		X(double,pressure,GetPressure) \
		X(double,temperature,GetTempC) \
		X(std::string,firmware_version,GetFirmwareVersion) \
		X(int,actual_update_rate,GetActualUpdateRate) \
		X(int,requested_update_rate,GetRequestedUpdateRate)
		//X(AHRS::BoardYawAxis,board_yaw_axis)//FIXME

struct Navx_input{
	//angle is cumulative and clockwise from top is positive
	STRUCT_MEMBERS(NAVX_INPUT_ITEMS)
	
	Navx_input();
	IMPL_STRUCT_DECLARE(Navx_input,NAVX_INPUT_ITEMS)
};

std::ostream& operator<<(std::ostream&,Navx_input);
bool operator==(Navx_input,Navx_input);
bool operator!=(Navx_input,Navx_input);
bool operator<(Navx_input,Navx_input);

#define NAVX_OUTPUT_ITEMS(X) \
	X(bool,zero_yaw,ZeroYaw) 

struct Navx_output{
	STRUCT_MEMBERS(NAVX_OUTPUT_ITEMS)

	Navx_output();
	IMPL_STRUCT_DECLARE(Navx_output,NAVX_OUTPUT_ITEMS);
};

std::ostream& operator<<(std::ostream&,Navx_output);
bool operator==(Navx_output,Navx_output);
bool operator!=(Navx_output,Navx_output);
bool operator<(Navx_output,Navx_output);

struct I2C_io{
	std::vector<uint8_t> data;
	I2C_io();
	I2C_io(std::vector<uint8_t>);
};

std::ostream& operator<<(std::ostream&,I2C_io);
bool operator==(I2C_io,I2C_io);
bool operator!=(I2C_io,I2C_io);
bool operator<(I2C_io,I2C_io);



struct Pump_input{
	#define PUMP_INPUT_ITEMS(X) \
		X(bool,pressure_switch_triggered)

	STRUCT_MEMBERS(PUMP_INPUT_ITEMS)

	Pump_input();
	IMPL_STRUCT_DECLARE(Pump_input,PUMP_INPUT_ITEMS)
};

std::ostream& operator<<(std::ostream&,Pump_input);
bool operator==(Pump_input,Pump_input);
bool operator!=(Pump_input,Pump_input);
bool operator<(Pump_input,Pump_input);

struct Pump_output{
	#define PUMP_OUTPUT_MODES \
		X(CLOSED_LOOP) \
		X(OPEN_LOOP)
	enum Mode{
		#define X(NAME) NAME,
		PUMP_OUTPUT_MODES
		#undef X
	};//controls whether the compressor automatically turns on at low pressure or not
	
	#define PUMP_OUTPUT_ITEMS(X) \
		X(Mode,mode)

	STRUCT_MEMBERS(PUMP_OUTPUT_ITEMS)
	
	Pump_output();
	IMPL_STRUCT_DECLARE(Pump_output,PUMP_OUTPUT_ITEMS)
};

std::ostream& operator<<(std::ostream&,Pump_output::Mode);
std::ostream& operator<<(std::ostream&,Pump_output);
bool operator==(Pump_output,Pump_output);
bool operator!=(Pump_output,Pump_output);
bool operator<(Pump_output,Pump_output);

std::ostream& operator<<(std::ostream&,Digital_out);
bool operator<(Digital_out,Digital_out);
bool operator==(Digital_out,Digital_out);
bool operator!=(Digital_out,Digital_out);

static const unsigned TALON_SRXS = 6;

struct Robot_outputs{
	static const unsigned PWMS=10;//Number of ports on the digital sidecar; there can be up to 20 using the MXP on the roboRIO which we don't do
	Checked_array<Pwm_output,PWMS> pwm;
	
	static const unsigned SOLENOIDS=8;
	Checked_array<Solenoid_output,SOLENOIDS> solenoid;
	
	static const unsigned RELAYS=4;
	Checked_array<Relay_output,RELAYS> relay;
	
	static const unsigned DIGITAL_IOS=14;//there are actually 26 on the roboRIO if you count the MXP, but that varies depending on whether they're set as dios or pwm
	Checked_array<Digital_out,DIGITAL_IOS> digital_io;
	
	static const unsigned TALON_SRX_OUTPUTS=TALON_SRXS;//FIXME: talon initializaitons
	Checked_array<Talon_srx_output, TALON_SRX_OUTPUTS> talon_srx;

	Navx_output navx;
	
	//could add in some setup for the analog inputs
	
	static const unsigned DRIVER_STATION_DIGITAL_OUTPUTS = Driver_station_output::DIGITAL_OUTPUTS;
	Driver_station_output driver_station;
	
	Pump_output pump;
	
	I2C_io i2c;

	Robot_outputs();
};

std::ostream& operator<<(std::ostream& o, Talon_srx_output);

std::ostream& operator<<(std::ostream& o, Talon_srx_input);

bool operator<(Robot_outputs,Robot_outputs);
bool operator==(Robot_outputs,Robot_outputs);
bool operator!=(Robot_outputs,Robot_outputs);
std::ostream& operator<<(std::ostream& o,Robot_outputs);

//limitation of FRC coms//TODO look into this
#define JOY_AXES 8
#define JOY_BUTTONS 16

struct Joystick_data{
  Joystick_data()
  {
    for(unsigned i = 0 ; i < JOY_AXES ; i++)
      axis[i] = 0 ;

    pov = -1 ;
  }
  
  Checked_array<double,JOY_AXES> axis;
  std::bitset<JOY_BUTTONS> button;
  int pov;
  
  static Maybe<Joystick_data> parse(std::string const&);
};
bool operator<(Joystick_data,Joystick_data);
bool operator==(Joystick_data,Joystick_data);
bool operator!=(Joystick_data,Joystick_data);
std::ostream& operator<<(std::ostream&,Joystick_data);

//We may need to add support for other modes at some point.
struct Robot_mode{
	bool autonomous;
	bool enabled;
	
	Robot_mode();
};
bool operator<(Robot_mode,Robot_mode);
bool operator==(Robot_mode,Robot_mode);
bool operator!=(Robot_mode,Robot_mode);
std::ostream& operator<<(std::ostream&,Robot_mode);

enum class Alliance{RED,BLUE,INVALID};
std::ostream& operator<<(std::ostream&,Alliance const&);

struct DS_info{
	#define DS_INFO_ITEMS(X) \
		X(bool,connected) \
		X(Alliance,alliance) \
		X(int,location) \
		X(bool,near_switch_left) \
		X(bool,scale_left) \
		X(bool,far_switch_left) \
		X(double,battery_voltage)
	
	STRUCT_MEMBERS(DS_INFO_ITEMS)

	IMPL_STRUCT_DECLARE(DS_info,DS_INFO_ITEMS)
	
	DS_info();
};

bool operator<(DS_info const&,DS_info const&);
bool operator==(DS_info const&,DS_info const&);
bool operator!=(DS_info const&,DS_info const&);
std::ostream& operator<<(std::ostream&,DS_info const&);

enum class Digital_in{OUTPUT,_0,_1,ENCODER};
std::ostream& operator<<(std::ostream&,Digital_in);
std::set<Digital_in> examples(Digital_in*);

typedef int Encoder_output;//TODO: is this not an input?

struct Digital_inputs{
	Checked_array<Digital_in,Robot_outputs::DIGITAL_IOS> in;
	static const unsigned ENCODERS=Robot_outputs::DIGITAL_IOS/2;
	Checked_array<Maybe_inline<Encoder_output>,ENCODERS> encoder;

	Digital_inputs();
};
bool operator<(Digital_inputs const&,Digital_inputs const&);
bool operator==(Digital_inputs const&,Digital_inputs const&);
bool operator!=(Digital_inputs const&,Digital_inputs const&);
std::ostream& operator<<(std::ostream&,Digital_inputs const&);


struct Robot_inputs{
	Robot_mode robot_mode;
	Time now;//time since robot code started running.

	DS_info ds_info;

	static const unsigned JOYSTICKS=3; //ports are 0-5, so there are actually six, but we only use the first three so we'll only consider them to limit print outs
	Checked_array<Joystick_data,JOYSTICKS> joystick;

	Digital_inputs digital_io;	

	static const unsigned ANALOG_INPUTS=4;
	Checked_array<Volt,ANALOG_INPUTS> analog;

	static const unsigned TALON_SRX_INPUTS=TALON_SRXS;
	Checked_array<Talon_srx_input, TALON_SRX_INPUTS> talon_srx;

	Navx_input navx;
	
	Driver_station_input driver_station;
	Rad orientation;
		
	static const unsigned CURRENT=16;
	Checked_array<double,CURRENT> current;

	Pump_input pump;
	paramsInput* input_params;

	Robot_inputs();
};
bool operator<(Robot_inputs,Robot_inputs);
bool operator==(Robot_inputs,Robot_inputs);
bool operator!=(Robot_inputs,Robot_inputs);
std::ostream& operator<<(std::ostream& o,Robot_inputs);
Robot_inputs rand(Robot_inputs*);
Robot_inputs random_inputs();

#endif
