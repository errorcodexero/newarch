#ifndef	WINCH_H
#define WINCH_H

#include <set>
#include "../util/interface.h"
#include "../util/countdown_timer.h"

struct Winch{
	enum class Goal{IN,OUT,STOP};
	
	typedef Goal Output;

	struct Input{
		bool deployed;
		Input();
	};

	struct Status_detail{
		bool deployed, partly_climbed;
		Status_detail();
		Status_detail(bool,bool);
	};
	
	typedef Status_detail Status;
	
	struct Input_reader{
		Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Input)const;
	};

	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Output)const;
		Output operator()(Robot_outputs const&)const;
	};

	struct Estimator{
		Countdown_timer climb_timer;
		Status_detail last;
		
		void update(Time,Input,Output);
		Status_detail get()const;
		Estimator();
	};

	Input_reader input_reader;
	Output_applicator output_applicator;
	Estimator estimator;
};

std::set<Winch::Goal> examples(Winch::Goal*);
std::set<Winch::Input> examples(Winch::Input*);
std::set<Winch::Status_detail> examples(Winch::Status_detail*);

std::ostream& operator<<(std::ostream&,Winch::Goal);
std::ostream& operator<<(std::ostream&,Winch::Input);
std::ostream& operator<<(std::ostream&,Winch::Status_detail);
std::ostream& operator<<(std::ostream&,Winch const&);

bool operator<(Winch::Input,Winch::Input);
bool operator==(Winch::Input,Winch::Input);
bool operator!=(Winch::Input,Winch::Input);

bool operator<(Winch::Status_detail,Winch::Status_detail);
bool operator==(Winch::Status_detail,Winch::Status_detail);
bool operator!=(Winch::Status_detail,Winch::Status_detail);

bool operator==(Winch::Estimator,Winch::Estimator);
bool operator!=(Winch::Estimator,Winch::Estimator);

bool operator==(Winch,Winch);
bool operator!=(Winch,Winch);

Winch::Output control(Winch::Status,Winch::Goal);
Winch::Status status(Winch::Status);
bool ready(Winch::Status,Winch::Goal);

#endif
