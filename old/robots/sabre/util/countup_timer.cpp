#include "countup_timer.h"
#include<iostream>
#include<cassert>

using namespace std;

Countup_timer::Countup_timer():start(-1),latest(-1){}

void Countup_timer::update(Time now,bool reset){
	if(reset){
		start=-1;
	}
	if(start==-1){
		start=now;
	}
	latest=now;
}

Time Countup_timer::elapsed()const{ return latest-start; }

void Countup_timer::out(ostream& o)const{
	o<<"Countup_timer(";
	o<<start<<","<<latest;
	o<<")";
}

bool operator==(Countup_timer a,Countup_timer b){
	return a.start==b.start && a.latest==b.latest;
}

ostream& operator<<(ostream& o,Countup_timer c){
	c.out(o);
	return o;
}

#ifdef COUNTUP_TIMER_TEST
int main(){
	Countup_timer c;
	cout<<c<<"\n";
	assert(c.elapsed()>=0 && c.elapsed()<.1);
	for(unsigned i=0;i<100;i++){
		c.update(i/10.0,0);
	}
	cout<<c<<"\n";
	assert(c.elapsed()>9.5 && c.elapsed()<10.5);
}
#endif
