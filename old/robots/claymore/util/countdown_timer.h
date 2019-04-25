#pragma once

#include "unittype.h"
#include <iostream>

class Countdown_timer{
    Time left,last_time;
    bool last_enable;
	
public:
    Countdown_timer();
    void set(Time length);
    void update(Time now,bool enable);
    bool done()const;
	
    friend std::ostream& operator<<(std::ostream&,Countdown_timer);

    template<typename Func>
	friend void traverse(Countdown_timer,Func&);

    friend bool operator<(Countdown_timer const&,Countdown_timer const&);
    friend bool operator==(Countdown_timer const&,Countdown_timer const&);
};

bool operator<(Countdown_timer const&,Countdown_timer const&);
bool operator==(Countdown_timer const&,Countdown_timer const&);
bool operator!=(Countdown_timer const&,Countdown_timer const&);
std::ostream& operator<<(std::ostream&,Countdown_timer);
