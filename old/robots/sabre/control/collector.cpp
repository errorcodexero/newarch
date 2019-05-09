#include "collector.h"

#define nyi { cout<<"nyi "<<__LINE__<<"\n"; exit(44); }

Collector::Goal::Goal():front(Front::Goal::OFF),sides(Sides::Goal::OFF),tilt(Tilt::Goal::stop()){}

Collector::Goal::Goal(Front::Goal f,Sides::Goal s, Tilt::Goal t):front(f),sides(s),tilt(t){}

ostream& operator<<(ostream& o,Collector const&){
	return o<<"Collector()";
}

ostream& operator<<(ostream& o,Collector::Output const& a){
	o<<"Collector::Output( ";
	#define X(A,B) o<<a.B<<" ";
	COLLECTOR_ITEMS(X)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Collector::Input const& a){
	o<<"Collector::Input( ";
	#define X(A,B) o<<a.B<<" ";
	COLLECTOR_ITEMS(X)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Collector::Status const& a){
	o<<"Collector::Status( ";
	#define X(A,B) o<<a.B<<" ";
	COLLECTOR_ITEMS(X)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Collector::Status_detail const& a){
	o<<"Collector::Status_detail( ";
	#define X(A,B) o<<a.B<<" ";
	COLLECTOR_ITEMS(X)
	#undef X
	return o<<")";
}

ostream& operator<<(ostream& o,Collector::Goal const& a){
	o<<"Collector::Goal( ";
	#define X(A,B) o<<a.B<<" ";
	COLLECTOR_ITEMS(X)
	#undef X
	return o<<")";
}

Robot_inputs Collector::Input_reader::operator()(Robot_inputs a,Collector::Input b)const{
	#define X(A,B) a=B(a,b.B);
	COLLECTOR_ITEMS(X)
	#undef X
	return a;
}

Collector::Input Collector::Input_reader::operator()(Robot_inputs const& a)const{
	return {
		#define X(A,B) B(a),
		COLLECTOR_ITEMS(X)
		#undef X
	};
}

bool operator==(Collector const&,Collector const&){
	return 1;
}

bool operator!=(Collector const& a,Collector const& b){
	return !(a==b);
}

bool operator<(Collector::Output a,Collector::Output b){
	#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	COLLECTOR_ITEMS(X)
	#undef X
	return 0;
}

bool operator<(Collector::Input,Collector::Input)nyi

bool operator<(Collector::Goal a,Collector::Goal b){
	#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	COLLECTOR_ITEMS(X)
	#undef X
	return 0;
}

bool operator==(Collector::Status a,Collector::Status b){
	#define X(A,B) if(a.B!=b.B) return 0;
	COLLECTOR_ITEMS(X)
	#undef X
	return 1;
}

bool operator!=(Collector::Status a,Collector::Status b){
	return !(a==b);
}

bool operator==(Collector::Estimator a,Collector::Estimator b){
	#define X(A,B) if(a.B!=b.B) return 0;
	COLLECTOR_ITEMS(X)
	#undef X
	return 1;
}

bool operator!=(Collector::Estimator a,Collector::Estimator b){
	return !(a==b);
}

bool operator==(Collector::Output a,Collector::Output b){
	#define X(A,B) if(a.B!=b.B) return 0;	
	COLLECTOR_ITEMS(X)
	#undef X
	return 1;
}

bool operator!=(Collector::Input a,Collector::Input b){
	return !(a==b);
}

bool operator==(Collector::Input a,Collector::Input b){
	#define X(A,B) if(a.B!=b.B) return 0;	
	COLLECTOR_ITEMS(X)
	#undef X
	return 1;
}

bool operator!=(Collector::Output a,Collector::Output b){
	return !(a==b);
}

bool operator<(Collector::Status a,Collector::Status b){
	#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	COLLECTOR_ITEMS(X)
	#undef X
	return 0;
}

bool operator==(Collector::Status_detail a,Collector::Status_detail b){
	#define X(A,B) if(a.B!=b.B) return 0;	
	COLLECTOR_ITEMS(X)
	#undef X
	return 1;
}

bool operator!=(Collector::Status_detail a,Collector::Status_detail b){
	return !(a==b);
}

bool operator<(Collector::Status_detail a,Collector::Status_detail b){
	#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	COLLECTOR_ITEMS(X)
	#undef X
	return 0;
}

Collector::Status status(Collector::Status_detail const& a){
	return {
		#define X(A,B) status(a.B),
		COLLECTOR_ITEMS(X)
		#undef X
	};
}

Collector::Output control(Collector::Status_detail const& st,Collector::Goal const& goal){
	return {
		#define X(A,B) control(st.B,goal.B),
		COLLECTOR_ITEMS(X)
		#undef X
	};
}

void Collector::Estimator::update(Time a,Input b,Output c){
	#define X(A,B) B.update(a,b.B,c.B);
	COLLECTOR_ITEMS(X)
	#undef X
}

Collector::Status_detail Collector::Estimator::get()const{
	return Collector::Status_detail{
		#define X(A,B) B.get(),
		COLLECTOR_ITEMS(X)
		#undef X
	};
}

Collector::Output Collector::Output_applicator::operator()(Robot_outputs const& a)const{
	return Collector::Output{
		#define X(A,B) B(a),
		COLLECTOR_ITEMS(X)
		#undef X
	};
}

Robot_outputs Collector::Output_applicator::operator()(Robot_outputs a,Output const& b)const{
	#define X(A,B) a=B(a,b.B);
	COLLECTOR_ITEMS(X)
	#undef X
	return a;
}

set<Collector::Input> examples(Collector::Input*){
	return {{
		#define X(A,B) example((A::Input*)nullptr),
		COLLECTOR_ITEMS(X)
		#undef X
	}};
}

set<Collector::Status_detail> examples(Collector::Status_detail*){
	/*return {{
		#define X(A,B) example((A::Status_detail*)nullptr),
		COLLECTOR_ITEMS(X)
		#undef X
	}};*/
	set<Collector::Status_detail> r;
	for(auto a:examples((Front::Status_detail*)0)){
		for(auto b:examples((Sides::Status_detail*)0)){
			for(auto c:examples((Tilt::Status_detail*)0)){
				r|=Collector::Status_detail{a,b,c};
			}
		}
	}
	return r;
}

/*template<typename A,typename B,typename C>
auto cross(A a,B b,C c){

}*/

set<Collector::Goal> examples(Collector::Goal*){
	/*return {{
		#define X(A,B) example((A::Goal*)nullptr),
		COLLECTOR_ITEMS(X)
		#undef X
	}};*/
	set<Collector::Goal> r;
	for(auto a:examples((Front::Goal*)0)) for(auto b:examples((Sides::Goal*)0)){
		for(auto c:examples((Tilt::Goal*)0)){
			r|=Collector::Goal{a,b,c};
		}
	}
	return r;
}

set<Collector::Output> examples(Collector::Output*){
	set<Collector::Output> r;
	for(auto a:examples((Front::Output*)0)){
		for(auto b:examples((Sides::Output*)0)){
			for(auto c:examples((Tilt::Output*)0)){
				r|=Collector::Output{a,b,c};
			}
		}
	}
	return r;
	//return {{}};
	/*	#define X(A,B) example((A::Output*)nullptr),
		COLLECTOR_ITEMS(X)
		#undef X
	}};*/
}

set<Collector::Status> examples(Collector::Status*){
	set<Collector::Status> r;
	for(auto a:examples((Front::Status*)nullptr)){
		for(auto b:examples((Sides::Status*)nullptr)){
			for(auto c:examples((Tilt::Status*)0)){
				r|=Collector::Status{a,b,c};
			}
		}
	}
	return r;
}

bool ready(Collector::Status const& st,Collector::Goal const& goal){
	#define X(A,B) if(!ready(st.B,goal.B)) return 0;
	COLLECTOR_ITEMS(X)
	#undef X
	return 1;
}

#ifdef COLLECTOR_TEST
#include "formal.h"
int main(){
	Tester_mode mode;
	mode.check_outputs_exhaustive=0;
	Collector a;
	tester(a,mode);
	return 0;
}
#endif
