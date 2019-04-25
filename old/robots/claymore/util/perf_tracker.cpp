#include "perf_tracker.h"
#include<algorithm>
#include<iostream>
#include<cassert>

//This class is for monitoring the performance of the robot by keeping track of the average and longest times that it has taken to complete a loop.  To use it, call update() with the current time once per loop.

using namespace std;

/*
A couple of assumtions about the input times:
-they are all positive and monotonically increasing.
*/
Perf_tracker::Perf_tracker():first(-1),last(-1),worst(-1),i(-1){}

void Perf_tracker::update(Time now){
	if(first==-1) first=now;
	if(last!=-1){
		Time elapsed=now-last;
		worst=max(worst,elapsed);
	}
	last=now;
	i++;
}

void Perf_tracker::reset_worst(){ worst=-1; }

Time Perf_tracker::average()const{
	if(i<1) return -1;//should probably return NAN, but need to figure out what that does on the robot before using it.
	return (last-first)/i;
}

Time Perf_tracker::worst_case()const{
	return worst;
}

bool operator==(Perf_tracker a,Perf_tracker b){
	return a.first==b.first && a.last==b.last && a.worst==b.worst && a.i==b.i;
}

bool operator!=(Perf_tracker a,Perf_tracker b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Perf_tracker a){
	o<<"Perf_tracker(";
	//o<<a.first<<" "<<a.last<<" "<<a.worst<<" "<<a.i;
	o<<a.average()<<" "<<a.worst_case();
	return o<<")";
}

bool approx_equal(Perf_tracker a,Perf_tracker b){
	return a.worst_case()==b.worst_case() && a.average()==b.average();
}

#ifdef PERF_TRACKER_TEST
int main(){
	Perf_tracker p;
	cout<<p<<"\n";
	p.update(0);
	p.update(1);
	p.update(1);
	cout<<p<<"\n";
	cout<<"worst case:"<<p.worst_case()<<"\n";
	cout<<"average:"<<p.average()<<"\n";
}
#endif
