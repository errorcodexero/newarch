#include "frc/WPILib.h"
#include "../control/main.h"
#include "dio_control.h"
#include "talon_srx_control.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

using namespace std;
using namespace frc ;

#ifdef NOTYET
void SendWOL (void)
{
	int udpSocket;
	if ((udpSocket = socket(AF_INET, SOCK_DGRAM, 0)) == ERROR){
		perror("Failed to create a UDP socket!");
		exit(1);
	}

	// you need to set this so you can broadcast:
	int broadcast = 1;
	if (setsockopt(udpSocket, SOL_SOCKET, SO_BROADCAST, reinterpret_cast<char*>(&broadcast), sizeof(broadcast)) == ERROR){
		perror("setsockopt (SO_BROADCAST)");
		exit(1);
	}

	struct sockaddr_in udpClient;
	udpClient.sin_family      = AF_INET;
	udpClient.sin_addr.s_addr = INADDR_ANY;
	udpClient.sin_port        = 0;

	if (bind(udpSocket, (struct sockaddr*) &udpClient, sizeof(udpClient)) == ERROR){
		perror("Failed to bind the cRioServer port!");
		close (udpSocket);
		exit(1);
	}

	// build the magic packet:
	//
	//     6 * 255 or (0xff)
	//    16 * MAC Address of target PC

	unsigned char tosend[102];
	unsigned char mac[6];

	// first 6 bytes of 255:
	for (int i = 0; i < 6; i++) 
		tosend[i] = 0xFF;

	// Store mac address of the NUC (ec:a8:6b:fe:fc:e6) :
	mac[0] = 0xec;
	mac[1] = 0xa8;
	mac[2] = 0x6b;
	mac[3] = 0xfe;
	mac[4] = 0xfc;
	mac[5] = 0xe6;

	// append it 16 times to packet:
	for(int i = 1; i <= 16; i++) 
		memcpy(&tosend[i * 6], &mac, 6 * sizeof(unsigned char));

	// set server end point (the broadcast addres):
	struct sockaddr_in udpServer;
	udpServer.sin_family      = AF_INET;
	udpServer.sin_addr.s_addr = inet_addr("10.14.25.255");
	udpServer.sin_port        = htons(9);

	// send the packet:
	sendto(udpSocket, reinterpret_cast<char*>(&tosend), sizeof(unsigned char) * 102, 0, (struct sockaddr*)&udpServer, sizeof(udpServer));

	cerr << "Sent WOL packet to NUC." << endl;

	close(udpSocket);
}
#endif

Joystick_data read_joystick(DriverStation& ds,int port){
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
		unsigned axes=std::min((unsigned)JOY_AXES,(unsigned)lim);
		for(unsigned i=0;i<axes;i++){
			//r.axis[i]=ds.GetStickAxis(port+1,i+1);
			//cerr<<"Reading Port "<<port<<" Axis "<<i<<endl<<flush;
			r.axis[i]=ds.GetStickAxis(port,i);
			//cerr<<r.axis[i]<<endl<<flush;
		}
	}
	auto lim=ds.GetStickButtonCount(port);
	assert(lim>=0);
	const auto buttons=std::min((unsigned)JOY_BUTTONS,(unsigned)lim);
	for(unsigned i=0;i<buttons;i++){
		//if(buttons&(1<<i)) r.button[i]=1;
		r.button[i]=ds.GetStickButton(port,i+1);
	}
	return r;
}

int read_joysticks(Robot_inputs &r){
	DriverStation &ds=DriverStation::GetInstance();
	//if(!ds) return 4;
	for(unsigned i=0;i<r.JOYSTICKS;i++){
		r.joystick[i]=read_joystick(ds,i);
	}
	return 0;
}

/*
void setoutputs_joysticks(Robot_inputs &r){
	DriverStation *ds=DriverStation::GetInstance();
	if(unsigned i=0;i<r.JOYSTICKS;i++){
		r.joystick[i]=SetOutput(1,1);			 
	}
}
*/

/*for cRIO
int read_analog(Robot_inputs& r){
	AnalogModule *am=AnalogModule::GetInstance(1);
	if(am){
		for(unsigned i=0;i<r.ANALOG_INPUTS;i++){
			r.analog[i]=am->GetAverageVoltage(i+1);
		}
		return 0;
	}else{
		for(unsigned i=0;i<r.ANALOG_INPUTS;i++){
			//could make this NAN.
			r.analog[i]=0;
		}
		return 64;
	}
}*/

/*DriverStationEnhancedIO &get_driver_station(){
	DriverStation *ds=DriverStation::GetInstance();
	if(!ds) return NULL;
	return ds->GetEnhancedIO();
}*/

/*int read_driver_station(Driver_station_input& r){
	DriverStation *ds=DriverStation::GetInstance();
	if(!ds) return 2048;
	//Cyprus board isn't supported and a replacement is not yet available.
	//DriverStationEnhancedIO &en=ds->GetEnhancedIO();
	for(unsigned i=0;i<r.ANALOG_INPUTS;i++){
		r.analog[i]=0;//en.GetAnalogIn(i+1);//Causing a LOT of printouts when the DS is disconnected

	}
	for(unsigned i=0;i<r.DIGITAL_INPUTS;i++){
		r.digital[i]=0;//en.GetDigital(i+1);//Same as above ^^
	}
	return 0;
}*/

//it might make sense to put this in the Robot_inputs structure.  
Volt battery_voltage(){
	auto &d=DriverStation::GetInstance();
	//AnalogModule *am=AnalogModule::GetInstance(DriverStation::kBatteryModuleNumber);
	/*if(!d){
		return 18; //this should look surprising
		//but maybe should actually return NAN.
	}*/
	/*float f=am->GetAverageVoltage(DriverStation::kBatteryChannel);
	return f * (1680.0 / 1000.0);//copied from WPIlib's DriverStation.cpp*/
	return d.GetBatteryVoltage();
}

int find_solenoid_module(){
	/*for(unsigned i=0;i<8;i++){
		if(nLoadOut::getModulePresence(nLoadOut::kModuleType_Solenoid,i)){
			return i;
		}
	}*/
	return -1;
}

/*For cRIO:
int set_pwm(unsigned i,Pwm_output p){
	if(i>=Robot_outputs::PWMS) return 1;
	DigitalModule *dm=digital_module();
	if(!dm) return 2;
	//this may be needed: dm->EnablePWM();
	dm->SetPWMRate(i+1,p);
	return 0;
}*/

/*For cRIO:
int set_relay(unsigned i,Relay_output v){
	if(i>=Robot_outputs::RELAYS) return 1;
	DigitalModule *dm=digital_module();
	if(!dm) return 2;
	dm->SetRelayForward(i+1,v==RELAY_10 || v==RELAY_11);
	dm->SetRelayReverse(i+1,v==RELAY_01 || v==RELAY_11);
	return 0;
}*/

/*int demo(int a0,int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9){
	cerr<<"In demo\n";
}*/
int demo(...){
	cerr<<"In demo\n";
	return 0;
}

string space_out(string s){
	return s+"                                     ";
}

template<typename USER_CODE>
class To_crio
{
	//todo: see if these still have to all be pointers or if there's some alternative w/ the roboRIO
	Solenoid *solenoid[Robot_outputs::SOLENOIDS];
	DIO_controls digital_io;
	VictorSP *pwm[Robot_outputs::PWMS];
	Relay *relay[Robot_outputs::RELAYS];
	AnalogInput *analog_in[Robot_inputs::ANALOG_INPUTS];
	int error_code;
	USER_CODE main;
	int skipped;
	Talon_srx_controls talon_srx_controls;
	//Jag_control jaguar[Robot_outputs::CAN_JAGUARS];
	//DriverStationLCD *lcd;
	//NetworkTable *table;
	//Gyro *gyro;
	PowerDistributionPanel *power;
	Compressor *compressor;
public:
	To_crio():error_code(0),skipped(0)//,gyro(NULL)
	{
		power = new PowerDistributionPanel();
		// Wake the NUC by sending a Wake-on-LAN magic UDP packet:
		//SendWOL();

		//int solenoid_module=find_solenoid_module();
		for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
			/*if(solenoid_module==-1){
				solenoid[i]=NULL;
			}else{*/
				//solenoid[i]=new Solenoid(solenoid_module+1,i+1);
				solenoid[i]=new Solenoid(i);//don't know of any way to determine module number, so just take the default one.
				//solenoid[i]=new Solenoid(i+1);
				if(!solenoid[i]) error_code|=8;
			//}
		}
		talon_srx_controls.init();
		
		for(unsigned i=0;i<Robot_outputs::PWMS;i++){
			pwm[i]=new VictorSP(i);//untested
			if(!pwm[i]) error_code|=8;
		}

		for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
			relay[i]=new Relay(i);
			if(!relay[i]) error_code|=8;
		}

		for(unsigned i=0;i<Robot_inputs::ANALOG_INPUTS;i++){
			analog_in[i]=new AnalogInput(i);
			if(!analog_in[i]) error_code|=8;
		}

		//for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
			//it just so happens that our four jags are numbered 1-4.  This is contrary to the IO map document that we have and also contrary to the recommendations in the Jaguar documentation (which recomends not to use the number 1 because it's the factory default).  We should change this at some point.  
			//jaguar[i].init(i+1);
		//}
		//CANJaguar::UpdateSyncGroup(Jag_control::SYNC_GROUP);
		/*
		for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
			int r=digital_io[i].set_channel(i);
			if(r) error_code|=256;
			//digital_in[i]=new DigitalInput(i+1);
		}*/
		
		/*lcd=DriverStationLCD::GetInstance();
		if(!lcd) error_code|=512;*/

		//table = NetworkTable::GetTable("crio");
		/*gyro=new Gyro(1);
		if(gyro){
			//gyro->InitGyro();
		}else{
			//TODO: Note this somehow.
		}*/

		compressor=new Compressor();
		if(compressor){
			//for now I'm assuming that this means that it will run automatically.  I haven't seen any documentation that says what this does though.
			compressor->Start();
		}else{
			error_code|=512;
		}
		
		//Slave
		
		cout<<"Initialization Complete."<<endl<<flush;
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

	pair<Robot_inputs,int> read(Robot_mode robot_mode){
		int error_code=0;
		Robot_inputs r;
		r.robot_mode=robot_mode;
		r.now=Timer::GetFPGATimestamp();
		error_code|=read_joysticks(r);
		error_code|=read_analog(r);
		//error_code|=read_driver_station(r.driver_station);
		r.current = read_currents();
		return make_pair(r,error_code);
	}
	//PowerDistributionPanel power;
	array<double,Robot_inputs::CURRENT> read_currents(){
		array<double,Robot_inputs::CURRENT> current;
		for(unsigned x = 0;x < current.size();x++){
			if(power){
				current[x] = power->GetCurrent(x);
			}else{
				current[x] = -9001;
			}
		}		
		return current;
	}
	int set_solenoid(unsigned i,Solenoid_output v){
		if(i>=Robot_outputs::SOLENOIDS) return 1;
		if(!solenoid[i]) return 2;
		solenoid[i]->Set(v);
		return 0;
	}

	int set_pwm(unsigned i,Pwm_output p){
		//cout<<"set_pwm "<<i<<" "<<(int)p<<"\n";
		if(i>=Robot_outputs::PWMS) return 1;
		if(!pwm[i]) return 2;
		//pwm[i]->SetRaw(p);//we're assuming that the values taken here are the same as given before
		/*if(i==0){
			pwm[i]->SetRaw(50);
		}else{
			pwm[i]->SetRaw((int)p-128);
		}*/
		//pwm[i]->Set(((float)p-128)/128);
		pwm[i]->Set(p);
		//pwm[i]->SetRaw(0);
		return 0;
	}

	int set_relay(unsigned i,Relay_output out){
		if(i>=Robot_outputs::RELAYS) return 1;
		if(!relay[i]) return 2;
		relay[i]->Set([=](){
			if(out==Relay_output::_00) return Relay::kOff;
			if(out==Relay_output::_01) return Relay::kReverse;
			if(out==Relay_output::_10) return Relay::kForward;
			//Assuming RELAY_11
			return Relay::kReverse;
		}());
		return 0;
	}

	int set_outputs(Robot_outputs out,bool /*enabled*/){
		int error_code=0;
		for(unsigned i=0;i<Robot_outputs::PWMS;i++){
			int r=set_pwm(i,out.pwm[i]);
			if(r) error_code|=2;
		}

		/*cout<<"cout1\n";
		cout.flush();
		cerr<<"cerr1\n";
		cerr.flush();
		usleep(1000*5);*/

		/*if(0){
			//The first column is numbered 1.
			lcd->Printf(DriverStationLCD::kUser_Line1,1,"%s",space_out(out.driver_station.lcd.line[0]).c_str());
			lcd->Printf(DriverStationLCD::kUser_Line2,1,"%s",space_out(out.driver_station.lcd.line[1]).c_str());
			lcd->Printf(DriverStationLCD::kUser_Line3,1,"%s",space_out(out.driver_station.lcd.line[2]).c_str());
			lcd->Printf(DriverStationLCD::kUser_Line4,1,"%s",space_out(out.driver_station.lcd.line[3]).c_str());
			lcd->Printf(DriverStationLCD::kUser_Line5,1,"%s",space_out(out.driver_station.lcd.line[4]).c_str());
			lcd->Printf(DriverStationLCD::kUser_Line6,1,"%s",space_out(out.driver_station.lcd.line[5]).c_str());
			lcd->UpdateLCD();
		}else{
			cerr<<"lcd is null\r\n";
		}*/
		for(unsigned i=0;i<Robot_outputs::SOLENOIDS;i++){
			int r=set_solenoid(i,out.solenoid[i]);
			if(r) error_code|=16;
		}
		for(unsigned i=0;i<Robot_outputs::RELAYS;i++){
			int r=set_relay(i,out.relay[i]);
			if(r) error_code|=32;
		}
		for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
			/*int r=digital_io[i].set(out.digital_io[i]);
			if(r) error_code|=512;*/
		}
		digital_io.set(out.digital_io);
		
		Joystick panel(2);
		for(unsigned i=0;i<Panel_outputs::PANEL_OUTPUTS;i++){
			panel.SetOutput(out.panel_output[i].port, out.panel_output[i].value);
		}
			
		SmartDashboard::PutBoolean("Ball: ", out.panel_output[Panel_outputs::BOULDER].value);
		SmartDashboard::PutBoolean("Spun Up: ", out.panel_output[Panel_outputs::SPUN_UP].value);
		
		//cout << "d_io: " << digital_io << endl << "o.d.io: " << out.digital_io << endl ;

		{
			Checked_array<bool,Robot_outputs::TALON_SRX_OUTPUTS> enable_all;
			for(unsigned int i=0; i<Robot_outputs::TALON_SRX_OUTPUTS; i++){
				enable_all[i]=true;
			}
			talon_srx_controls.set(out.talon_srx,enable_all); 
		}
		{
			/*DriverStation *ds=DriverStation::GetInstance();
			if(ds){
				DriverStationEnhancedIO &d=ds->GetEnhancedIO();
				for(unsigned i=0;i<8;i++){
					d.SetDigitalConfig(i+1,DriverStationEnhancedIO::kInputPullUp);
				}
				for(unsigned i=0;i<Driver_station_output::DIGITAL_OUTPUTS;i++){
					d.SetDigitalConfig(i+1+8,DriverStationEnhancedIO::kOutput);
					d.SetDigitalOutput(i+1+8,out.driver_station.digital[i]);
				}
			}else{
				error_code|=2048;
			}*/
		}

		//for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
			//jaguar[i].set(out.jaguar[i],enabled);
			//cerr<<jaguar[i]<<"\n";
			//cerr<<"Are we enabled?"<<enabled<<"\n";
			//cerr<<out.jaguar[i]<<"\n";
			//cerr<<jaguar[i].jaguar->GetSpeed()<<"\n";
		//}
		/*	cerr<<"\n"<<jaguar[0].jaguar->GetSpeed()<<"\n";
			cerr<<jaguar[1].jaguar->GetSpeed()<<"\n";
			cerr<<jaguar[2].jaguar->GetSpeed()<<"\n";
			cerr<<jaguar[3].jaguar->GetSpeed()<<"\n";*/
		/*
		float kP = 1.000;
		float kI = 0.005;
		float kD = 0.000;
		for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
			if(out.jaguar[i].controlSpeed != controlSpeed[i]){
				jaguar[i]->ChangeControlMode(out.jaguar[i].controlSpeed ? CANJaguar::kSpeed : CANJaguar::kPercentVbus);
				controlSpeed[i] = out.jaguar[i].controlSpeed;
				jaguar[1]->SetPID(kP, kI, kD); //Need to add refernces to what PID is
				jaguar[1]->EnableControl();
				jaguar[i]->SetExpiration(2.0);
				
			}
			if(out.jaguar[i].controlSpeed){
				jaguar[i]->Set(out.jaguar[i].speed, SYNC_GROUP);
			}else {
				jaguar[i]->Set(out.jaguar[i].voltage, SYNC_GROUP);
			}
		}
		*/
		//rate limiting the output  
		if(skipped==0){
			//cerr<<"Ran "<<mode<<"\r\n";
			/*cerr<<in<<"\r\n";
			cerr<<main<<"\r\n";*/
			//cerr<<"errorcode="<<error_code<<"\n";
			//cerr.flush();
			//cerr<<out<<"\r\n";
			/*cerr<<"Going to start task\r\n";
			int priority=Task::kDefaultPriority;
			int stack_size=64000;//copied from Task
			INT32 task_id=taskSpawn("tDemo",priority,VX_FP_TASK,stack_size,demo,0,1,2,3,4,5,6,7,8,9);
			cerr<<"Task id="<<task_id<<" error="<<ERROR<<"\r\n";*/
		}
		
		skipped=(skipped+1)%100;
		return error_code;
	}
	

	void run(Robot_inputs in){
		static int print_num=0;
		Robot_outputs out=main(in);
		/*const int PRINT_SPEED=10;
		if((print_num%PRINT_SPEED)==0){
			cout<<"in: "<<in<<"\n";
			cout<<"main: "<<main<<"\n";
			cout<<"out: "<<out<<"\n";
			//cout<<"talon_srx_controls: "<<talon_srx_controls<<"\n";
			cout<<"CLEAR_SCREEN\n";
		}*/
		int x=set_outputs(out,in.robot_mode.enabled);
		if(x) cout<<"x was:"<<x<<"\n";
		/*static int i=0;
		if(!i){
			for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
				//cerr<<"dio"<<i<<":"<<digital_io[i]<<"\n";
			}
		}
		i=(i+1)%100;*/
		print_num++;
	}
	
	void run(Robot_mode mode){
		/*cerr<<"Going to set LCD\n";
		cerr.flush();
		
		ALON//string s="hell owrld\n";
		static const unsigned SIZE=USER_DS_LCD_DATA_SIZE;
		char s[SIZE];
		memset(s,' ',SIZE);
		s[0]='a';
		s[1]='b';
		//setUserDsLcdData(s,SIZE,0);
		DriverStationLCD *a=DriverStationLCD::GetInstance();
		if(!a){
			cerr<<"LCD null\n";
		}else{
			a->PrintfLine(DriverStationLCD::kUser_Line1,"d");
			a->UpdateLCD();
			//a->PrintLine("hello");
		}
		cerr<<"Done with LCD\n";
		cerr.flush();*/
		

		pair<Robot_inputs,int> in1=read(mode);
		Robot_inputs in=in1.first;
		error_code|=in1.second;
		/*for(unsigned i=0;i<Robot_outputs::CAN_JAGUARS;i++){
			//in.jaguar[i]=.3;//jaguar[i].get();
		}*/
		for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
			//in.digital_io[i]=digital_io[i].get();
		}
		in.digital_io=digital_io.get();
		in.talon_srx=talon_srx_controls.get();
		//cout<<"in:"<<in<<"\n";
		//}
		/*if(gyro){
			in.orientation=gyro->GetAngle();
		}*/
		run(in);

/*                // Network Table update:
		enum DsMode_t { DS_OTHER = 0, DS_AUTO = 1, DS_TELE = 2 };
		DsMode_t dsMode = 
			(in.robot_mode.autonomous && in.robot_mode.enabled) ? DS_AUTO :
			(in.robot_mode.enabled) ? DS_TELE : DS_OTHER;
			table->PutBoolean ("isEnabled", in.robot_mode.enabled);
			table->PutNumber  ("dsMode",    dsMode);*/
	}
};

template<typename USER_CODE>
class Robot_adapter: public SampleRobot{
	To_crio<USER_CODE> u;

	void RobotInit(){}
	
	void Autonomous(void)
	{
		while(IsAutonomous() && IsEnabled()){
			//might need a loop here
			Robot_mode mode;
			mode.autonomous=1;
			mode.enabled=IsEnabled();
			u.run(mode);
			
			Wait(0.005);
		}
	}

	void OperatorControl(void)
	{
		//should see what happens when we get rid of this loop.  
		while (IsOperatorControl() && IsEnabled())
		{
			Robot_mode r;
			r.enabled=IsEnabled();
			u.run(r);
			
			//should see what happpens when this wait is removed.
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	//Runs during test mode
	void Test(){
		while(IsTest() && IsEnabled()){
			Robot_mode r;
			r.enabled=IsEnabled();
			u.run(r);
			
			Wait(0.005);
		}
	}
	
	void Disabled(){
		while(IsDisabled()){
			Robot_mode r;
			r.autonomous=IsAutonomous();
			u.run(r);

			Wait(0.005);
		}
	}
};

/*void initialize(FRCCommonControlData &a){
	a.packetIndex = 0;
	a.control = 0;

	//joystick axis values to neutral
	for(unsigned i=0;i<Joystick_data::AXES;i++){
		a.stick0Axes[i]=0;
		a.stick1Axes[i]=0;
		a.stick2Axes[i]=0;
		a.stick3Axes[i]=0;
	}
	
	//buttons initially off
	a.stick0Buttons = 0;
	a.stick1Buttons = 0;
	a.stick2Buttons = 0;
	a.stick3Buttons = 0;

	//analog and digital data from the drivers' station
	a.analog1 = 0;
	a.analog2 = 0;
	a.analog3 = 0;
	a.analog4 = 0;
	a.dsDigitalIn = 0;
}

struct Com_data{
	FRCCommonControlData data;
	
	Com_data(){
		initialize(data);
	}
	
	bool button(unsigned joystick,unsigned number)const{
		uint16_t button_data;
		switch(joystick){
		case 0:
			button_data=data.stick0Buttons;
			break;
		case 1:
			button_data=data.stick1Buttons;
			break;
		case 2:
			button_data=data.stick2Buttons;
			break;
		case 3:
			button_data=data.stick3Buttons;
			break;
		default:
			//it will be nice when we can use exception.
			cerr<<"Error: Bad joystick number.\n";
			return 0;
		}
		if(number>=Joystick_data::BUTTONS){
			//it will be nice when we can use exceptions
			cerr<<"Error: Button number out of range";
			return 0;
		}
		return !!(button_data&(1<<number));
	}
	
	uint8_t axis(unsigned joystick,unsigned number)const{
		if(number>=Joystick_data::AXES){
			//wil be nice when we can use exceptions
			cerr<<"Error: Axis number too high.\n";
			return 0;
		}
		switch(joystick){
		case 0: return data.stick0Axes[number];
		case 1: return data.stick1Axes[number];
		case 2: return data.stick2Axes[number];
		case 3: return data.stick3Axes[number];
		default:
			cerr<<"Joystick number too high.\n";
			return 0;
		}
	}
	
	uint16_t analog(unsigned i) const {
		switch(i){
		case 0: return data.analog1;
		case 1: return data.analog2;
		case 2: return data.analog3;
		case 3: return data.analog4;
		default:
			//exceptions would be good here.
			cerr<<"Drivers' station analog input number range is 0-3.\n";
			return 0;
		}
	}
	
	Robot_mode robot_mode()const{
		Robot_mode r;
		r.autonomous=data.autonomous;
		r.enabled=data.enabled;
		return r;
	}
};

Joystick_data read_joystick(Com_data const& cd,int port){
	Joystick_data r;
	for(unsigned i=0;i<r.AXES;i++){
		//TODO: Check whether the inputs can come actually come back as -128, and if not then just divide by 127.
		r.axis[i]=(double)cd.axis(port,i)/128;
	}
	for(unsigned i=0;i<r.BUTTONS;i++) r.button[i]=cd.button(port,i);
	return r;
}

void read_joysticks(Com_data const& cd,Robot_inputs &r){
	for(unsigned i=0;i<Robot_inputs::JOYSTICKS;i++){
		r.joystick[i]=read_joystick(cd,i);
	}
}

pair<Robot_inputs,int> read(Com_data const& cd){
	Robot_inputs r;
	r.robot_mode=cd.robot_mode();
	r.now=Timer::GetFPGATimestamp();
	read_joysticks(cd,r);
	int error_code=read_analog(r);
	return make_pair(r,error_code);
}

void report(Com_data const& cd){
	if(!cd.data.enabled){
		FRC_NetworkCommunication_observeUserProgramDisabled();
		return;
	}
	if(cd.data.autonomous){
		FRC_NetworkCommunication_observeUserProgramAutonomous();
		return;
	}
	FRC_NetworkCommunication_observeUserProgramTeleop();
}*/

/*int low_level(...){
	//This is not actually required.
	//FRC_NetworkCommunication_observeUserProgramStarting();
	cerr<<"in main task.\n";
	To_crio<Main> m;
	Com_data cd;
	
	//int update_number=0;
	while(1){
		setStatusData(
			battery_voltage(),//GetBatteryVoltage(),
			0,//UINT8//m_digitalOut,
			0,//update_number++,//m_updateNumber,
			NULL,//userStatusDataHigh, 
			0,//userStatusDataHighSize, 
			NULL,//userStatusDataLow, 
			0,//userStatusDataLowSize, 
			WAIT_FOREVER
		);
		getCommonControlData(&cd.data,40);//I think this means there's a 40 ms timeout.
		
		//This is not actually required.
		//report(cd);
		
		pair<Robot_inputs,int> p=read(cd);
		if(p.second){
			cerr<<"Error code "<<p.second<<"\n";
		}
		//TODO: Make this work before we re-enable this mode.
	//	for(unsigned i=0;i<Robot_outputs::DIGITAL_IOS;i++){
		//	p.first.digital_in[i]=digital_io[i].get();
		//}
		m.run(p.first);
	}
	return 0;
}*/

/*extern "C"{
	INT32 FRC_UserProgram_StartupLibraryInit(){
		cerr<<"started the low level thing!\n";
		int stack_size=64000;//copied from Task
		int priority=Task::kDefaultPriority;
		INT32 task_id=taskSpawn("tDemo",priority,VX_FP_TASK,stack_size,low_level,0,1,2,3,4,5,6,7,8,9);
		cerr<<"Task id="<<task_id<<" error="<<ERROR<<"\r\n";
		cerr<<"end starter\n";
		return 0;
	}
}*/

START_ROBOT_CLASS(Robot_adapter<Main>);
