#ifndef TOPLEVEL_H
#define TOPLEVEL_H

#include <vector>
#include "pump.h"
#include "drivebase.h"
#include "grabber.h"
#include "intake.h"
#include "lifter.h"
#include "lights.h"
#include "nop.h"
#include "wings.h"

#define TOPLEVEL_ITEMS \
	X(Pump,pump,Pump::Goal::AUTO)\
	X(Drivebase,drive,)\
	X(Grabber,grabber,Grabber::Goal::hold())\
	X(Intake,intake,Intake::Goal::off())\
	X(Lifter,lifter,Lifter::Goal::stop())\
	X(Lights,lights,)\
	X(Wings,wings,Wings::Goal::LOCKED)

class Toplevel{
	public:

	struct Input{
		#define X(A,B,C) A::Input B;
		TOPLEVEL_ITEMS
		#undef X
	};

	class Input_reader{
		#define X(A,B,C) A::Input_reader B;
		TOPLEVEL_ITEMS
		#undef X

		public:
		explicit Input_reader();
		Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Input)const;

		friend bool operator==(Input_reader const&,Input_reader const&);
	};
	Input_reader input_reader;

	struct Output{
		Output();
		#define X(A,B,C) A::Output B;
		TOPLEVEL_ITEMS
		#undef X
	};

	struct Goal{
		Goal();
		#define X(A,B,C) A::Goal B;
		TOPLEVEL_ITEMS
		#undef X
		bool dummy[0];
	};

	struct Status_detail{
		#define X(A,B,C) A::Status_detail B;
		TOPLEVEL_ITEMS
		#undef X
	};
	
	struct Status{
		Status();//TODO:remove this
		#define X(A,B,C) A::Status B;
		TOPLEVEL_ITEMS
		#undef X
	};

	class Estimator{
		#define X(A,B,C) A::Estimator B;
		TOPLEVEL_ITEMS
		#undef X

		public:
		void update(Time,Input,Output);
		Status_detail get()const;
		void out(std::ostream&)const;

		friend bool operator==(Estimator,Estimator);
	};
	Estimator estimator;

	class Output_applicator{
		#define X(A,B,C) A::Output_applicator B;
		TOPLEVEL_ITEMS
		#undef X

		public:
		Robot_outputs operator()(Robot_outputs,Output const&)const;
		Output operator()(Robot_outputs const&)const;
	};
	Output_applicator output_applicator;
};
bool operator<(Toplevel::Output const&,Toplevel::Output const&);
bool operator==(Toplevel::Output const&,Toplevel::Output const&);
bool operator!=(Toplevel::Output const&,Toplevel::Output const&);
std::ostream& operator<<(std::ostream&,Toplevel::Output);

bool operator<(Toplevel::Goal const&,Toplevel::Goal const&);
std::ostream& operator<<(std::ostream&,Toplevel::Goal);
std::ostream& operator<<(std::ostream&,Toplevel::Status_detail const&);

bool operator<(Toplevel::Status const&,Toplevel::Status const&);
bool operator==(Toplevel::Status,Toplevel::Status);
bool operator!=(Toplevel::Status,Toplevel::Status);
std::ostream& operator<<(std::ostream& o,Toplevel::Status);
//Maybe<Toplevel::Status> parse_status(std::string const&);
//
bool operator<(Toplevel::Status_detail const&,Toplevel::Status_detail const&);
bool operator==(Toplevel::Status_detail const&,Toplevel::Status_detail const&);
bool operator!=(Toplevel::Status_detail const&,Toplevel::Status_detail const&);
std::set<Toplevel::Status_detail> examples(Toplevel::Status_detail*);
Toplevel::Status_detail rand(Toplevel::Status_detail*);

std::set<Toplevel::Goal> examples(Toplevel::Goal*);
std::set<Toplevel::Output> examples(Toplevel::Output*);
std::set<Toplevel::Status> examples(Toplevel::Status*);
std::set<Toplevel::Input> examples(Toplevel::Input*);

bool operator<(Toplevel::Input const&,Toplevel::Input const&);
bool operator==(Toplevel::Input const&,Toplevel::Input const&);
std::ostream& operator<<(std::ostream&,Toplevel::Input const&);

bool operator!=(Toplevel::Input_reader const&,Toplevel::Input_reader const&);
std::ostream& operator<<(std::ostream&,Toplevel::Input_reader const&);

bool operator!=(Toplevel::Output_applicator const&,Toplevel::Output_applicator const&);
std::ostream& operator<<(std::ostream&,Toplevel::Output_applicator const&);

bool operator==(Toplevel::Estimator,Toplevel::Estimator);
bool operator!=(Toplevel::Estimator,Toplevel::Estimator);
std::ostream& operator<<(std::ostream& o,Toplevel::Estimator);
bool approx_equal(Toplevel::Estimator,Toplevel::Estimator);

Toplevel::Output control(Toplevel::Status_detail,Toplevel::Goal);
bool ready(Toplevel::Status,Toplevel::Goal);
std::vector<std::string> not_ready(Toplevel::Status,Toplevel::Goal);
Toplevel::Status status(Toplevel::Status_detail const&);

bool operator==(Toplevel const&,Toplevel const&);
bool operator!=(Toplevel const&,Toplevel const&);
std::ostream& operator<<(std::ostream&,Toplevel const&);

#endif
