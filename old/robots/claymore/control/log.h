#ifndef LOG_H
#define LOG_H

#include<fstream>
#include "../util/interface.h"
#include "toplevel.h"

class Log{
	std::ofstream f;

	public:
	Log();
	explicit Log(std::string const&);
	
	void operator()(Robot_inputs const&,Toplevel::Status_detail const&,Robot_outputs const&);
};

#endif
