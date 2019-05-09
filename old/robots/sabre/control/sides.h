#ifndef SIDES_H
#define SIDES_H

#include <iostream>
#include <set>
#include "../util/interface.h"
#include "front.h"

using namespace std;

struct Sides{
	enum class Goal{IN,OFF,OUT};
	
	struct Input{};
	
	typedef Input Status_detail;

	typedef Status_detail Status;

	struct Input_reader{
		Sides::Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Sides::Input)const;
	};

	typedef Goal Output;
	
	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Sides::Output)const;
		Sides::Output operator()(Robot_outputs)const;	
	};

	struct Estimator{
		Status_detail get()const;
		void update(Time,Input,Output){};	
	};

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator; 
};

ostream& operator<<(ostream&,Sides::Goal);
ostream& operator<<(ostream&,Sides);
ostream& operator<<(ostream&,Sides::Input);

bool operator==(Sides::Input,Sides::Input);
bool operator!=(Sides::Input,Sides::Input);
bool operator<(Sides::Input, Sides::Input);

bool operator<(Sides::Status_detail,Sides::Status_detail);
bool operator==(Sides::Status_detail,Sides::Status_detail);
bool operator!=(Sides::Status_detail,Sides::Status_detail);

bool operator==(Sides::Input_reader,Sides::Input_reader);
bool operator<(Sides::Input_reader, Sides::Input_reader);

bool operator==(Sides::Estimator, Sides::Estimator);
bool operator!=(Sides::Estimator, Sides::Estimator);

bool operator==(Sides::Output_applicator,Sides::Output_applicator);

bool operator==(Sides,Sides);
bool operator!=(Sides,Sides);

set<Sides::Input> examples(Sides::Input*);
set<Sides::Goal> examples(Sides::Goal*);
set<Sides::Status_detail> examples(Sides::Status_detail*);

Sides::Output control(Sides::Status_detail, Sides::Goal);
Sides::Status status(Sides::Status_detail);
bool ready(Sides::Status, Sides::Goal);

#endif
