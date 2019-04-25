#include "posedge_toggle.h"
#include<iostream>
#include<cassert>

Posedge_toggle::Posedge_toggle():value(0),last(1){}

void Posedge_toggle::update(bool sample){
	if(!last && sample) value=!value;
	last=sample;
}

bool Posedge_toggle::get()const{ return value; }

bool operator<(Posedge_toggle a,Posedge_toggle b){
	int a1=(a.value<<1)+a.last;
	int b1=(b.value<<1)+b.last;
	return a1<b1;
}

bool operator==(Posedge_toggle a,Posedge_toggle b){
	return a.value==b.value && a.last==b.last;
}

bool operator!=(Posedge_toggle a,Posedge_toggle b){
	return !(a==b);
}

std::ostream& operator<<(std::ostream& o,Posedge_toggle a){
	return o<<"Posedge_toggle("<<a.last<<" "<<a.value<<")";
}
