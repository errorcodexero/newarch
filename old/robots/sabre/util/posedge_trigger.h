#ifndef POSEDGE_TRIGGER_H
#define POSEDGE_TRIGGER_H

#include<iosfwd>

struct Posedge_trigger{
	bool last;
	//Last stores the last value that was given

	Posedge_trigger();
	explicit Posedge_trigger(bool assume_previous);

	//Lets you call things of posedgetrigger as if it were a function
	bool operator()(bool);
	//If previous value was 0, and new value is one, return one
};
bool operator==(Posedge_trigger,Posedge_trigger);
bool operator!=(Posedge_trigger,Posedge_trigger);
std::ostream& operator<<(std::ostream& o,Posedge_trigger);

#endif
