#ifndef SHOOTER_H
#define SHOOTER_H

#include <iostream>
#include <set>
#include "../util/interface.h"
#include "../util/util.h"
#include "../util/countdown_timer.h"

const double GROUND_RPM=-6000.0;//This is the one we're actually using//7300
const double CLIMB_RPM=-4000.0;
const double FREE_SPIN_RPM=-1000.0;

struct Shooter{
	struct Goal{
		PID_values constants;

		#define SHOOTER_MODES X(VOLTAGE) X(SPEED)
		enum class Mode{
			#define X(name) name,
			SHOOTER_MODES
			#undef X
		};
		Mode mode;
		double value;//0-1 for voltage mode, RPM for speed mode
		Goal();
		Goal(PID_values,Shooter::Goal::Mode,double);
	};
	
	struct Status_detail{
		double speed;//rpm
		
		Status_detail();
		Status_detail(double);
	};
	
	typedef Status_detail Status;

	struct Input{
		int speed;//rpm
		bool enabled;
	};
	
	struct Input_reader{
		Shooter::Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Shooter::Input)const;
	};

	struct Output{
		PID_values constants;
		double speed;//rpm
		double voltage;
		Talon_srx_output::Mode mode;
		Output();
		Output(double,double,Talon_srx_output::Mode);
	};
	
	struct Output_applicator{
		Shooter::Output operator()(Robot_outputs const&)const;
		Robot_outputs operator()(Robot_outputs,Shooter::Output)const;
	};

	struct Estimator{
		Shooter::Status_detail last;
		Shooter::Output last_output;
	
		Shooter::Status_detail get()const;
		void update(Time,Shooter::Input,Shooter::Output);
		Estimator();
	};

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator;
};

std::ostream& operator<<(std::ostream&,Shooter::Goal::Mode);
std::ostream& operator<<(std::ostream&,Shooter::Goal);
std::ostream& operator<<(std::ostream&,Shooter::Input);
std::ostream& operator<<(std::ostream&,Shooter::Status_detail);
std::ostream& operator<<(std::ostream&,Shooter);
std::ostream& operator<<(std::ostream&,Shooter::Output);

bool operator==(Shooter::Input,Shooter::Input);
bool operator!=(Shooter::Input,Shooter::Input);
bool operator<(Shooter::Input,Shooter::Input);

bool operator<(Shooter::Status_detail,Shooter::Status_detail);
bool operator==(Shooter::Status_detail,Shooter::Status_detail);
bool operator!=(Shooter::Status_detail,Shooter::Status_detail);

bool operator==(Shooter::Goal,Shooter::Goal);
bool operator!=(Shooter::Goal,Shooter::Goal);
bool operator<(Shooter::Goal,Shooter::Goal);

bool operator==(Shooter::Output,Shooter::Output);
bool operator!=(Shooter::Output,Shooter::Output);
bool operator<(Shooter::Output,Shooter::Output);

bool operator==(Shooter::Input_reader,Shooter::Input_reader);
bool operator<(Shooter::Input_reader,Shooter::Input_reader);

bool operator==(Shooter::Estimator,Shooter::Estimator);
bool operator!=(Shooter::Estimator,Shooter::Estimator);

bool operator==(Shooter::Output_applicator,Shooter::Output_applicator);

bool operator==(Shooter,Shooter);
bool operator!=(Shooter,Shooter);

std::set<Shooter::Input> examples(Shooter::Input*);
std::set<Shooter::Goal> examples(Shooter::Goal*);
std::set<Shooter::Status_detail> examples(Shooter::Status_detail*);
std::set<Shooter::Status> examples(Shooter::Status*);

Shooter::Output control(Shooter::Status_detail,Shooter::Goal);
Shooter::Status status(Shooter::Status_detail);
bool ready(Shooter::Status,Shooter::Goal);

#endif

