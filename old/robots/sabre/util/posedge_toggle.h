#ifndef POSEDGE_TOGGLE_H
#define POSEDGE_TOGGLE_H

#include<iosfwd>

class Posedge_toggle{
	bool value,last;
	
	public:
	Posedge_toggle();
	void update(bool sample);
	bool get()const;
	
	friend std::ostream& operator<<(std::ostream&,Posedge_toggle);
	friend bool operator==(Posedge_toggle,Posedge_toggle);
};
bool operator==(Posedge_toggle,Posedge_toggle);
bool operator!=(Posedge_toggle,Posedge_toggle);
std::ostream& operator<<(std::ostream&,Posedge_toggle);

#endif
