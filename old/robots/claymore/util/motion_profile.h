#ifndef MOTION_PROFILE_H
#define MOTION_PROFILE_H
#include <functional>
#include <assert.h>

class Motion_profile{
	double goal;//inches
	double vel_modifier;
	double max;
	
	public:
	double target_speed(const double);
	void set_goal(double);

	Motion_profile();
	Motion_profile(double,double,double);
	friend std::ostream& operator<<(std::ostream&,Motion_profile const&);
	friend bool operator==(Motion_profile const& a,Motion_profile const& b);
};

double target_to_out_power(double);
double target_to_out_power(double,double);

bool operator==(Motion_profile const& a,Motion_profile const& b);
std::ostream& operator<<(std::ostream&,Motion_profile const&);

#endif
