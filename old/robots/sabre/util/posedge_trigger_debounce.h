#ifndef POSEDGE_TRIGGER_DEBOUNCE_H
#define POSEDGE TRIGGER_DEBOUNCE_H

#include "debounce.h"
#include "posedge_trigger.h"

class Posedge_trigger_debounce{
	#define POSEDGE_TRIGGER_DEBOUNCE_ITEMS(X)\
		X(Debounce,debounce)\
		X(Posedge_trigger,trigger)

	#define X(A,B) A B;
	POSEDGE_TRIGGER_DEBOUNCE_ITEMS(X)
	#undef X

	public:
	bool operator()(Time,bool);

	friend std::ostream& operator<<(std::ostream&,Posedge_trigger_debounce const&);
};

std::ostream& operator<<(std::ostream&,Posedge_trigger_debounce const&);

#endif
