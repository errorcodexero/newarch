#include "settable_toggle.h"
#include<iostream>
#include<cassert>

using namespace std;

Settable_toggle::Settable_toggle():value(0){}

void Settable_toggle::set(bool v){ value=v; }

void Settable_toggle::update(bool b){
	if(trig(b)) value=!value;
}

bool Settable_toggle::get()const{ return value; }

ostream& operator<<(ostream& o,Settable_toggle a){
	return o<<"Settable_toggle("<<a.trig<<a.value<<")";
}

bool operator==(Settable_toggle a,Settable_toggle b){
	return a.trig==b.trig && a.value==b.value;
}

bool operator!=(Settable_toggle a,Settable_toggle b){ return !(a==b); }

#ifdef SETTABLE_TOGGLE_TEST
int main(){
	Settable_toggle a;
	assert(a.get()==0);
	a.update(0);
	a.update(1);
	assert(a.get()==1);
	a.update(0);
	assert(a.get()==1);
	a.update(1);
	assert(a.get()==0);
	cout<<a<<"\n";
}
#endif
