#include "WPILib.h"
#include "AHRS.h"
#include "main.h"
#include "dio_control.h"
#include "talon_srx_control.h"
#include "pump_control.h"
#include "navx_control.h"
#include "i2c_control.h"
#include "params_parser.h"
#include "subsystems.h"
#include "params_parser.h"
#include "message_logger.h"
#include "message_dest_seq_file.h"
#include "message_dest_DS.h"
#include "message_dest_stream.h"
#include "motor_current_monitor.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

const char *param_file_name_p = "/home/lvuser/params.txt" ;

using namespace std;

Joystick_data read_joystick(frc::DriverStation& ds,int port){
	//I don't know what the DriverStation does when port is out of range.
	Joystick_data r;
	{
		auto lim=ds.GetStickPOVCount(port);
		assert(lim>=0);
		for(unsigned int i=0;i<(unsigned)lim;i++){
			r.pov=ds.GetStickPOV(port,i);
		}
	}
	{
		auto lim=ds.GetStickAxisCount(port);
		assert(lim>=0);
		for(unsigned i=0;i<(unsigned)lim;i++){
			r.axis[i]=ds.GetStickAxis(port,i);
		}
	}
	auto lim=ds.GetStickButtonCount(port);
	assert(lim>=0);
	for(unsigned i=0;i<(unsigned)lim;i++){
		//if(buttons&(1<<i)) r.button[i]=1;
		r.button[i]=ds.GetStickButton(port,i+1);
	}
	return r;
}

int read_joysticks(Robot_inputs &r, frc::DriverStation& ds){
	for(unsigned i=0;i<r.JOYSTICKS;i++){
		r.joystick[i]=read_joystick(ds,i);
	}
	return 0;
}

int demo(...){
	cerr<<"In demo\n";
	return 0;
}

string space_out(string s){
	return s+"                                     ";
}

template<typename USER_CODE>
class To_roborio
{
	//TODO: see if these still have to all be pointers or if there's some alternative w/ the roboRIO
	frc::Solenoid *solenoid[Robot_outputs::SOLENOIDS];
	DIO_controls digital_io;
	frc::VictorSP *pwm[Robot_outputs::PWMS];
	frc::Relay *relay[Robot_outputs::RELAYS];
	frc::AnalogInput *analog_in[Robot_inputs::ANALOG_INPUTS];
	int error_code;
	USER_CODE main;
	Talon_srx_controls talon_srx_controls;
	Navx_control navx_control;
	I2C_control i2c_control;
	//DriverStationLCD *lcd;
	//NetworkTable *table;
	//Gyro *gyro;
	frc::PowerDistributionPanel *power;
	Pump_control pump_control;
	//frc::Compressor *compressor;
	frc::DriverStation& driver_station;
	MotorCurrentMonitor dbl_monitor ;
	MotorCurrentMonitor dbr_monitor ;
	MotorCurrentMonitor grabber_monitor ;
	MotorCurrentMonitor lift_monitor ;
	MotorCurrentMonitor lin_monitor ;
	MotorCurrentMonitor rin_monitor ;
	std::ofstream null_stream;
	
public:
	To_roborio():error_code(0),
				 navx_control(frc::SPI::Port::kMXP),
				 i2c_control(8),
				 driver_station(frc::DriverStation::GetInstance()),
				 dbl_monitor(3),
				 dbr_monitor(3),
				 grabber_monitor(1),
				 lift_monitor(2),
				 lin_monitor(1),
				 rin_monitor(1),
			 null_stream("/dev/null")
	{
		//////////////////////////////////////////////////////////////////////
		// Initialize the message logger
		//////////////////////////////////////////////////////////////////////
		
		messageLogger &logger = messageLogger::get();
		logger.enableType(messageLogger::messageType::error);
		logger.enableType(messageLogger::messageType::warning);
		logger.enableType(messageLogger::messageType::info);
		logger.enableType(messageLogger::messageType::debug);


		//
		// Decide what subsystems you want to see
		//

#ifdef DEBUG
		logger.enableSubsystem(SUBSYSTEM_AUTONOMOUS);
		logger.enableSubsystem(SUBSYSTEM_DRIVEBASE);
		//logger.enableSubsystem(SUBSYSTEM_LIFTER);
		//logger.enableSubsystem(SUBSYSTEM_DRIVEBASE_RAW_DATA);
		//logger.enableSubsystem(SUBSYSTEM_LIFTER_TUNING);
		//logger.enableSubsystem(SUBSYSTEM_TIMING);
		//logger.enableSubsystem(SUBSYSTEM_GRABBER);
		//logger.enableSubsystem(SUBSYSTEM_GRABBER_TUNING);
		//logger.enableSubsystem(SUBSYSTEM_PDPCURRENTS);
		//logger.enableSubsystem(SUBSYSTEM_DIGITALIO);
		//logger.enableSubsystem(SUBSYSTEM_TELEOP);
		//logger.enableSubsystem(SUBSYSTEM_PANEL);
		//logger.enableSubsystem(SUBSYSTEM_SOLENOIDS);
#else
		//
		// In competition mode, we always want the drivebase and auto mode
		// debug information
		//
		logger.enableSubsystem(SUBSYSTEM_DRIVEBASE);
		logger.enableSubsystem(SUBSYSTEM_AUTONOMOUS);
#endif

		std::shared_ptr<messageLoggerDest> dest_p ;

#ifdef DEBUG
		//
		// We only want printouts on COUT when we are debugging
		// In competition mode, this information goes to a log file on
		// the USB stick
		//
		//dest_p = std::make_shared<messageDestStream>(std::cout) ;
		//logger.addDestination(dest_p) ;
#endif
		
		//
		// This is where the roborio places the first USB flashd drive it
		// finds.  Other drives are placed at /V, /W, /X.  The devices are
		// actually mounted at /media/sd*, and a symbolic link is created
		// to /U.
		//
		std::string flashdrive("/u/") ;
		std::string logname("logfile_") ;
		dest_p = std::make_shared<messageDestSeqFile>(flashdrive,logname) ;
		logger.addDestination(dest_p) ;

		//
		// Send warnings and errors to the driver station
		//
		dest_p = std::make_shared<messageDestDS>() ;
		logger.addDestination(dest_p) ;


		//////////////////////////////////////////////////////////////////////
		// Initialize the hardware
		//////////////////////////////////////////////////////////////////////

		power = new frc::PowerDistributionPanel();

		for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
			solenoid[i]=new frc::Solenoid(i);//don't know of any way to determine module number, so just take the default one.
			if(!solenoid[i]) error_code|=8;
		}
		talon_srx_controls.init();
#ifdef THEREMIN
		talon_srx_controls.set_inverted(2);
		talon_srx_controls.set_inverted(3);
#endif
	
#ifdef CLAYMORE
		talon_srx_controls.set_inverted(0);
		talon_srx_controls.set_inverted(1);
		talon_srx_controls.set_inverted(2);
#endif				
		for(unsigned i=0;i<Robot_outputs::PWMS;i++){
			pwm[i]=new frc::VictorSP(i);
			if(!pwm[i]) error_code|=8;
		}

		for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
			relay[i]=new frc::Relay(i);
			if(!relay[i]) error_code|=8;
		}

		for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
			analog_in[i]=new frc::AnalogInput(i);
			if(!analog_in[i]) error_code|=8;
		}

		//////////////////////////////////////////////////////////////////////
		// Read the parameters file
		//////////////////////////////////////////////////////////////////////
		
		paramsInput *params_p = paramsInput::get() ;

		if (params_p->readFile(param_file_name_p))
		{
			logger.startMessage(messageLogger::messageType::info) ;
			logger << "Parameters file '" ;
			logger << param_file_name_p << "' was read sucessfully" ;
			logger.endMessage() ;
		}
		else
		{
			logger.startMessage(messageLogger::messageType::error) ;
			logger << "Parameters file '" ;
			logger << param_file_name_p << "' read failed" ;
			logger.endMessage() ;
		}

		//////////////////////////////////////////////////////////////////////
		// Setup the motor monitors
		//////////////////////////////////////////////////////////////////////
		double val ;
		
		dbl_monitor.setMeasurementsToAverage(5) ;
		val = params_p->getValue("power:drivebase:left:variance", 0.25) ;
		dbl_monitor.setVarianceThreshold(val) ;
		val = params_p->getValue("power:drivebase:left:max", 10.0) ;
		dbl_monitor.setMaxCurrent(10.0) ;
		
		dbr_monitor.setMeasurementsToAverage(5) ;
		val = params_p->getValue("power:drivebase:right:variance", 0.25) ;
		dbr_monitor.setVarianceThreshold(val) ;
		val = params_p->getValue("power:drivebase:right:max", 10.0) ;
		dbr_monitor.setMaxCurrent(10.0) ;

		grabber_monitor.setMeasurementsToAverage(5) ;
		val = params_p->getValue("power:grabber:max", 10.0) ;
		grabber_monitor.setMaxCurrent(val) ;

		lift_monitor.setMeasurementsToAverage(5) ;
		val = params_p->getValue("power:lifter:variance", 0.25) ;
		lift_monitor.setVarianceThreshold(val) ;
		val = params_p->getValue("power:lifter:max", 10.0) ;
		lift_monitor.setMaxCurrent(val) ;

		lin_monitor.setMeasurementsToAverage(5) ;
		val = params_p->getValue("power:intake:left:max", 10.0) ;
		lin_monitor.setMaxCurrent(val) ;

		rin_monitor.setMeasurementsToAverage(5) ;
		val = params_p->getValue("power:intake:right:max", 10.0) ;
		rin_monitor.setMaxCurrent(val) ;


		//////////////////////////////////////////////////////////////////////
		// Initialize subsystems that have controllers
		//////////////////////////////////////////////////////////////////////
		Drivebase::drivebase_controller.setParams(params_p);	
		Lifter::lifter_controller.init() ;
		Grabber::grabber_controller.init() ;

		logger.startMessage(messageLogger::messageType::info) ;
		logger << "Initialization complete" ;
		logger.endMessage() ;
	}
	
	int read_analog(Robot_inputs &r){
		int error=0;
		for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
			if(analog_in[i]){
				r.analog[i]=analog_in[i]->GetVoltage();
			}else{
				error=64;
			}
		}
		return error;
	}

	DS_info read_ds_info(){
		DS_info ds_info;
		ds_info.connected=driver_station.IsDSAttached();
		frc::DriverStation::Alliance ds_alliance=driver_station.GetAlliance();
		ds_info.alliance=[&]{
			switch(ds_alliance){
				case frc::DriverStation::Alliance::kRed: return Alliance::RED;
				case frc::DriverStation::Alliance::kBlue: return Alliance::BLUE;
				case frc::DriverStation::Alliance::kInvalid: return Alliance::INVALID;
				default: assert(0);
			}
		}();
		ds_info.location=driver_station.GetLocation();
		std::string game_data = driver_station.GetGameSpecificMessage();
		ds_info.near_switch_left = game_data[0] == 'L';
		ds_info.scale_left = game_data[1] == 'L';
		ds_info.far_switch_left = game_data[2] == 'L';
		ds_info.battery_voltage = driver_station.GetBatteryVoltage();
		return ds_info;
	}

	Navx_input read_navx(){
		return navx_control.get();
	}

	pair<Robot_inputs,int> read(Robot_mode robot_mode){
		int error_code=0;
		Robot_inputs r;
		r.robot_mode=robot_mode;
		r.now=frc::Timer::GetFPGATimestamp();	
		r.ds_info=read_ds_info();
		error_code|=read_joysticks(r, driver_station);
		error_code|=read_analog(r);
		//error_code|=read_driver_station(r.driver_station);
		r.current=read_currents();
		r.navx=read_navx();
		r.input_params = paramsInput::get() ;
		return make_pair(r,error_code);
	}
	array<double,Robot_inputs::CURRENT> read_currents(){
		array<double,Robot_inputs::CURRENT> current;
		for(unsigned x = 0;x < current.size();x++){
			if(power){
				current[x] = power->GetCurrent(x);
			}else{
				current[x] = -9001;
			}
		}

		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_PDPCURRENTS) ;
		logger << "PDP Currents: " ;
		for(size_t i = 0 ; i < current.size() ; i++)
		{
			if (i != 0)
				logger << ", " ;
			logger << i << "=" << current[i] ;
		}
		logger.endMessage() ;

#ifdef CHECK_MOTORS
		dbl_monitor.logNewMeasurement(MotorCurrentMonitor::Measurement{current[13], current[14], current[15]}) ;
		dbr_monitor.logNewMeasurement(MotorCurrentMonitor::Measurement{current[0], current[1], current[2]}) ;
		grabber_monitor.logNewMeasurement(MotorCurrentMonitor::Measurement{current[10]}) ;
		lift_monitor.logNewMeasurement(MotorCurrentMonitor::Measurement{current[3], current[12]}) ;
		lin_monitor.logNewMeasurement(MotorCurrentMonitor::Measurement{current[11]}) ;
		rin_monitor.logNewMeasurement(MotorCurrentMonitor::Measurement{current[4]}) ;
#endif
		
		return current;
	}
	int set_solenoid(unsigned i,Solenoid_output v){
		if(i>=Robot_outputs::SOLENOIDS) return 1;
		if(!solenoid[i]) return 2;
		solenoid[i]->Set(v);
		return 0;
	}

	int set_pwm(unsigned i,Pwm_output p){
		if(i>=Robot_outputs::PWMS) return 1;
		if(!pwm[i]) return 2;
		pwm[i]->Set(p);
		return 0;
	}

	int set_relay(unsigned i,Relay_output out){
		if(i>=Robot_outputs::RELAYS) return 1;
		if(!relay[i]) return 2;
		relay[i]->Set([=](){
			if(out==Relay_output::_00) return frc::Relay::kOff;
			if(out==Relay_output::_01) return frc::Relay::kReverse;
			if(out==Relay_output::_10) return frc::Relay::kForward;
			//Assuming RELAY_11
			return frc::Relay::kOn;
		}());
		return 0;
	}

	int set_outputs(Robot_outputs out,bool /*enabled*/){
		int error_code=0;
		for(unsigned i=0;i<Robot_outputs::PWMS;i++){
			int r=set_pwm(i,out.pwm[i]);
			if(r) error_code|=2;
		}

		messageLogger &logger = messageLogger::get();
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_SOLENOIDS) ;
		logger << "Solenoids: ";
		for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
			logger << out.solenoid[i];
			logger << ", ";
			int r=set_solenoid(i,out.solenoid[i]);
			if(r) error_code|=16;
		}
		logger.endMessage();

		for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
			int r=set_relay(i,out.relay[i]);
			if(r) error_code|=32;
		}
		
		digital_io.set(out.digital_io);
	
		{	
			frc::Joystick panel(Panel::PORT);
			for(unsigned i=0;i<Driver_station_output::DIGITAL_OUTPUTS;i++){
				panel.SetOutput(i+1, out.driver_station.digital[i]);
			}
		}
		{
			Checked_array<bool,Robot_outputs::TALON_SRX_OUTPUTS> enable_all;
			for(unsigned int i=0; i<Robot_outputs::TALON_SRX_OUTPUTS; i++){
				enable_all[i]=true;
			}
			talon_srx_controls.set(out.talon_srx,enable_all); 
		}
	
		i2c_control.write(out.i2c);	
	
		pump_control.set(out.pump);

		navx_control.set(out.navx);	

		return error_code;
	}
	

	void run(Robot_inputs in){
		messageLogger &logger = messageLogger::get();
		
		
		double start = frc::Timer::GetFPGATimestamp() ;
		Robot_outputs out=main(in/*,print_stream*/);
		
		double elapsed = frc::Timer::GetFPGATimestamp() - start  ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "main robot loop " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
		
		start = frc::Timer::GetFPGATimestamp() ;
		set_outputs(out,in.robot_mode.enabled);
		elapsed = frc::Timer::GetFPGATimestamp() - start ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "Set hardware outputs " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
	}
	
	void run(Robot_mode mode){
		messageLogger &logger = messageLogger::get();
		double start = frc::Timer::GetFPGATimestamp() ;
		double elapsed , last ;
		
		pair<Robot_inputs,int> in1=read(mode);
		
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "---------------------------------------------------------------\n" ;
		logger << "Reading hardware inputs" ;
		logger.endMessage() ;

		
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - start ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "    read base inputs " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
		elapsed = last ;
		
		Robot_inputs in=in1.first;

		in.navx=read_navx();
		
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "    navx " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
		elapsed = last ;

		error_code|=in1.second;
		in.digital_io=digital_io.get();
		
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DIGITALIO) ;
		logger << "Ins: " ;
		for(size_t i = 0 ; i < in.digital_io.in.size() ; i++)
		{
			if (i != 0)
				logger << ", " ;
			logger << i << "=" ;
			Digital_in din = in.digital_io.in[i] ;
			if (din == Digital_in::OUTPUT)
				logger << "OUTPUT" ;
			else if (din == Digital_in::_0)
				logger << "0" ;
			else if (din == Digital_in::_1)
				logger << "1" ;
			else if (din == Digital_in::ENCODER)
				logger << "ENCODER" ;
			else
				logger << "????" ;
		}
		logger.endMessage() ;
		
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_DIGITALIO) ;
		logger << "Encoders:" ;
		for(size_t i = 0 ; i < in.digital_io.encoder.size() ; i++)
		{
			if (i != 0)
				logger << ", " ;
			logger << " " << i << " " ;
			if (in.digital_io.encoder[i])
				logger << *in.digital_io.encoder[i] ;
			else
				logger << "N/A" ;
		}
		logger.endMessage() ;
		
		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "    digital io inputs " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
		
		elapsed = last ;
		
		in.talon_srx=talon_srx_controls.get();

		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "    talon srx inputs " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
		elapsed = last ;
		
		in.pump=pump_control.get();

		last = frc::Timer::GetFPGATimestamp() ;
		elapsed = last - elapsed ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "    pump inputs " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
		elapsed = last ;
		
		elapsed = elapsed - start ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TIMING) ;
		logger << "read inputs total " << elapsed * 1000 << " msec" ;
		logger.endMessage() ;
		
		run(in);
	}
};

template<typename USER_CODE>
class Robot_adapter: public frc::SampleRobot{
	To_roborio<USER_CODE> u;

	double looptime = 0.05 ;

	void RobotInit(){}
	
	void Autonomous(void)
	{
		//////////////////////////////////////////////////////////////////////
		// Send match information to the message logger
		//////////////////////////////////////////////////////////////////////
		messageLogger &logger = messageLogger::get();
		DriverStation &ds = DriverStation::GetInstance() ;
		logger.startMessage(messageLogger::messageType::info) ;
		logger << "Match Specific Data:\n" ;
		logger << "    GameSpecificData: " << ds.GetGameSpecificMessage() << "\n" ;
		logger << "          Event Name: " << ds.GetEventName() << "\n" ;
		logger << "          Match Type: " ;
		switch(ds.GetMatchType())
		{
		case DriverStation::kNone:
			logger << "kNone\n" ;
			break ;
		case DriverStation::kPractice:
			logger << "kPractice\n" ;
			break ;
		case DriverStation::kQualification:
			logger << "kQualification\n" ;
			break ;
		case DriverStation::kElimination:
			logger << "kElimination\n" ;
			break ;
		default:
			logger << "Unknown (bad data from driver station)\n" ;
			break ;
		} ;
		logger << "        Match Number: " << ds.GetMatchNumber() << "\n" ;
		
		logger << "            Alliance: " ;
		switch(ds.GetAlliance())
		{
		case DriverStation::kRed:
			logger << "kRed\n" ;
			break ;
		case DriverStation::kBlue:
			logger << "kBlue\n" ;
			break ;
		case DriverStation::kInvalid:
			logger << "kInvalid\n" ;
			break ;
		default:
			logger << "Unknown (bad data from driver station)\n" ;
			break ;
		}
		logger << "            Location: " << ds.GetLocation() << "\n" ;
		logger.endMessage() ;
		
		while(IsAutonomous() && IsEnabled()){
			//might need a loop here

			double start = frc::Timer::GetFPGATimestamp() ;
			Robot_mode mode;
			mode.autonomous=1;
			mode.enabled=IsEnabled();
			u.run(mode);

			double elapsed = frc::Timer::GetFPGATimestamp() - start ;
			if (elapsed < looptime)
				frc::Wait(looptime - elapsed) ;
			else
			{
				std::cout << "Loop exceeded loop time, actual " << elapsed * 1000 << " msec" << std::endl ;
				std::cout << std::endl << std::endl ;
			}
		}
	}

	void OperatorControl(void)
	{
		double looptime = 0.05 ;
		
		//should see what happens when we get rid of this loop.  
		while (IsOperatorControl() && IsEnabled())
		{
			double start = frc::Timer::GetFPGATimestamp() ;
			Robot_mode r;
			r.enabled=IsEnabled();
			u.run(r);

#ifdef OLD_LOOP_TIMING
			//should see what happpens when this wait is removed.
			frc::Wait(0.005);// Wait 5 ms so we don't hog CPU cycle time
#else
			double elapsed = frc::Timer::GetFPGATimestamp() - start ;
			if (elapsed < looptime)
				frc::Wait(looptime - elapsed) ;
			else
			{
				std::cout << "Loop exceeded loop time, actual " << elapsed * 1000 << " msec" << std::endl ;
				std::cout << std::endl << std::endl ;
			}
#endif			
		}
	}
	
	//Runs during test mode
	void Test(){
		while(IsTest() && IsEnabled()){
			Robot_mode r;
			r.enabled=IsEnabled();
			u.run(r);
			
			frc::Wait(0.005);
		}
	}
	
	void Disabled(){
		while(IsDisabled()){
			Robot_mode r;
			r.autonomous=IsAutonomous();
			u.run(r);

			frc::Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(Robot_adapter<Main>);
