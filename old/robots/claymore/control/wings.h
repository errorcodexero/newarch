#ifndef WINGS_H
#define WINGS_H

#include <iostream>
#include <set>
#include "../util/interface.h"
#include "nop.h"

struct Wings{
	enum class Goal{LOCKED, UNLOCKED};
	
	using Input=Nop::Input;
	
	typedef Input Status_detail;

	typedef Status_detail Status;

	using Input_reader=Nop::Input_reader;

	typedef Goal Output;
	
	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Wings::Output)const;
		Wings::Output operator()(Robot_outputs)const;	
	};

	using Estimator=Nop::Estimator;

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator; 
};

std::ostream& operator<<(std::ostream&,Wings::Goal);
std::ostream& operator<<(std::ostream&,Wings);

bool operator<(Wings::Status_detail,Wings::Status_detail);
bool operator==(Wings::Status_detail,Wings::Status_detail);
bool operator!=(Wings::Status_detail,Wings::Status_detail);

bool operator==(Wings::Output_applicator,Wings::Output_applicator);

bool operator==(Wings,Wings);
bool operator!=(Wings,Wings);

std::set<Wings::Goal> examples(Wings::Goal*);
std::set<Wings::Status_detail> examples(Wings::Status_detail*);

Wings::Output control(Wings::Status_detail, Wings::Goal);
Wings::Status status(Wings::Status_detail);
bool ready(Wings::Status, Wings::Goal);

#endif
