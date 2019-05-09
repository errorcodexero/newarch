#ifndef CLIMB_RELEASE_H
#define CLIMB_RELEASE_H

#include <iostream>
#include <set>
#include "../util/countdown_timer.h"
#include "../util/interface.h"
#include "../util/util.h"
#include "../util/driver_station_interface.h"

struct Climb_release{
	enum class Goal{IN,STOP,OUT};
	
	enum class Status_detail{IN,OUT,PROBABLY_OUT,UNKNOWN};
	
	typedef Status_detail Status;

	struct Input{
		bool enabled;
	};
	
	struct Input_reader{
		Climb_release::Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Climb_release::Input)const;
	};

	typedef Goal Output;
	
	struct Output_applicator{
		Climb_release::Output operator()(Robot_outputs)const;
		Robot_outputs operator()(Robot_outputs,Climb_release::Output)const;
	};

	struct Estimator{
		Status_detail last;
		Countdown_timer timer;
		Countdown_timer refresh_timer;

		Climb_release::Output last_output;
		Climb_release::Status_detail get()const;
		void update(Time,Climb_release::Input,Climb_release::Output);
		Estimator();
	};

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator;
};

std::ostream& operator<<(std::ostream&,Climb_release::Goal);
std::ostream& operator<<(std::ostream&,Climb_release::Input);
std::ostream& operator<<(std::ostream&,Climb_release::Status_detail);
std::ostream& operator<<(std::ostream&,Climb_release::Estimator);
std::ostream& operator<<(std::ostream&,Climb_release);

bool operator==(Climb_release::Input,Climb_release::Input);
bool operator!=(Climb_release::Input,Climb_release::Input);
bool operator<(Climb_release::Input,Climb_release::Input);

bool operator==(Climb_release::Input_reader,Climb_release::Input_reader);
bool operator<(Climb_release::Input_reader,Climb_release::Input_reader);

bool operator==(Climb_release::Estimator,Climb_release::Estimator);
bool operator!=(Climb_release::Estimator,Climb_release::Estimator);

bool operator==(Climb_release::Output_applicator,Climb_release::Output_applicator);

bool operator==(Climb_release,Climb_release);
bool operator!=(Climb_release,Climb_release);

std::set<Climb_release::Input> examples(Climb_release::Input*);
std::set<Climb_release::Goal> examples(Climb_release::Goal*);
std::set<Climb_release::Status_detail> examples(Climb_release::Status_detail*);

Climb_release::Output control(Climb_release::Status_detail,Climb_release::Goal);
Climb_release::Status status(Climb_release::Status_detail);
bool ready(Climb_release::Status,Climb_release::Goal);

#endif
