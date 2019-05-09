#ifndef BOUND_TRACKER_H
#define BOUND_TRACKER_H

#include<iosfwd>

//this would be easy to make a template on the value type.
struct Bound_tracker{
	double min,max;
	bool data;
	
	Bound_tracker();
	
	void update(double);
};

std::ostream& operator<<(std::ostream&,Bound_tracker);

#endif
