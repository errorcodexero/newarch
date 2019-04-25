#ifndef LIGHTS_H
#define LIGHTS_H

#include <iostream>
#include <set>
#include "../util/interface.h"
#include "../util/util.h"
#include "nop.h"

struct Lights{
	enum class Camera_light{ON,OFF};
	struct Goal{//TODO: use macros
		Camera_light camera_light;
	
		//these do not represent goals but statuses of other parts of our robot--done this way due to limitations of our code structure
		bool climbing;
		unsigned lifter_height;
		double drive_left;	
		double drive_right;	
		bool has_cube;
		bool collector_open;
		bool collector_closed;
		bool wings_deployed;
		
		#define LIFTER_STATUSES_FOR_LIGHTS X(UNKNOWN) X(FLOOR) X(EXCHANGE) X(CLIMB) X(SWITCH) X(SCALE)
		enum class Lifter_status{
			#define X(STATUS) STATUS, 
			LIFTER_STATUSES_FOR_LIGHTS
			#undef X
		};

		Lifter_status lifter_status;

		Goal();
		Goal(Camera_light,bool,unsigned,double,double,bool,bool,bool,bool,Lifter_status);
	};
	
	struct Input{
		bool autonomous;
		bool enabled;
		Alliance alliance;
		Input();	
		Input(bool,bool,Alliance);	
	};

	struct Status_detail{
		unsigned lifter_height;//TODO remove
		bool climbing;//TODO remove
		bool autonomous;//communicate mode?
		bool enabled;
		Alliance alliance;
		
		Time now;

		Status_detail();
		Status_detail(unsigned,bool,bool,bool,Alliance,Time);
	};

	using Status = Status_detail;

	struct Input_reader{
		Input operator()(Robot_inputs)const;	
		Robot_inputs operator()(Robot_inputs,Input)const;
	};

	struct Output{ 
		bool camera_light;

		//To send over I2C
		std::vector<uint8_t> blinky_light_info;

		//OI lights
		static const unsigned LIFTER_STATUS_BINARY_LEN = 3;
		std::array<bool,LIFTER_STATUS_BINARY_LEN> lifter_status;
		bool collector_open;
		bool collector_closed;
		bool has_cube;
		bool wings_deployed;
		bool enabled;

		Output();
		Output(bool,std::vector<uint8_t>,std::array<bool,LIFTER_STATUS_BINARY_LEN>,bool,bool,bool,bool,bool);
	};
	
	struct Output_applicator{
		Output operator()(Robot_outputs)const;	
		Robot_outputs operator()(Robot_outputs,Output)const;
	};

	struct Estimator{
		Status_detail last;
		
		void update(Time,Input,Output);
		Status_detail get()const;

		Estimator();
	};

	Input_reader input_reader;
	Estimator estimator;
	Output_applicator output_applicator; 
};

std::ostream& operator<<(std::ostream&,Lights::Camera_light);
std::ostream& operator<<(std::ostream&,Lights::Status_detail);
std::ostream& operator<<(std::ostream&,Lights::Output);
std::ostream& operator<<(std::ostream&,Lights::Goal);
std::ostream& operator<<(std::ostream&,Lights::Input);
std::ostream& operator<<(std::ostream&,Lights);

bool operator==(Lights::Status_detail,Lights::Status_detail);
bool operator!=(Lights::Status_detail,Lights::Status_detail);
bool operator<(Lights::Status_detail,Lights::Status_detail);

bool operator==(Lights::Output,Lights::Output);
bool operator!=(Lights::Output,Lights::Output);
bool operator<(Lights::Output,Lights::Output);

bool operator<(Lights::Goal,Lights::Goal);
bool operator==(Lights::Goal,Lights::Goal);
bool operator!=(Lights::Goal,Lights::Goal);

bool operator<(Lights::Input,Lights::Input);
bool operator==(Lights::Input,Lights::Input);
bool operator!=(Lights::Input,Lights::Input);

bool operator==(Lights::Input_reader,Lights::Input_reader);
bool operator==(Lights::Output_applicator,Lights::Output_applicator);

bool operator==(Lights::Estimator,Lights::Estimator);
bool operator!=(Lights::Estimator,Lights::Estimator);

bool operator==(Lights,Lights);
bool operator!=(Lights,Lights);

std::set<Lights::Input> examples(Lights::Input*);
std::set<Lights::Status_detail> examples(Lights::Status_detail*);
std::set<Lights::Output> examples(Lights::Output*);
std::set<Lights::Goal> examples(Lights::Goal*);

Lights::Output control(Lights::Status_detail, Lights::Goal);
bool ready(Lights::Status, Lights::Goal);
Lights::Status status(Lights::Status_detail const&);
#endif
