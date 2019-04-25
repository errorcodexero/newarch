#ifndef MAIN_H
#define MAIN_H

#include "force_interface.h"
#include "../util/perf_tracker.h"
#include "../util/countup_timer.h"
#include "log.h"
#include "../executive/executive.h"

struct Main{
	Executive mode;

	Force_interface force;
	Perf_tracker perf;
	Toplevel toplevel;
	
	Countup_timer since_switch;

	Posedge_trigger autonomous_start;
	Log log;	
	
	Main();
	Main(Executive);
	Robot_outputs operator()(Robot_inputs,std::ostream& = std::cerr);
};

bool operator==(Main const&,Main const&);
bool operator!=(Main const&,Main const&);
std::ostream& operator<<(std::ostream&,Main const&);

#endif
