#ifndef POSEDGE_TRIGGER_DEBOUNCE_H
#define POSEDGE_TRIGGER_DEBOUNCE_H

#include "debounce.h"
#include "posedge_trigger.h"

class Posedge_trigger_debounce{
	#define POSEDGE_TRIGGER_DEBOUNCE_ITEMS(X)\
		X(Debounce<bool>,debounce)\
		X(Posedge_trigger,trigger)

	#define X(A,B) A B;
	POSEDGE_TRIGGER_DEBOUNCE_ITEMS(X)
	#undef X

	public:
	Posedge_trigger_debounce();
	bool operator()(Time,bool);

	friend std::ostream& operator<<(std::ostream&,Posedge_trigger_debounce const&);
	friend bool operator<(Posedge_trigger_debounce const&,Posedge_trigger_debounce const&);
	friend bool operator==(Posedge_trigger_debounce const&,Posedge_trigger_debounce const&);
};

bool operator<(Posedge_trigger_debounce const&,Posedge_trigger_debounce const&);
bool operator!=(Posedge_trigger_debounce const&,Posedge_trigger_debounce const&);
std::ostream& operator<<(std::ostream&,Posedge_trigger_debounce const&);

#endif
