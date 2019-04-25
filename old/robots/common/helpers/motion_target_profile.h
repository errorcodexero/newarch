#pragma once

class MotionTargetProfile
{
public:
	MotionTargetProfile();
	virtual ~MotionTargetProfile() {}
	void reset();
	void setMaxSpeed(double speed);
	void setMaxAcceleration(double acceleration);
	void setMaxDeceleration(double deceleration);
	void setTotalDistance(double distance);
	void setInitialSpeed(double speed);
	void setFinalSpeed(double speed);
	double getTotalTargetTime() const;
	double getCurrentTargetSpeed(double current_time) const;
	double getCurrentTargetDistance(double current_time) const;
	bool computeProfile(bool verbose = false); // Given hardware and operational parameters, calculate motion profile

private:
	bool dataIsSpecified();

	const double UNSPECIFIED = -1;
	bool profile_needs_computing;

	// Parameters that are properties of the hardware
	double max_speed;				 // Max target speed.  Speed is unit of distance per sec.  May not be reached if distance is short.
	double max_acceleration; // Max acceleration.  Acceleration is unit of speed per sec.
	double max_deceleration; // Max deceleration.  Acceleration is unit of speed per sec.

	// Operational parameters provided by client
	double total_distance;
	double initial_speed;
	double final_speed;

	// Computed motion profile
	double rampup_dist;
	double rampup_time;
	double rampdown_dist;
	double rampdown_time;
	double max_speed_reached;
	double total_time;
	bool is_trapezoidal_profile;
};
