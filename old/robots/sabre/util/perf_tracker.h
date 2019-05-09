#ifndef PERF_TRACKER_H
#define PERF_TRACKER_H

#include "interface.h"

class Perf_tracker{
	Time first,last;
	Time worst;
	int i;
	
	public:
	Perf_tracker();

	void update(Time now);
	void reset_worst();

	Time worst_case()const;
	Time average()const;
	
	friend std::ostream& operator<<(std::ostream&,Perf_tracker);
	friend bool operator==(Perf_tracker,Perf_tracker);
};

bool operator==(Perf_tracker,Perf_tracker);
bool operator!=(Perf_tracker,Perf_tracker);
std::ostream& operator<<(std::ostream&,Perf_tracker);
bool approx_equal(Perf_tracker,Perf_tracker);

#endif
