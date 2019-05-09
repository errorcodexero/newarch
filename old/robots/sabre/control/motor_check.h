#ifndef MOTOR_CHECK_H
#define MOTOR_CHECK_H

#include<set>
#include "../util/util.h"

typedef double Amps;

class Motor_check{
	public:
	//not named OK because that is a macro in WPIlib
	enum class Status{OK_,STOPPED_AND_NONZERO,GOING_AND_ZERO};

	private:
	double old_power;
	Time old_power_start;
	Status status;
	std::set<Status> seen_;

	Maybe_inline<Status> run(Time time,Amps current,double set_power_level);

	public:
	Motor_check();

	void update(Time time,Amps current,double set_power_level);
	Status get()const;
	std::set<Status> const& seen()const;
};

bool operator==(Motor_check,Motor_check);
bool operator!=(Motor_check,Motor_check);

std::ostream& operator<<(std::ostream& o,Motor_check::Status);

#endif
