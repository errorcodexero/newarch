#include "motor_check.h"
#include<iostream>
#include<set>
#include<cmath>
#include "../util/util.h"

using namespace std;

bool operator==(Motor_check a, Motor_check b){
	return (a.get()==b.get() && a.seen()==b.seen());
}

bool operator!=(Motor_check a,Motor_check b){ return !(a==b); }

Maybe_inline<Motor_check::Status> Motor_check::run(Time time,Amps current,double set_power_level){
	static const Maybe_inline<Status> NO_CHECK;
	//assert(current>=0);
	current=fabs(current);//TODO: Figure out what negative current means

	if(old_power_start==-1 || old_power!=set_power_level){
		old_power_start=time;
		old_power=set_power_level;
		return NO_CHECK;
	}

	Time elapsed=time-old_power_start;
	if(elapsed<.5) return NO_CHECK;

	//may need to check this is right for all speed controller types
	static const auto SPEED_CONTROL_DEADZONE=.03;

	if(fabs(set_power_level)<SPEED_CONTROL_DEADZONE){
		return Maybe_inline<Status>{(current<.5)?Status::OK_:Status::STOPPED_AND_NONZERO};
	}else if(fabs(set_power_level)>.2){
		return Maybe_inline<Status>{(current>=1)?Status::OK_:Status::GOING_AND_ZERO};
	}
	return NO_CHECK;
}

Motor_check::Motor_check():old_power(0),old_power_start(-1),status(Status::OK_){}

void Motor_check::update(Time time,Amps current,double set_power_level){
	auto x=run(time,current,set_power_level);
	if(x){
		seen_.insert(*x);
		status=*x;
	}
}

Motor_check::Status Motor_check::get()const{
	return status;
}

set<Motor_check::Status> const& Motor_check::seen()const{ return seen_; }

ostream& operator<<(ostream& o,Motor_check::Status a){
	#define X(NAME) if(a==Motor_check::Status::NAME) return o<<""#NAME;
	X(OK_) X(STOPPED_AND_NONZERO) X(GOING_AND_ZERO)
	#undef X
	assert(0);
}

#ifdef MOTOR_CHECK_TEST
int main(){
	Motor_check a;
	assert(a.get()==Motor_check::Status::OK_);

	a.update(0,0,1);
	a.update(1,0,1);
	assert(a.get()==Motor_check::Status::GOING_AND_ZERO);

	a.update(2,0,0);
	a.update(2.7,0,0);
	assert(a.get()==Motor_check::Status::OK_);

	a.update(4,4,0);
	assert(a.get()==Motor_check::Status::STOPPED_AND_NONZERO);

	a.update(5,0,0);
	assert(a.get()==Motor_check::Status::OK_);

	cout<<a.seen()<<"\n";
}
#endif
