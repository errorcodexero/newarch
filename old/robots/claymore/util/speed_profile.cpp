#include "speed_profile.h"
#include <cassert>
#include <cmath>

SpeedProfile::SpeedProfile(double dist, double accel, double decel, double start, double cruise, double end, double time)
{
	type = Type::TRAPEZOID;

	cruising_v = cruise ;
	distance = dist ;
	acceleration = accel ;
	deceleration = decel ;
	start_v = start ;
	end_v = end ;
	start_time = time ;

	double accel_time = (cruising_v - start_v) / acceleration;
	double accel_dist = start_v * accel_time + .5 * acceleration * accel_time * accel_time;
	
	double decel_time = (end_v - cruising_v) / deceleration;
	double decel_dist = end_v * decel_time + .5 * -deceleration * decel_time * decel_time;

	double cruise_dist = distance - accel_dist - decel_dist;
	double cruise_time = cruise_dist / cruising_v;
	
	if(cruise_time < 0) {
		type = Type::TRIANGLE;

		meet_v = std::sqrt((2 * distance * acceleration * deceleration + deceleration * start_v * start_v - acceleration * end * end) / (deceleration - acceleration));

		accel_time = (meet_v - start_v) / acceleration;
		accel_dist = start_v * accel_time + .5 * acceleration * accel_time * accel_time;
		
		decel_time = (end_v - meet_v) / deceleration;
		decel_dist = end * decel_time + .5 * -deceleration * decel_time * decel_time;

		decel_point = time + accel_time;
	} else {
		cruise_point = time + accel_time;
		decel_point = cruise_point + cruise_time;
	}
	end_point = decel_point + decel_time;
}

double SpeedProfile::get(double time) {
	double elapsed = time - start_time;
	
	if(type == Type::TRIANGLE) {
		if(time < decel_point) return start_v + elapsed * acceleration;
		if(time < end_point) return meet_v + (elapsed - decel_point) * deceleration;
		return end_v;
	}

	if(type == Type::TRAPEZOID) {
		if(time < cruise_point) return start_v + elapsed * acceleration;
		if(time < decel_point) return cruising_v;
		if(time < end_point) return cruising_v + (elapsed - decel_point) * deceleration;
		return end_v;
	}

	assert(0);
}

bool SpeedProfile::done(double time) {
	return time > end_point;
}

#ifdef SPEED_PROFILE_TEST
#include <iostream>

int main() {
	std::cout<<"Trapazoid:\n";
	SpeedProfile trapazoid_profile(150.0, 1.0, -1.0, 0.0, 10.0, 0.0, 0.0);
	for(double t = 0; t < 30; t++) {
		std::cout<<"t="<<t<<" v="<<trapazoid_profile.get(t)<<" done:"<<trapazoid_profile.done(t)<<"\n";
	}

	std::cout<<"\nTriangle:\n";
	SpeedProfile triangle_profile(75.0, 2.0, -1.0, 0.0, 100.0, 0.0, 0.0);
	for(double t = 0; t < 20; t++) {
		std::cout<<"t="<<t<<" v="<<triangle_profile.get(t)<<" done:"<<triangle_profile.done(t)<<"\n";
	}
}
#endif
