#ifndef TOTE_SENSORS
#define TOTE_SENSORS

#include "../util/interface.h"

struct Tote_sensors{
	struct Input{
		bool left,right,center;
	};

	struct Input_reader{
		Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Input)const;
	};
	Input_reader input_reader;

	struct Output{};
	typedef Output Status;
	typedef Status Status_detail;
	typedef Output Goal; 

	struct Estimator{
		void update(Time,Input,Output);
		Status_detail get()const;
	};
	Estimator estimator;

	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Output)const;
		Output operator()(Robot_outputs)const;
	};
	Output_applicator output_applicator;
};

bool operator<(Tote_sensors::Input const&,Tote_sensors::Input const&);
bool operator==(Tote_sensors::Input const&,Tote_sensors::Input const&);
bool operator!=(Tote_sensors::Input const&,Tote_sensors::Input const&);
std::ostream& operator<<(std::ostream&,Tote_sensors::Input const&);

bool operator<(Tote_sensors::Output,Tote_sensors::Output);
bool operator==(Tote_sensors::Output,Tote_sensors::Output);
bool operator!=(Tote_sensors::Output,Tote_sensors::Output);
std::ostream& operator<<(std::ostream&,Tote_sensors::Output);

bool operator!=(Tote_sensors::Estimator const&,Tote_sensors::Estimator const&);

bool operator!=(Tote_sensors const&,Tote_sensors const&);
std::ostream& operator<<(std::ostream&,Tote_sensors const&);

Tote_sensors::Output control(Tote_sensors::Status,Tote_sensors::Goal);
bool ready(Tote_sensors::Status,Tote_sensors::Goal);
Tote_sensors::Status status(Tote_sensors::Status_detail);

#endif
