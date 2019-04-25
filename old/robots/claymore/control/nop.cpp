#include "nop.h"

using namespace std;

/*
This is a subsystem that does nothing.  It is meant to be used when some parts of a new subsystem don't do anything.  For example, if a new subsystem were to be created that had no sensor inputs it could directly re-use the Input and Input_reader types.
*/

ostream& operator<<(ostream& o,Nop const&){
	return o<<"Nop";
}

bool operator<(Nop::Input,Nop::Input){ return 0; }
bool operator==(Nop::Input,Nop::Input){ return 1; }
bool operator!=(Nop::Input,Nop::Input){ return 0; }

ostream& operator<<(ostream& o,Nop::Input const&){
	return o<<"Nop::Input";
}

set<Nop::Input> examples(Nop::Input*){
	return {Nop::Input{}};
}

bool operator==(Nop::Input_reader,Nop::Input_reader){ return 1; }

Nop::Input Nop::Input_reader::operator()(Robot_inputs const&)const{
	return {};
}

Robot_inputs Nop::Input_reader::operator()(Robot_inputs a,Nop::Input)const{
	return a;
}

bool operator==(Nop::Output_applicator,Nop::Output_applicator){ return 1; }

Nop::Output Nop::Output_applicator::operator()(Robot_outputs)const{
	return {};
}

Robot_outputs Nop::Output_applicator::operator()(Robot_outputs a,Nop::Output)const{
	return a;
}

bool operator==(Nop::Estimator,Nop::Estimator){ return 1; }
bool operator!=(Nop::Estimator,Nop::Estimator){ return 0; }
Nop::Status_detail Nop::Estimator::get()const{ return {}; }

Nop::Status status(Nop::Status_detail a){ return a; }
Nop::Output control(Nop::Status_detail,Nop::Goal){ return {}; }
bool ready(Nop::Status,Nop::Goal){ return 1; }

#ifdef NOP_TEST
#include "formal.h"

int main(){
	Tester_mode a;
	a.check_multiple_outputs=0;
	tester(Nop{},a);
}
#endif
