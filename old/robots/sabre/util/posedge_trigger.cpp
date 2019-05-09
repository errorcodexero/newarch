#include "posedge_trigger.h"
#include<iostream>
#include<cassert>

using namespace std;

Posedge_trigger::Posedge_trigger():last(1){}

Posedge_trigger::Posedge_trigger(bool assume_previous):last(assume_previous){}

bool Posedge_trigger::operator()(bool b){
	bool r=!last && b;
	last=b;
	return r;
}

bool operator==(Posedge_trigger a,Posedge_trigger b){
	return a.last==b.last;
}

bool operator!=(Posedge_trigger a,Posedge_trigger b){
	return !(a==b);
}

ostream& operator<<(ostream& o,Posedge_trigger p){
	return o<<"+e("<<p.last<<")";
}

#ifdef POSEDGE_TRIGGER_TEST
#include<cassert>

int main(){
	Posedge_trigger p;
	bool init=p(1);
	bool a=p(0);
	bool b=p(1);
	bool c=p(1);
	assert(!init);
	assert(!a);
	assert(b);
	assert(!c);
	return 0;
}
#endif
