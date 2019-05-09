#ifndef SETTABLE_TOGGLE_H
#define SETTABLE_TOGGLE_H

#include "posedge_trigger.h"

class Settable_toggle{
	Posedge_trigger trig;
	bool value;

	public:
	Settable_toggle();

	void set(bool value);
	void update(bool sample);
	bool get()const;

	friend std::ostream& operator<<(std::ostream&,Settable_toggle);
	friend bool operator==(Settable_toggle,Settable_toggle);
};
bool operator==(Settable_toggle,Settable_toggle);
bool operator!=(Settable_toggle,Settable_toggle);
std::ostream& operator<<(std::ostream&,Settable_toggle);

#endif
