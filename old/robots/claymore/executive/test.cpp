#include "test.h"
#include <queue>
#include "teleop.h"

using namespace std;

template<typename T>
queue<T>& operator|=(queue<T> &q,T t){
	q.push(t);
	return q;
}

//This should probably go in its own file.
template<typename T>
class Unique_queue{
	set<T> s;
	queue<T> q;

	public:
	void insert(T t){
		auto p=s.insert(t);
		if(p.second){
			try{
				q|=t;
			}catch(...){
				//could decide to try to do something better w/ exception, like remove from the set then rethrow
				abort();
			}
		}
	}

	T pop(){
		try{
			auto r=q.front();
			q.pop();
			s.erase(r);
			return r;
		}catch(...){
			//no, throwing exceptions is not supported in this.
			abort();
		}
	}

	bool empty()const{
		return q.empty();
	}
};

template<typename T>
Unique_queue<T> &operator|=(Unique_queue<T> &a,T t){
	a.insert(t);
	return a;
}

template<typename T>
Unique_queue<T> &operator|=(Unique_queue<T> &a,vector<T> v){
	for(auto elem:v) a|=elem;
	return a;
}

//TODO: Move to util?
template<typename Node,typename F>
auto traverse(Node node,F next){
	Unique_queue<Node> to_visit;
	to_visit|=node;

	set<Node> visited;

	//Just chose some number that probably means that there's something depending on some floats.
	static const size_t MAX_STATES=100;
	while(!to_visit.empty() && visited.size()<MAX_STATES){
		auto at=to_visit.pop();
		visited|=at;

		//cout<<"Visiting:"<<at<<"\n";
		auto n=next(at);
		to_visit|=(n-visited);
	}

	//cout<<"Visited "<<visited.size()<<"\n";
	return visited;
}

void print_diff(Executive const& a,Executive const& b){
	if(a==b) return;
	a.get().diff(cout,b.get());
}

void show(string name,auto a){
	cout<<name<<":\n";
	for(auto elem:take(20,to_vec(to_set(a)))){
		cout<<"\t"<<elem<<"\n";
	}
}

void compare(set<Toplevel::Goal> s){
	#define X(A,B,C) show(""#A,mapf([](auto a){ return a.B; },s));
	TOPLEVEL_ITEMS
	#undef X
}

void test_executive(Executive mode){
	assert(mode==mode);
	assert(!(mode<mode));
	/*Things that could try doing:
	-see what the next modes show up as being if run a bunch of different stuff through
	-see if the internal state changes when different things happend to it & try to explore the state space
	-see what the Goals may be when a bunch of random stuff is run through.
	*/
	mode.next_mode(rand((Next_mode_info*)0));
	assert(mode==mode);
	auto a=mode;

	/*set<Executive> nexts;
	//should the next_mode function be const?
	for(auto _:range(100)){
		(void)_;
		auto n=mode.next_mode(rand((Next_mode_info*)0));
		nexts.insert(move(n));
	}
	PRINT(nexts.size());
	PRINT(nexts);*/
	auto nexts=traverse(
		mode,
		[&](Executive at){
			return mapf(
				[&](Next_mode_info next_mode_info){
					auto r=at.next_mode(next_mode_info);
					/*if(r!=at){
						print_diff(r,at);
					}*/
					return r;
				},
				examples((Next_mode_info*)0)
			);
		}
	);
	cout<<"Nexts:\n";
	print_lines(nexts);

	PRINT(mode);
	{	
		Executive test_if_teleop= mode.next_mode(Next_mode_info{false,false,rand((Toplevel::Status_detail*)0),0.0,rand((Panel*)0),rand((Robot_inputs*)0)});
		assert( test_if_teleop == Executive{Teleop{}});//test to make sure that each mode goes to Teleop if it's not in auto mode. Maybe expand this test later.
	}
	set<Toplevel::Goal> outs;
	auto after=traverse(
		mode,
		[&](Executive at){
			return mapf(
				[&](Run_info run_info){
					auto b=at;
					outs|=b.run(run_info);
					return b;
				},
				examples((Run_info*)0)
			);
		}
	);

	//cross(nexts,examples((Run_info*)0));

	/*for(auto _:range(100)){
		(void)_;
		auto out=mode.run(rand((Run_info*)0));
		outs|=out;
	}*/
	//could do something more sophisticated to show what the changes are, per subsystem
	//PRINT(outs);
	compare(outs);

	set<Executive> after_run;
	mode.run(rand((Run_info*)0));
	if(a!=mode){
		//assert(0);//acutally should implement something
	}
}

#ifdef TEST_TEST
int main(){}
#endif
