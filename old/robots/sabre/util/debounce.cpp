#include "debounce.h"

using namespace std;

//after a transition, ignore any other for .5 seconds

Debounce::Debounce():value(0){}

bool Debounce::get()const{ return value; }

void Debounce::update(Time now,bool v){
	timer.update(now,1);
	if(!timer.done()) return;
	if(v==value) return;
	value=v;
	timer.set(.5);
	timer.update(now,1);
}

ostream& operator<<(ostream& o,Debounce const& a){
	o<<"Debounce( ";
	#define X(A,B) o<<""#B<<":"<<a.B<<" ";
	DEBOUNCE_ITEMS(X)
	#undef X
	return o<<")";
}

#ifdef DEBOUNCE_TEST
int main(){
	Debounce d;
	assert(d.get()==0);
	d.update(0,1);
	assert(d.get()==1);
	d.update(.2,0);
	assert(d.get()==1);
	d.update(.8,0);
	cout<<d<<"\n";
	assert(d.get()==0);
}
#endif
