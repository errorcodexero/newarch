#ifndef PUMP_H
#define PUMP_H

//This is meant to control the pump for the pneumatic system.
//It may seem a little bit overkill, but this is here basically because there are some fancier things we might want to do later like know when we have at least 60psi.  

#include<iosfwd>
#include<set>
#include "../util/maybe.h"
#include "../util/interface.h"

struct Pump{
	enum class Goal{AUTO,OFF};
	typedef Goal Output;

	enum class Status{FULL,NOT_FULL};
	typedef Status Status_detail;
	typedef Status Input;

	struct Input_reader{
		Robot_inputs operator()(Robot_inputs,Input)const;
		Input operator()(Robot_inputs)const;
	};
	Input_reader input_reader;

	struct Estimator{
		Pump::Status status;

		Estimator();
		void update(Time,Pump::Input,Pump::Output);
		Status_detail get()const;
	};
	Estimator estimator;

	struct Output_applicator{
		Robot_outputs operator()(Robot_outputs,Pump::Output)const;
		Pump::Output operator()(Robot_outputs)const;
	};
	Output_applicator output_applicator;
};
std::ostream& operator<<(std::ostream&,Pump const&);
std::ostream& operator<<(std::ostream&,Pump::Goal);
std::ostream& operator<<(std::ostream&,Pump::Status);

bool operator==(Pump::Estimator const&,Pump::Estimator const&);
bool operator!=(Pump::Estimator const&,Pump::Estimator const&);

bool operator!=(Pump const&,Pump const&);

Maybe<Pump::Status> parse_status(std::string const&);
Pump::Output control(Pump::Status,Pump::Goal);
Pump::Status status(Pump::Status_detail);
bool ready(Pump::Status,Pump::Goal);
std::set<Pump::Goal> examples(Pump::Goal*);
std::set<Pump::Status> examples(Pump::Status*);

#endif
