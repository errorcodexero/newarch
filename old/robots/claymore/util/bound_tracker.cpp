#include "bound_tracker.h"
#include<algorithm>
#include<iostream>

using namespace std;

#ifdef BOUND_TRACKER_TEST
int main(){
	Bound_tracker<double> b;
	cout<<b;
	cout<<b<<"\n";
	b.update(4);
	cout<<b<<"\n";
	b.update(2);
	cout<<b<<"\n";
	b.update(6);
	cout<<b<<"\n";
}
#endif
