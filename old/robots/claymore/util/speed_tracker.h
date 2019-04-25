#ifndef SPEED_TRACKER_H
#define SPEED_TRACKER_H

#include "interface.h"
#include "countdown_timer.h"

struct Speed_tracker{
	const Time POLL_TIME=0.05; //seconds

	private:
	double last_distance;
	Time last_time;
	double speed;//inches/second
	Countdown_timer poll_timer;

	public:
	void update(Time,double);
	double get()const;
	
	Speed_tracker();
	
	friend bool operator==(Speed_tracker const&,Speed_tracker const&);
	friend bool operator!=(Speed_tracker const&,Speed_tracker const&);
	friend bool operator<(Speed_tracker const&,Speed_tracker const&);
	friend std::ostream& operator<<(std::ostream&,Speed_tracker const&);
};

bool operator==(Speed_tracker const&,Speed_tracker const&);
bool operator!=(Speed_tracker const&,Speed_tracker const&);
bool operator<(Speed_tracker const&,Speed_tracker const&);
std::ostream& operator<<(std::ostream&,Speed_tracker const&);

#endif
