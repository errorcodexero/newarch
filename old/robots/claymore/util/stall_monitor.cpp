#include "stall_monitor.h"
#include <cmath>
#include<iomanip>

using namespace std;

Stall_monitor::Stall_monitor():currents(Persistent_tracker<double>{KEEP}),speeds(Persistent_tracker<double>{KEEP}){}

void Stall_monitor::update(double current,double velocity){
	//update speeds
	double speed = fabs(velocity);
	
	speeds.update(speed);	
	
	mean_speed = [&]{
		if(speeds.get().empty()) return Maybe<double>{};
		double sum = 0;
		for(double a: speeds.get()){
			sum += a;
		}
		return Maybe<double>{sum / (double)speeds.get().size()};
	}();

	//update currents
	currents.update(current);
	
	max_current = [&]{
		if(currents.get().empty()) return Maybe<double>{};
		Maybe<double> max;
		for(double a: currents.get()){
			if(!max || a > max) max = a;
		}
		return max;
	}();

	{//update stall
		bool spike = false, slow = false;
	
		if(max_current){
			static const double STALL_CURRENT = 1;//TODO
			spike = *max_current > STALL_CURRENT;
		}
		if(mean_speed){
			static const double STALL_SPEED = 1;//TODO
			slow = *mean_speed < STALL_SPEED;
		}
		stall = spike && slow;
	}
}

Maybe<bool> Stall_monitor::get()const{
	return stall;	
}

ostream& operator<<(ostream& o,Stall_monitor const& a){
	o<<"Stall_monitor(";
	o<<fixed<<setprecision(2)<<"currents:" <<a.currents;
	o<<fixed<<setprecision(2)<<" max_current:" <<a.max_current;
	o<<fixed<<setprecision(2)<<" speeds:"<<a.speeds;
	o<<fixed<<setprecision(2)<<" mean_speed:"<<a.mean_speed;
	o<<" stall:"<<a.stall;
	return o<<")";
}

#ifdef STALL_MONITOR_TEST

int main(){
	Stall_monitor a;

	double current = 1, speed = 2;
	for(Time t = 0; t < 10; t += .1){
		a.update(current,speed);
		cout<<fixed<<setprecision(2)<<t<<"   current:"<<current<<"   speed:"<<speed<<"   "<<a<<"\n";
		speed *= .8;
		current *= 1.2;
	}
}

#endif
