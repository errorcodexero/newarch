#include <iostream>
#include <iomanip>
#include <cmath>
#include <assert.h>

#include "motion_target_profile.h"

static bool doubleAlmostEqual(double a, double b, double epsilon = 0.001)
{
    return std::abs(a - b) < epsilon;
}

MotionTargetProfile::MotionTargetProfile()
{
	reset();
}

void MotionTargetProfile::reset()
{
	profile_needs_computing = true;
	max_speed = UNSPECIFIED;
	max_acceleration = UNSPECIFIED;
	max_acceleration = UNSPECIFIED;
	total_distance = UNSPECIFIED;
	initial_speed = 0; // Assume initially at rest by default
	final_speed = 0;   // Assume finally at rest by default
}

void MotionTargetProfile::setMaxSpeed(double speed)
{
	max_speed = speed;
	profile_needs_computing = true;
}

void MotionTargetProfile::setMaxAcceleration(double acceleration)
{
	max_acceleration = acceleration;
	profile_needs_computing = true;
}

void MotionTargetProfile::setMaxDeceleration(double deceleration)
{
	max_deceleration = deceleration;
	profile_needs_computing = true;
}

void MotionTargetProfile::setTotalDistance(double distance)
{
	total_distance = distance;
	profile_needs_computing = true;
}

void MotionTargetProfile::setInitialSpeed(double speed)
{
	initial_speed = speed;
	profile_needs_computing = true;
}

void MotionTargetProfile::setFinalSpeed(double speed)
{
	final_speed = speed;
	profile_needs_computing = true;
}

double MotionTargetProfile::getTotalTargetTime() const
{
	assert(!profile_needs_computing);
	return total_time;
}

double MotionTargetProfile::getCurrentTargetSpeed(double current_time) const
{
	assert(!profile_needs_computing);
	double speed;
	if (current_time < rampup_time) {
		speed = initial_speed + (max_speed_reached - initial_speed) * (current_time / rampup_time);
	} else if (current_time > (total_time - rampdown_time)) {
		speed = final_speed + (max_speed_reached - final_speed) * ((total_time-current_time) / rampdown_time);
	} else {
		speed = max_speed_reached;
	}
	return speed;
}

double MotionTargetProfile::getCurrentTargetDistance(double current_time) const
{
	assert(!profile_needs_computing);
	double dist;
	if (current_time < rampup_time) {
		dist = initial_speed * current_time + 0.5 * max_acceleration * std::pow(current_time,2);
	} else if (current_time > (total_time - rampdown_time)) {
		dist = total_distance - (final_speed * (total_time-current_time) + 0.5 * max_deceleration * std::pow(total_time-current_time,2));
	} else {
		dist = rampup_dist + max_speed_reached * (current_time - rampup_time);
	}
	return dist;
}

bool MotionTargetProfile::computeProfile(bool verbose)
{
	assert(dataIsSpecified());
	assert(max_speed > 0);
	assert(initial_speed >= 0 && initial_speed <= max_speed);
	assert(final_speed >= 0 && final_speed <= max_speed);
	assert(max_acceleration > 0);
	assert(max_deceleration > 0);

	// Work out if we accelerate to max speed then decelerate to final speed
	rampup_time = (max_speed - initial_speed) / max_acceleration;
	rampup_dist = (initial_speed * rampup_time) + (0.5 * max_acceleration * std::pow(rampup_time, 2));
	rampdown_time = (max_speed - final_speed) / max_deceleration;
	rampdown_dist = (max_speed * rampdown_time) - (0.5 * max_deceleration * std::pow(rampdown_time, 2));
	double rampupdown_dist = rampup_dist + rampdown_dist;
	double dist_at_max_speed = total_distance - rampupdown_dist;
	is_trapezoidal_profile = (dist_at_max_speed > 0);  // Decide on trapezoidal vs. triangular speed profile
	if (is_trapezoidal_profile) {
		max_speed_reached = max_speed;
		total_time = rampup_time + rampdown_time + (dist_at_max_speed / max_speed);
	} else {
		double term1 = 2*max_acceleration*max_deceleration*total_distance;
		double term2 = max_acceleration*std::pow(final_speed,2);
		double term3 = max_deceleration*std::pow(initial_speed,2);
		double term4 = max_acceleration + max_deceleration;
		max_speed_reached = std::sqrt((term1+term2+term3)/term4);

		rampup_dist = (std::pow(max_speed_reached,2) - std::pow(initial_speed,2))/(2*max_acceleration);
		rampdown_dist = (std::pow(max_speed_reached,2) - std::pow(final_speed,2))/(2*max_deceleration);
		assert(doubleAlmostEqual(rampup_dist + rampdown_dist, total_distance));
		rampup_time = (max_speed_reached - initial_speed) / max_acceleration;
		rampdown_time = (max_speed_reached - final_speed) / max_deceleration;
		total_time = rampup_time + rampdown_time;
		dist_at_max_speed = 0;
	}

	if (verbose) {
		std::cout << "is_trapezoidal_profile = " << is_trapezoidal_profile << std::endl;
		std::cout << "rampup_time            = " << rampup_time << std::endl;
		std::cout << "rampup_dist            = " << rampup_dist << std::endl;
		std::cout << "rampdown_time          = " << rampdown_time << std::endl;
		std::cout << "rampdown_dist          = " << rampdown_dist << std::endl;
		std::cout << "max_speed_reached      = " << max_speed_reached << std::endl;
		std::cout << "dist_at_max_speed      = " << dist_at_max_speed << std::endl;
		std::cout << "total_time             = " << total_time << std::endl;
	}

	profile_needs_computing = false;
	return true; // Success
}

bool MotionTargetProfile::dataIsSpecified()
{
	const bool result = (max_speed != UNSPECIFIED) &&
						(max_acceleration != UNSPECIFIED) &&
						(max_deceleration != UNSPECIFIED) &&
						(total_distance != UNSPECIFIED);
	return result;
}

#ifdef MOTION_TARGET_PROFILE_TEST      // TODO: COMPARE RESULTS

void printProfile(const MotionTargetProfile& p)
{
	double timeIncr = p.getTotalTargetTime() / 50;
	std::cout.precision(4);
	for (double t=0; t<=p.getTotalTargetTime(); t+=timeIncr) {
		std::cout << t << ", " <<  p.getCurrentTargetSpeed(t) << ", " << p.getCurrentTargetDistance(t) << std::endl;
	}
}

void runTests(MotionTargetProfile& p)
{
	// Zero initial and final speeds
	p.setMaxSpeed(20);
	p.setMaxAcceleration(10);
	p.setMaxDeceleration(10);
	p.setInitialSpeed(0);
	p.setFinalSpeed(0);
	p.computeProfile(true);
	printProfile(p);

	// Non-zero, same initial and final speeds
	p.setInitialSpeed(5);
	p.setFinalSpeed(5);
	p.computeProfile(true);
	printProfile(p);

	// Non-zero, different initial and final speeds
	p.setInitialSpeed(5);
	p.setFinalSpeed(15);
	p.computeProfile(true);
	printProfile(p);

	// Non-zero, different initial and final speeds
	p.setInitialSpeed(15);
	p.setFinalSpeed(5);
	p.computeProfile(true);
	printProfile(p);
}


int main()
{
	MotionTargetProfile p;

	p.setTotalDistance(100);
	runTests(p);    // Trapezoidal cases

	p.setTotalDistance(20);
	runTests(p);    // Triangular cases

	return 0;
}
#endif
