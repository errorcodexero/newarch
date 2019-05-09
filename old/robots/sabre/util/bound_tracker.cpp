#include "bound_tracker.h"
#include<algorithm>
#include<iostream>

using namespace std;

Bound_tracker::Bound_tracker():data(0){
	min = (strtod("NAN",NULL));
	max = (strtod("NAN",NULL));
}
	
void Bound_tracker::update(double v){
	if(data){
		//std::min and std::max don't work right on the cRIO.
		//min=std::min(min,v);
		//max=std::max(max,v);
		if(v<min) min=v;
		if(v>max) max=v;
	}else{
		min=max=v;
		data=1;
	}
}

std::ostream& operator<<(std::ostream& o,Bound_tracker a){
	o<<"Bound_tracker(";
	if(a.data){
		o<<a.min<<" "<<a.max;
	}else{
		o<<"n/a";
	}
	return o<<")";
}

#ifdef BOUND_TRACKER_TEST
int main(){
	Bound_tracker b;
	cout<<b<<"\n";
	b.update(4);
	cout<<b<<"\n";
	b.update(2);
	cout<<b<<"\n";
}
#endif
