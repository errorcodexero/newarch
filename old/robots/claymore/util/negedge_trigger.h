#ifndef NEGEDGE_TRIGGER_H
#define NEGEDGE_TRIGGER_H

#include<iosfwd>

struct Negedge_trigger{
	bool last;

	Negedge_trigger();
	bool operator()(bool);
};

std::ostream& operator<<(std::ostream& o,Negedge_trigger);

#endif
