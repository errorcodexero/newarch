#include "output.h"
#include<cassert>
#include<set>
#include<iostream>
#include "interface.h"

using namespace std;

bool numbered_type(Output::Type a){
	return a!=Output::Type::PUMP;
}

Output::Output(Output::Type type,unsigned i):type_(type),index_(i){}

Output::Type Output::type()const{ return type_; }

unsigned Output::index()const{
	assert(numbered_type(type_));
	return index_;
}

#define X(A,B,C) Output Output::A(unsigned i){\
	assert(i<C);\
	return Output(Type::B,i);\
}
NUMBERED_TYPES
#undef X

Output Output::pump(){
	return Output(Type::PUMP,0);
}

ostream& operator<<(ostream& o,Output::Type a){
	#define X(A) if(a==Output::Type::A) return o<<""#A;
	OUTPUT_TYPES
	#undef X
	assert(0);
}

ostream& operator<<(ostream& o,Output const& a){
	o<<"Output("<<a.type();
	if(numbered_type(a.type())) o<<" "<<a.index();
	return o<<")";
}

bool operator<(Output const& a,Output const& b){
	if(a.type()<b.type()) return 1;
	if(b.type()<a.type()) return 0;
	if(numbered_type(a.type())) return a.index()<b.index();
	return 0;
}

set<Output> outputs(){
	set<Output> r;
	#define X(A,B,C) for(unsigned i=0;i<C;i++) r.insert(Output::A(i));
	NUMBERED_TYPES
	#undef X
	r.insert(Output::pump());
	return r;
}

#ifdef OUTPUT_TEST
int main(){
	for(auto a:outputs()){
		cout<<a<<"\n";
	}
}
#endif
