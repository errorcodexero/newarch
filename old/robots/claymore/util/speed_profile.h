#ifndef SPEED_PROFILE
#define SPEED_PROFILE

class SpeedProfile {
	enum class Type { TRAPEZOID, TRIANGLE };
	Type type;

	double start_time; //Robot time of profile start
	double distance; //Distance to travel
	double acceleration, deceleration; //Rates of acceleration and deceleration
	double start_v, end_v; //Start and end velocity
	double decel_point, end_point; //Times at which to begin decelerating and to end

	//Trapazoid-specific	
	double cruising_v; //Cruise velocity
	double cruise_point; //Time at which to begin cruising

	//Triangle-specific
	double meet_v; //Meet velocity

	public:
	SpeedProfile(double,double,double,double,double,double,double);

	double get(double);
	bool done(double);
};

#endif
