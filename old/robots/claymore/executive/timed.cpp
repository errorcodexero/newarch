#if 0
#include "timed.h"
#include "chain.h"
#include "teleop.h"

using namespace std;

struct A:Step_impl_inner<A>{
	Countdown_timer timer;
	Toplevel::Goal goal;

	Toplevel::Goal run(Run_info){
		nyi//return Goal
	}

	bool done(Next_mode_info a){
		timer.update(a.in.now,a.in.robot_mode.enabled);
		return timer.done();
	}

	bool operator==(A const&)const{
		nyi
	}
};

Step timed(Time /*time*/,Toplevel::Goal /*goal*/){
	return Step{A()};
}

Executive timed(Time time,Toplevel::Goal goal,Executive next){
	return Executive{Chain{timed(time,goal),next}};
}
#endif
#ifdef TIMED_TEST
#include "test.h"
int main(){
	//TODO: Give it something other than "Teleop" as next.
	//test_executive(timed(0,example((Toplevel::Goal*)0),Executive{Teleop{}}));
}
#endif
