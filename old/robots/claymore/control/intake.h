#ifndef INTAKE_H
#define INTAKE_H

#include "../util/interface.h"
#include "nop.h"
#include "message_logger.h"
#include <iostream>
#include <set>
#include <sstream>

struct Intake{
	struct Goal {
		private:
		Goal();
		double left_, right_;

		public:
		double left()const;
		double right()const;

		static Goal in();
		static Goal in(double);
		static Goal off();
		static Goal out();
		static Goal out(double);
		static Goal absolute(double, double);
	};

	using Input=Nop::Input;
	
	typedef Input Status_detail;

	typedef Status_detail Status;

	using Input_reader=Nop::Input_reader;

	struct Output{
		double left, right;

		Output();
		Output(double, double);
	};
	
	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Intake::Output)const;
		Intake::Output operator()(Robot_outputs)const;	
	};

	using Estimator=Nop::Estimator;

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator; 
};

std::ostream& operator<<(std::ostream&,Intake::Goal);
std::ostream& operator<<(std::ostream&,Intake::Output);
std::ostream& operator<<(std::ostream&,Intake);

bool operator<(Intake::Goal,Intake::Goal);
bool operator==(Intake::Goal,Intake::Goal);
bool operator!=(Intake::Goal,Intake::Goal);

bool operator<(Intake::Output,Intake::Output);
bool operator==(Intake::Output,Intake::Output);
bool operator!=(Intake::Output,Intake::Output);

bool operator==(Intake::Output_applicator,Intake::Output_applicator);

bool operator==(Intake,Intake);
bool operator!=(Intake,Intake);

std::set<Intake::Goal> examples(Intake::Goal*);
std::set<Intake::Status_detail> examples(Intake::Status_detail*);

Intake::Output control(Intake::Status_detail, Intake::Goal);
Intake::Status status(Intake::Status_detail);
bool ready(Intake::Status, Intake::Goal);

inline messageLogger &operator<<(messageLogger &logger, Intake::Goal goal)
{
	std::stringstream strm ;

	strm << goal ;
	logger << strm.str() ;

	return logger ;
}
#endif
