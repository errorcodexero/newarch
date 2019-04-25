#ifndef STALL_MONITOR_H
#define STALL_MONITOR_H

#include "interface.h"
#include "util.h"
#include "persistent_tracker.h"


class Stall_monitor{
	static const int KEEP = 5;
	Persistent_tracker<double> currents;
	Maybe<double> max_current;
	Persistent_tracker<double> speeds;
	Maybe<double> mean_speed;
	Maybe<bool> stall;

	public:
	Stall_monitor(); 
	void update(double,double);
	Maybe<bool> get()const;

	friend std::ostream& operator<<(std::ostream&,Stall_monitor const&);
};

#endif
