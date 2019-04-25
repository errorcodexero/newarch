#include "executive.h"
#include<queue>

using namespace std;

IMPL_STRUCT(Next_mode_info::Next_mode_info,NEXT_MODE_INFO_ITEMS)
IMPL_STRUCT(Run_info::Run_info,RUN_INFO_ITEMS)

Executive& Executive::operator=(Executive const& a){
	assert(a.impl);
	impl=a.impl->clone();
	return *this;
}

Executive::Executive(Executive const& a){
	assert(a.impl);
	impl=a.impl->clone();
}

Executive::Executive(Executive_interface const& a):impl(a.clone()){
	assert(impl);
}

Executive_interface const& Executive::get()const{
	auto g=impl.get();
	assert(g);
	return *g;
}

Executive Executive::next_mode(Next_mode_info a){
	assert(impl);
	return impl->next_mode(a);
}

Toplevel::Goal Executive::run(Run_info a){
	assert(impl);
	return impl->run(a);
}

bool operator<(Executive const& a,Executive const& b){
	return a.get()<b.get();
}

bool operator==(Executive const& a,Executive const& b){
	assert(a.impl);
	return a.impl->operator==(b);
}

ostream& operator<<(ostream& o,Executive_interface const& a){
	a.display(o);
	return o;
}

ostream& operator<<(ostream& o,Executive const& a){
	return o<<*a.impl;
}

bool rand(bool*){
	return rand()%2;
}

Time rand(Time*){
	//chosen arbitrarily.
	return Time(rand()%100);
}

pair<int,int> rand(pair<int,int>*){
	return make_pair(rand(),rand());
}

Next_mode_info rand(Next_mode_info*){
	return Next_mode_info{
		#define X(A,B) rand((A*)0),
		NEXT_MODE_INFO_ITEMS(X)
		#undef X
	};
}

Next_mode_info example(Next_mode_info *a){
	return rand(a);
}

vector<Next_mode_info> examples(Next_mode_info *a){
	return mapf([&](int){ return example(a); },range(100));
}

Joystick_data rand(Joystick_data*){
	return driver_station_input_rand();
}

Run_info rand(Run_info*){
	return Run_info{
		#define X(A,B) rand((A*)0),
		RUN_INFO_ITEMS(X)
		#undef X
	};
}

vector<Run_info> examples(Run_info *a){
	return mapf([=](int){ return rand(a); },range(100));
}

bool operator==(Next_mode_info const& a,Next_mode_info const& b){
	#define X(A,B) if(a.B!=b.B) return 0;
	NEXT_MODE_INFO_ITEMS(X)
	#undef X
	return 1;
}

bool operator==(Run_info const& a,Run_info const& b){
	#define X(A,B) if(a.B!=b.B) return 0;
	RUN_INFO_ITEMS(X)
	#undef X
	return 1;
}

/*bool operator==(Executive const& a,Executive const& b){
	return a.equals(b);
}*/

bool operator!=(Executive const& a,Executive const& b){
	return !(a==b);
}

bool operator<(unique_ptr<Executive> const& a,unique_ptr<Executive> const& b){
	if(a.get()){
		if(b.get()){
			return *a<*b;
		}
		return 0;
	}
	return a.get()<b.get();
}

#ifdef EXECUTIVE_TEST
int main(){
	{
		Next_mode_info *a(0);
		auto x=rand(a);
		assert(x==x);
	}
	{
		Run_info *b=0;
		auto x=rand(b);
		assert(x==x);
	}
}
#endif
