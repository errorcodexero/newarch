#include "visitable.h"
#include<iostream>

using namespace std;

#ifdef VISITABLE_TEST
struct A:Visitable<A>{
	bool enable;
	string s;

	A():enable(0),s(""){}

	explicit A(bool a,auto b):enable(a),s(b){
	}
};

set<A> examples(const A*){
	return {};
}

void visit(size_t i,auto &f,A const& a){
	switch(i){
		case 0:
			f("enable",a.enable);
			break;
		case 1:
			f("s",a.s);
			break;
		default:
			f.terminate();
	}
}

int main(){
	A a1{false,"a"},a2{0,"1"};
	cout<<(a1==a2)<<"\n";
	cout<<(a1>a2)<<"\n";
	cout<<(a1<a2)<<"\n";
	cout<<a1<<"\n";
	cout<<examples(&a1)<<"\n";
}
#endif
