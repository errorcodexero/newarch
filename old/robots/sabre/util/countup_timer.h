#ifndef COUNTUP_TIMER_H
#define COUNTUP_TIMER_H

#include "interface.h"

class Countup_timer{
	Time start,latest;

	public:
	Countup_timer();
	void update(Time,bool reset);
	Time elapsed()const;
	void out(std::ostream&)const;

	friend bool operator==(Countup_timer,Countup_timer);
};
bool operator==(Countup_timer,Countup_timer);
std::ostream& operator<<(std::ostream&,Countup_timer);

#endif
