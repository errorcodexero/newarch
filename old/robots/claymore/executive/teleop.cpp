#include "teleop.h"
#include <math.h>
#include "autonomous.h"
#include "../input/util.h"

using namespace std;

double set_drive_speed(double axis,double boost,bool slow){
	static const float MAX_SPEED=1;//Change this value to change the max power the robot will achieve with full boost (cannot be larger than 1.0)
	static const float DEFAULT_SPEED=.4;//Change this value to change the default power
	static const float SLOW_BY=.5;//Change this value to change the percentage of the default power the slow button slows
	return (pow(axis,3)*((DEFAULT_SPEED+(MAX_SPEED-DEFAULT_SPEED)*boost)-((DEFAULT_SPEED*SLOW_BY)*slow)));
}

bool operator<(Teleop::Nudge const& a,Teleop::Nudge const& b){
#define X(A,B) if(a.B<b.B) return 1; if(b.B<a.B) return 0;
	NUDGE_ITEMS(X)
#undef X
		return 0;
}

bool operator==(Teleop::Nudge const& a,Teleop::Nudge const& b){
#define X(A,B) if(a.B!=b.B) return 0;
	NUDGE_ITEMS(X)
#undef X
		return 1;
}

ostream& operator<<(ostream& o,Teleop::Nudge const& a){
#define X(t,NAME) o<<""#NAME<<":"<<(a.NAME)<<" ";
	NUDGE_ITEMS(X)
#undef X
		return o;
}

ostream& operator<<(ostream& o,Teleop::Collector_mode const& a){
#define X(NAME) if(a==Teleop::Collector_mode::NAME) return o<<""#NAME;
	COLLECTOR_MODES
#undef X
		assert(0);
}

Executive Teleop::next_mode(Next_mode_info info) {
	if (info.autonomous_start) {
		return Executive{Autonomous()};
	}
	//Teleop t(CONSTRUCT_STRUCT_PARAMS(TELEOP_ITEMS));
	return Executive{*this};
}

IMPL_STRUCT(Teleop::Teleop,TELEOP_ITEMS)

Teleop::Teleop():lifter_goal(Lifter::Goal::stop()),wings_goal(Wings::Goal::LOCKED),collector_mode(Collector_mode::IDLE),started_intake_with_cube(false),high_gear(false)
{
	if(Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
		collector_mode = Collector_mode::HOLD_CUBE;
}

void Teleop::runDrivebase(const Run_info &info, Toplevel::Goal &goals)
{
    bool spin=fabs(info.driver_joystick.axis[Gamepad_axis::RIGHTX])>.01;//drive turning button
    double boost=info.driver_joystick.axis[Gamepad_axis::LTRIGGER]; //Turbo button
    bool slow=info.driver_joystick.button[Gamepad_button::LB]; //Slow button
    bool enabled = info.in.robot_mode.enabled;
    
    POV_section driver_pov=pov_section(info.driver_joystick.pov);
    
    const array<POV_section,NUDGES> nudge_povs={POV_section::UP,POV_section::DOWN,POV_section::RIGHT,POV_section::LEFT};
    //Forward, backward, clockwise, counter-clockwise
    for(unsigned i=0;i<NUDGES;i++){
		if(nudges[i].trigger(boost<.25 && driver_pov==nudge_povs[i])) nudges[i].timer.set(.1);
		nudges[i].timer.update(info.in.now,enabled);
    }
    const double NUDGE_POWER=.2,ROTATE_NUDGE_POWER=.2;
    double left=([&]{
		if(!nudges[Nudges::FORWARD].timer.done()) return NUDGE_POWER;
		if(!nudges[Nudges::BACKWARD].timer.done()) return -NUDGE_POWER;
		if(!nudges[Nudges::CLOCKWISE].timer.done()) return ROTATE_NUDGE_POWER;
		if(!nudges[Nudges::COUNTERCLOCKWISE].timer.done()) return -ROTATE_NUDGE_POWER;
		double power=set_drive_speed(info.driver_joystick.axis[Gamepad_axis::LEFTY],boost,slow);
		if(spin) power+=set_drive_speed(-info.driver_joystick.axis[Gamepad_axis::RIGHTX],boost,slow);
		return -power; //inverted so drivebase values can be positive
	}());
    double right=-clip([&]{ //right side is reversed
		if(!nudges[Nudges::FORWARD].timer.done()) return -NUDGE_POWER;
		if(!nudges[Nudges::BACKWARD].timer.done()) return NUDGE_POWER;
		if(!nudges[Nudges::CLOCKWISE].timer.done()) return ROTATE_NUDGE_POWER;	
		if(!nudges[Nudges::COUNTERCLOCKWISE].timer.done()) return -ROTATE_NUDGE_POWER;
		double power=set_drive_speed(info.driver_joystick.axis[Gamepad_axis::LEFTY],boost,slow);
		if(spin) power-=set_drive_speed(-info.driver_joystick.axis[Gamepad_axis::RIGHTX],boost,slow);
		return power;
	}());
    
    if(info.driver_joystick.button[Gamepad_button::RB]) high_gear = false;
    if(info.driver_joystick.axis[Gamepad_axis::RTRIGGER] > .8) high_gear = true;
    
    Drivebase::Goal::Gear gear_shifter = high_gear ? Drivebase::Goal::Gear::HIGH : Drivebase::Goal::Gear::LOW;
    
    goals.drive = Drivebase::Goal::absolute(left, right, gear_shifter, false);
}

void Teleop::runCollector(const Run_info &info, Toplevel::Goal &goals)
{
    messageLogger &logger = messageLogger::get();

	logger << "HAS CUBE: ";
	logger << (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube);

	if(disable_trigger(!info.in.robot_mode.enabled)) {
		Lifter::lifter_controller.resetCalibration();
		Grabber::grabber_controller.resetCalibration();
	}

    if(info.panel.climb_disabled)
	{
		//
		// If climbing is disabled, the lifter should be in high gear and we should
		// reset the climb base tick amount
		//
		Lifter::lifter_controller.resetClimbBase() ;
		Lifter::lifter_controller.highgear() ;
		
		//
		// This side of the if is for when the climb lock switch has climbing disabled.  This is
		// the normal operation for the majority of the match
		//
		
		//
		// If the grabber tells us we collected a cube, perform teleop cube processing
		//
		if (Grabber::grabber_controller.enterState(GrabberController::CubeState::HasCube))
		{
			logger << "    Performed acquire cube actions\n" ;
			logger << "    Is Calibrated: " << Lifter::lifter_controller.isCalibrated() << "\n";

			LifterController::Preset end_height_preset;
			string preset_str;
			switch(info.panel.collection_end_height) {
			case Panel::Collection_end_height::EXCHANGE:
				end_height_preset = LifterController::Preset::EXCHANGE;
				preset_str = "exchange";
				break;
			case Panel::Collection_end_height::SWITCH:
				end_height_preset = LifterController::Preset::SWITCH;
				preset_str = "switch";
				break;
			case Panel::Collection_end_height::SCALE:
				end_height_preset = LifterController::Preset::SCALE;
				preset_str = "scale";
				break;
			default:
				assert(0);
			}

			double exheight = Lifter::lifter_controller.presetToHeight(end_height_preset) ;
			if (Lifter::lifter_controller.getHeight() < exheight && Lifter::lifter_controller.isCalibrated())
			{
				lifter_goal = Lifter::Goal::go_to_preset(end_height_preset);
				logger << "    Set lifter goal to " << preset_str << " height\n";
			}	
		}

		if(Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::GraspCube) {
			collector_mode = Collector_mode::CLAMP_DOWN;
		}

		//
		// Now process the panel inputs and decide what to do.  Note, these
		// actions may be overridden if we are in manual mode by code below.
		//
		if(collect_open_trigger(info.panel.collect_open))
		{
			if(collector_mode == Collector_mode::COLLECT_OPEN)
			{
				collector_mode = Collector_mode::IDLE;
				logger << "    Collector to IDLE mode\n" ;
			}
			else
			{
				collector_mode = Collector_mode::COLLECT_OPEN;
				logger << "    Collector to OPEN mode\n" ;
			}
		}
		else if(collect_closed_trigger(info.panel.collect_closed))
		{
			if(collector_mode == Collector_mode::COLLECT_CLOSED)
			{
				collector_mode = Collector_mode::IDLE;
				logger << "    Collector to IDLE mode\n" ;
			}
			else
			{
				collector_mode = Collector_mode::COLLECT_CLOSED;
				logger << "    Collector to CLOSED mode\n" ;
			}
		}
		else if(info.panel.eject)
		{
			logger << "    Collector to EJECT\n" ;
			collector_mode = Collector_mode::EJECT;
			started_intake_with_cube = (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube) ;
			intake_timer.set(0.1);
		}
		else if(info.panel.drop)
		{
			logger << "    Collector to DROP\n" ;
			collector_mode = Collector_mode::DROP;
			started_intake_with_cube = (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube) ;
			intake_timer.set(0.5);
		}

		if (info.panel.floor)
		{
			lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::FLOOR);
		}
		else if(info.panel.exchange)
		{
			lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::EXCHANGE);
		}
		else if(info.panel.switch_)
		{
			lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SWITCH);
		}
		else if(info.panel.scale)
		{
			lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::SCALE);
		}
		else if(info.panel.climb)
		{
			//
			// This preset height is to go to the climbing height.  Therefore when we go to this
			// height we also stow the grabber out of the way to ready for the alignment and
			// climb.
			//
			lifter_goal = Lifter::Goal::go_to_preset(LifterController::Preset::PREP_CLIMB);
			collector_mode = Collector_mode::STOW ;
		}
		else if(info.panel.lifter == Panel::Lifter::UP)
		{
			lifter_goal = Lifter::Goal::up(info.panel.lifter_high_power);
		}
		else if(info.panel.lifter == Panel::Lifter::DOWN)
		{
			lifter_goal = Lifter::Goal::down(info.panel.lifter_high_power);
		}
		else if(ready(status(info.status.lifter), lifter_goal))
		{
			lifter_goal = Lifter::Goal::stop() ;
		}
		
		if(calibrate_lifter_trigger(info.panel.calibrate_lifter)) {
			logger << "    Lifter calibration requested from panel\n" ;
			lifter_goal = Lifter::Goal::calibrate();
		}
		
		if(calibrate_grabber_trigger(info.panel.calibrate_grabber)) {
			logger << "    Grabber calibration requested from panel\n" ;
			Grabber::grabber_controller.calibrate() ;
			collector_mode = Collector_mode::CALIBRATE;
		} 
	}
	else
	{
		//
		// Switching the climb lock to unlock climbing does not do anything that
		// cannot be reversed.  However, once you manually adjust the lift or press
		// the climb button with climbing unlocked, you are in the low gear state
		// until the end of the match (or until you recalibrate).
		//
		
		//
		// This side of the if is for when the climb lock has climbing enabled.  The
		// behavior of the robot and the panel change when climbing is enabled at the
		// endgame.
		//
		if (info.panel.climb)
		{
			//
			// What conditions apply here?  If we are still calibrated we can
			// check the lifter height
			//

			//
			// Note, this goal will shift the lifter to low gear and reset the
			// calibration
			//
			lifter_goal = Lifter::Goal::climb() ;
		}
		else if (info.panel.lifter == Panel::Lifter::UP)
		{
			Lifter::lifter_controller.lowgear() ;
			lifter_goal = Lifter::Goal::up(info.panel.lifter_high_power);
		}
		else if (info.panel.lifter == Panel::Lifter::DOWN)
		{
			Lifter::lifter_controller.lowgear() ;
			lifter_goal = Lifter::Goal::down(info.panel.lifter_high_power);
		}

		if (lifter_goal == Lifter::Goal::climb() && Lifter::lifter_controller.isClimbed()) {
			lifter_goal = Lifter::Goal::maintain_climb();
		}

		if ((lifter_goal == Lifter::Goal::up(true) || lifter_goal == Lifter::Goal::up(false)
		    || lifter_goal == Lifter::Goal::down(true) || lifter_goal == Lifter::Goal::down(false))
		   && info.panel.lifter == Panel::Lifter::OFF) {
			Lifter::lifter_controller.lowgear() ;
			lifter_goal = Lifter::Goal::stop() ;
		}
	}

    switch(collector_mode) {
    case Collector_mode::IDLE:
		goals.grabber = Grabber::Goal::idle();
		goals.intake = Intake::Goal::off();
		break;
    case Collector_mode::HOLD_CUBE:
		goals.grabber = Grabber::Goal::hold();
		goals.intake = Intake::Goal::off();
		break;
	case Collector_mode::CLAMP_DOWN:
		goals.grabber = Grabber::Goal::clamp();
		goals.intake = Intake::Goal::off();
		if(Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
			collector_mode = Collector_mode::HOLD_CUBE;
		break;
    case Collector_mode::COLLECT_OPEN:
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
		goals.intake = Intake::Goal::in();
		if (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
			collector_mode = Collector_mode::HOLD_CUBE;
		break;
    case Collector_mode::COLLECT_CLOSED:
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
		goals.intake = Intake::Goal::in();
		if (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
			collector_mode = Collector_mode::HOLD_CUBE;
		break;
    case Collector_mode::EJECT:
    case Collector_mode::DROP:
		if(collector_mode == Collector_mode::EJECT) {
			goals.grabber = Grabber::Goal::hold();
			goals.intake = Intake::Goal::out();
		} else {
			goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
			goals.intake = Intake::Goal::out(0.2);
		}
		intake_timer.update(info.in.now, info.in.robot_mode.enabled);
		if ((started_intake_with_cube && Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::NoCube) || intake_timer.done())
			collector_mode = Collector_mode::IDLE;
		break;
	case Collector_mode::STOW:
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::STOWED);
		goals.intake = Intake::Goal::off();
		break;
    case Collector_mode::CALIBRATE:
		goals.grabber = Grabber::Goal::calibrate();
		if(ready(info.status.grabber, Grabber::Goal::calibrate()))
			collector_mode = Collector_mode::HOLD_CUBE;
		break;
    default:
		assert(0);
    }

    if(!info.panel.grabber_auto) {
		//
		// These override settings made by the collector mode
		//
		if (info.panel.grabber == Panel::Grabber::OFF)
		{
			goals.grabber = Grabber::Goal::idle();
		}
		else if (info.panel.grabber == Panel::Grabber::OPEN)
		{
			goals.grabber = Grabber::Goal::open();
		}
		else if (info.panel.grabber == Panel::Grabber::CLOSE)
		{
			goals.grabber = Grabber::Goal::close();
		}
    }
    
    if(!info.panel.intake_auto) {
		//
		// These override settings made by the collector mode
		//
		if(info.panel.intake == Panel::Intake::OFF)
			goals.intake = Intake::Goal::off();
		else if(info.panel.intake == Panel::Intake::IN)
			goals.intake = Intake::Goal::in();
		else if(info.panel.intake == Panel::Intake::OUT)
			goals.intake = Intake::Goal::out();
    }

	goals.lifter = lifter_goal ;
    
    logger << "    Grabber goal: " << goals.grabber << "\n";
	logger << "    Lifter goal: " << goals.lifter.toString() << "\n" ;
    logger << "    Collector mode: " << collector_mode << "\n";
}

void Teleop::runLights(const Run_info &info, Toplevel::Goal &goals)
{
    goals.lights.climbing = goals.lifter.preset_target() == LifterController::Preset::PREP_CLIMB;
    goals.lights.lifter_height = (unsigned)Lifter::lifter_controller.getHeight() ;
    goals.lights.drive_left = goals.drive.left();
    goals.lights.drive_right = goals.drive.right();
    goals.lights.has_cube = (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::GraspCube) ;

    goals.lights.collector_open = collector_mode == Collector_mode::COLLECT_OPEN;
    goals.lights.collector_closed = collector_mode == Collector_mode::COLLECT_CLOSED;
    goals.lights.wings_deployed = goals.wings == Wings::Goal::UNLOCKED;
    goals.lights.lifter_status = [&]{
		Lifter::Status s = status(info.status.lifter);
		if(!ready(s,goals.lifter)){
			return Lights::Goal::Lifter_status::UNKNOWN;
		}
		switch(goals.lifter.preset_target()){
		case LifterController::Preset::FLOOR:
	    return Lights::Goal::Lifter_status::FLOOR;
		case LifterController::Preset::EXCHANGE:
		return Lights::Goal::Lifter_status::EXCHANGE;
		case LifterController::Preset::PREP_CLIMB:
	    return Lights::Goal::Lifter_status::CLIMB;
		case LifterController::Preset::SWITCH:
	    return Lights::Goal::Lifter_status::SWITCH;
		case LifterController::Preset::SCALE:
	    return Lights::Goal::Lifter_status::SCALE;
		default:
	    nyi
		}
    }();
}

Toplevel::Goal Teleop::run(Run_info info)
{
    messageLogger &logger = messageLogger::get();
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_TELEOP);
    logger << "Teleop:\n";

	if (info.panel.climb_disabled)
		logger << "    climb lock is set, cannot climb\n" ;
	else
		logger << "    climb lock is clear, can climb\n" ;
    
    Toplevel::Goal goals;
    
    runDrivebase(info, goals) ;
    runCollector(info, goals) ;

	if (info.panel.wings && !info.panel.climb_disabled)
	{
		logger << "    Unlocking wings\n";
		goals.wings = Wings::Goal::UNLOCKED;
    }

    runLights(info, goals) ;

    logger.endMessage() ;

    return goals;
}

bool Teleop::operator<(Teleop const& a)const{
#define X(t,NAME) if(NAME<a.NAME) return 1; if(a.NAME<NAME) return 0;
	TELEOP_ITEMS(X)
#undef X
		return 0;
}

bool Teleop::operator==(Teleop const& a)const{
#define X(t,NAME) if(NAME!=a.NAME) return 0;
	TELEOP_ITEMS(X)
#undef X
		return 1;
}

void Teleop::display(ostream& o)const{
	o<<"Teleop( ";
#define X(t,NAME) o<<""#NAME<<":"<<(NAME)<<" ";
	TELEOP_ITEMS(X)
#undef X
		o<<")";
}

#ifdef TELEOP_TEST
#include "test.h"

int main() {
	Teleop a;
	test_executive(a);
}
#endif
