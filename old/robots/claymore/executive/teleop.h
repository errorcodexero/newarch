#ifndef TELEOP_H
#define TELEOP_H

#include "executive.h"
#include "message_logger.h"
#include "../util/posedge_trigger_debounce.h"
#include "../util/posedge_toggle.h"
#include "../util/quick.h"
#include <sstream>

struct Teleop : Executive_impl<Teleop> {
    
	enum Nudges{FORWARD,BACKWARD,CLOCKWISE,COUNTERCLOCKWISE,NUDGES};
	#define NUDGE_ITEMS(X) X(Posedge_trigger,trigger) X(Countdown_timer,timer)
	struct Nudge{
		Posedge_trigger trigger;
		Countdown_timer timer;
	};

    #define COLLECTOR_MODES X(IDLE) X(HOLD_CUBE) X(CLAMP_DOWN) X(COLLECT_OPEN) X(COLLECT_CLOSED) X(EJECT) X(DROP) X(STOW) X(CALIBRATE)
	enum class Collector_mode{
		#define X(NAME) NAME,
		COLLECTOR_MODES
		#undef X
	};

#define TELEOP_ITEMS(X)                                    \
	X(SINGLE_ARG(std::array<Nudge,NUDGES>),nudges)	       \
	X(Posedge_trigger, disable_trigger)                    \
	X(Lifter::Goal, lifter_goal)                           \
	X(Wings::Goal, wings_goal)                             \
	X(Collector_mode, collector_mode)                      \
	X(Posedge_trigger, collect_open_trigger)               \
	X(Posedge_trigger, collect_closed_trigger)             \
	X(Countdown_timer, intake_timer)                       \
	X(bool, started_intake_with_cube)                      \
	X(Posedge_trigger, calibrate_lifter_trigger)	       \
	X(Posedge_trigger, calibrate_grabber_trigger)          \
	X(Countdown_timer, cube_timer)                         \
	X(bool, high_gear)				       
	STRUCT_MEMBERS(TELEOP_ITEMS)

	Executive next_mode(Next_mode_info);
	Toplevel::Goal run(Run_info);
	void runDrivebase(const Run_info &info, Toplevel::Goal &goal) ;
	void runCollector(const Run_info &info, Toplevel::Goal &goal) ;
	void runLights(const Run_info &info, Toplevel::Goal &goal) ;
   	bool operator<(Teleop const&)const;
	bool operator==(Teleop const&)const;
	void display(std::ostream&)const;
	Teleop();
	IMPL_STRUCT_DECLARE(Teleop,TELEOP_ITEMS)
};

std::ostream& operator<<(std::ostream&,Teleop::Nudge const&);

bool operator<(Teleop::Nudge const&,Teleop::Nudge const&);
bool operator==(Teleop::Nudge const&,Teleop::Nudge const&);
double set_drive_speed(double,double,double);

inline messageLogger &operator<<(messageLogger &logger, Teleop::Collector_mode mode)
{
	#define COLLECTOR_MODES X(IDLE) X(HOLD_CUBE) X(CLAMP_DOWN) X(COLLECT_OPEN) X(COLLECT_CLOSED) X(EJECT) X(DROP) X(STOW) X(CALIBRATE)
	
	switch(mode)
	{
	case Teleop::Collector_mode::IDLE:
		logger << "IDLE" ;
		break ;
	case Teleop::Collector_mode::HOLD_CUBE:
		logger << "HOLD_CUBE" ;
		break ;
	case Teleop::Collector_mode::CLAMP_DOWN:
		logger << "CLAMP_DOWN" ;
		break;
	case Teleop::Collector_mode::COLLECT_OPEN:
		logger << "COLLECT_OPEN" ;
		break ;
	case Teleop::Collector_mode::COLLECT_CLOSED:
		logger << "COLLECT_CLOSED" ;
		break ;
	case Teleop::Collector_mode::EJECT:
		logger << "EJECT" ;
		break ;
	case Teleop::Collector_mode::DROP:
		logger << "DROP" ;
		break ;
	case Teleop::Collector_mode::CALIBRATE:
		logger << "CALIBRATE" ;
		break;
	case Teleop::Collector_mode::STOW:
		logger << "STOW" ;
		break;
	default:
		assert(false) ;
		break ;
	}

	return logger ;
}
#endif
