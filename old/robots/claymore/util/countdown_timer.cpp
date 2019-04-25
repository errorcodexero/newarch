#include "countdown_timer.h"
#include<iostream>

/*
This implements a timer which counts down based on calls to update().  

The arguments to update() are the current time (which is assumed to be monotonically increasing), and whether we should be counting down right now.
*/
using namespace std;

Countdown_timer::Countdown_timer():left(0),last_time(0),last_enable(0){}

void Countdown_timer::set(Time length){
	left=length;
	last_time=0;
	last_enable=0;
}

void Countdown_timer::update(Time now,bool enable){
	if(enable && last_enable && left>0){
		left-=(now-last_time);
	}
	last_enable=enable;
	last_time=now;
}

bool Countdown_timer::done()const{
	return left<=0;//TODO: change to a number slightly higher than zero? 
}

bool operator<(Countdown_timer const& a,Countdown_timer const& b){
	#define X(NAME) if(a.NAME<b.NAME) return 1; if(b.NAME<a.NAME) return 0;
	X(left) X(last_time) X(last_enable)
	#undef X
	return 0;
}

bool operator==(Countdown_timer const& a,Countdown_timer const& b){
	return a.left==b.left && a.last_time==b.last_time && a.last_enable==b.last_enable;
}

bool operator!=(Countdown_timer const& a,Countdown_timer const& b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Countdown_timer a){
	return o<<"Countdown_timer("<<a.left<<" "<<a.last_time<<" "<<a.last_enable<<")";
}

#ifdef COUNTDOWN_TIMER_TEST

#include<cassert>

int main(){
	Countdown_timer t;
	t.update(0,1);
	t.set(1);
	t.update(0,1);
	assert(!t.done());
	t.update(.5,1);
	cout<<t<<"\n";
	t.update(.6,0);
	t.update(50,0);
	assert(!t.done());
	t.update(50.2,1);
	t.update(50.9,1);
	cout<<t<<"\n";
	cout.flush();
	assert(t.done());
}

#endif
