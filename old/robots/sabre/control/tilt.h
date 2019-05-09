#ifndef TILT_H
#define TILT_H

#include <iostream>
#include "../util/util.h"
#include "../util/interface.h"
#include "../util/driver_station_interface.h"
#include "../util/countdown_timer.h"
#include "../util/quick.h"
#include <set>
#include <string>

struct Tilt{
	class Goal{
		public: 
		#define TILT_GOAL_MODES \
			X(DOWN) \
			X(STOP) \
			X(UP) \
			X(GO_TO_ANGLE)
		#define X(name) name,
		enum class Mode{TILT_GOAL_MODES};	
		#undef X
	
		private:
		Mode mode_;
		double angle_min,angle_target,angle_max;//in degrees from top
		
		public:
		Goal();

		Mode mode()const;
		std::array<double,3> angle()const;
		
		static Goal up();
		static Goal down();
		static Goal go_to_angle(std::array<double,3>);
		static Goal stop();
	};
	
	typedef double Output;	
	
	struct Status_detail{
		bool top;
		double angle;
		
		Status_detail();
		Status_detail(bool,double);
	};

	using Status=double;//# of degrees

	using Input=Status_detail;

	struct Input_reader{
		Input operator()(Robot_inputs const&)const;
		Robot_inputs operator()(Robot_inputs,Input)const;
	};
	
	struct Estimator{
		Status_detail last;

		public: 
		Estimator();
		
		void update(Time,Input,Output);
		Status_detail get()const;
	};
	
	struct Output_applicator{
		Output operator()(Robot_outputs)const;
		Robot_outputs operator()(Robot_outputs,Output)const;
	};	
	
	Output_applicator output_applicator;
	Input_reader input_reader;
	Estimator estimator;
};

std::ostream& operator<<(std::ostream&, Tilt::Goal::Mode);
std::ostream& operator<<(std::ostream&, Tilt::Status_detail);
std::ostream& operator<<(std::ostream&, Tilt::Goal); 
std::ostream& operator<<(std::ostream&, Tilt);

bool operator<(Tilt::Status_detail, Tilt::Status_detail);
bool operator<(Tilt::Status_detail,Tilt::Status_detail);
bool operator==(Tilt::Status_detail, Tilt::Status_detail);
bool operator!=(Tilt::Status_detail, Tilt::Status_detail);

bool operator==(Tilt::Goal,Tilt::Goal);
bool operator!=(Tilt::Goal,Tilt::Goal);
bool operator<(Tilt::Goal,Tilt::Goal);

bool operator==(Tilt::Output_applicator,Tilt::Output_applicator);

bool operator==(Tilt::Input_reader,Tilt::Input_reader);

bool operator==(Tilt::Estimator,Tilt::Estimator);
bool operator!=(Tilt::Estimator,Tilt::Estimator);

bool operator==(Tilt,Tilt);
bool operator!=(Tilt,Tilt);

std::set<Tilt::Input> examples(Tilt::Input*);
std::set<Tilt::Goal> examples(Tilt::Goal*);
std::set<Tilt::Status> examples(Tilt::Status*);
std::set<Tilt::Status_detail> examples(Tilt::Status_detail*);

Tilt::Output control(Tilt::Status_detail, Tilt::Goal);
Tilt::Status status(Tilt::Status_detail);
bool ready(Tilt::Status, Tilt::Goal);

float degrees_to_volts(float);

std::array<double,3> make_tolerances(double);

#endif
