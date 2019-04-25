#include "speed_tracker.h"
#include "../control/drivebase.h"

using namespace std;

void Speed_tracker::update(Time t,double distance){
	poll_timer.update(t,1);
	if(poll_timer.done()){
		speed=(distance-last_distance)/(t-last_time);
		last_distance=distance;
		last_time=t;
		poll_timer.set(POLL_TIME);
		poll_timer.update(t,1);  
	}
}

double Speed_tracker::get()const{
	return speed;
}

Speed_tracker::Speed_tracker():last_distance(0),last_time(0),speed(0),poll_timer({}){
	poll_timer.set(POLL_TIME);
}

bool operator==(Speed_tracker const& a,Speed_tracker const& b){
	return a.last_distance==b.last_distance && a.last_time==b.last_time && a.speed==b.speed && a.poll_timer==b.poll_timer;
}

bool operator!=(Speed_tracker const& a,Speed_tracker const& b){ return !(a==b); }

bool operator<(Speed_tracker const& a,Speed_tracker const& b){
	if(a.last_time<b.last_time) return 1;
	if(b.last_time<a.last_time) return 0;
	if(a.last_distance<b.last_distance) return 1;
	if(b.last_distance<a.last_distance) return 0;
	if(a.speed<b.speed) return 1;
	if(b.speed<a.speed) return 0;
	if(a.poll_timer<b.poll_timer) return 1;
	if(b.poll_timer<a.poll_timer) return 0;
	return 0;
}

ostream& operator<<(ostream& o,Speed_tracker const& a){
	o<<"Speed_tracker(";
	o<<"last_distance:"<<a.last_distance;
	o<<" last_time:"<<a.last_time;
	o<<" speed:"<<a.speed;
	o<<" poll_timer:"<<a.poll_timer;
	return o<<")";
}

#ifdef SPEED_TRACKER_TEST
int main(){
	//TODO
}
#endif
