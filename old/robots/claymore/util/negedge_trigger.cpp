#include "negedge_trigger.h"
#include<iostream>

using namespace std;

Negedge_trigger::Negedge_trigger():last(0){}

bool Negedge_trigger::operator()(bool b){
	bool r=last && !b;
	last=b;
	return r;
}

ostream& operator<<(ostream& o,Negedge_trigger p){
	return o<<"+e("<<p.last<<")";
}

#ifdef NEGEDGE_TRIGGER_TEST
#include<cassert>

int main(){
	Negedge_trigger p;
	bool init=p(1);
	bool a=p(0);
	bool b=p(1);
	bool c=p(1);
	assert(!init);
	assert(a);
	assert(!b);
	assert(!c);
	return 0;
}
#endif
