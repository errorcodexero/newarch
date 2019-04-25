#include "step.h"
#include "executive.h"
#include "util.h"
#include "message_logger.h"
#include "params_parser.h"
#include "../subsystems.h"
#include <queue>
#include <cmath>


using namespace std;

static double autoStartTime ;

//this part should eventually go into util
template<typename T>
queue<T>& operator|=(queue<T> &q,T t){
    q.push(t);
    return q;
}

ostream& operator<<(ostream& o,Step const& a){
    a.display(o);
    return o;
}

Step::Step(Step const& a):impl(a.get().clone()),fail_branch(nullptr){}
Step::Step(Step const& a, vector<Step> fail):Step(a){
	fail_branch = &fail;
}

Step::Step(Step_impl const& a){
	fail_branch = nullptr;
	auto c=a.clone();
    if(!c)nyi
		impl=move(c);
}

Step::Step(Step_impl const& a, vector<Step> fail):Step(a){
	fail_branch = &fail;
}

vector<Step>* Step::get_fail_branch(){
	return fail_branch;
}

Toplevel::Goal Step::run(Run_info info, Toplevel::Goal goals){
    return impl->run(info,goals);
}
Toplevel::Goal Step::run(Run_info info){
    return impl->run(info,{});
}

Step::Status Step::done(Next_mode_info a){
    return impl->done(a);
}

void Step::display(ostream& o)const{
    impl->display(o);
}

bool Step::operator==(Step const& a)const{
    assert(impl);
    return impl->operator==(a);
}

bool Step::operator<(Step const& a)const{
    assert(impl);
    return impl->operator<(a);
}


void Step_impl::display(ostream& o)const{
    o<<"Step_impl";
}

Step_impl const& Step::get()const{
    assert(impl);
    return *impl.get();
}

ostream& operator<<(ostream& o,Step_impl const& a){
    a.display(o);
    return o;
}

/*bool Step_impl::operator==(Step_impl const& a)const{
  T const& b=dynamic_cast<T const&>(a.get());
  return this->operator==(b);
  }*/

Step_impl::~Step_impl(){}


//
// Combo: Run two steps simultaneously
//

void Combo::display(std::ostream& o)const{
    Step_impl_inner<Combo>::display(o);
    o<<"(";
    step_a.display(o);
    o<<" ";
    step_b.display(o);
    o<<")";
}

Combo::Combo(Step a,Step b):step_a(a),step_b(b){}

Step::Status Combo::done(Next_mode_info info){
    Step::Status a_status = step_a.done(info);
    Step::Status b_status = step_b.done(info);
    switch(a_status){
    case Step::Status::FINISHED_SUCCESS:
		return b_status;
    case Step::Status::UNFINISHED:
		return a_status;//TODO
    case Step::Status::FINISHED_FAILURE:
		nyi //TODO
    default:
		assert(0);
    } 
}

Toplevel::Goal Combo::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Combo::run(Run_info info,Toplevel::Goal goals){
    goals = step_a.run(info,goals);
    goals = step_b.run(info,goals);
    return goals;
}

unique_ptr<Step_impl> Combo::clone()const{
    return unique_ptr<Step_impl>(new Combo(*this));
}

bool Combo::operator==(Combo const& b)const{
    return step_a == b.step_a && step_b == b.step_b;
}

//
// Wait: Wait for a specified amount of time
//

Wait::Wait(Time wait_time){
    wait_timer.set(wait_time);
}

Step::Status Wait::done(Next_mode_info){
    Step::Status ret =  wait_timer.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Wait step complete" ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Wait::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Wait::run(Run_info info,Toplevel::Goal goals){
    wait_timer.update(info.in.now,info.in.robot_mode.enabled);
    return goals;
}

unique_ptr<Step_impl> Wait::clone()const{
    return unique_ptr<Step_impl>(new Wait(*this));
}

bool Wait::operator==(Wait const& b)const{
    return wait_timer == b.wait_timer;
}

//
// Start auto, mark the start of the auto program
//
StartAuto::StartAuto(const char *name_p)
{
	mProgramName = name_p ;
	mInited = false ;
}

Step::Status StartAuto::done(Next_mode_info info)
{
    autoStartTime = info.in.now ;
    return Step::Status::FINISHED_SUCCESS ;
}

Toplevel::Goal StartAuto::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal StartAuto::run(Run_info info,Toplevel::Goal goals)
{
	if (!mInited)
	{
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Starting autonomous program " << mProgramName ;
		logger.endMessage() ;
		mInited = true ;
	}
    return goals;
}

unique_ptr<Step_impl> StartAuto::clone()const{
    return unique_ptr<Step_impl>(new StartAuto(*this));
}

bool StartAuto::operator==(StartAuto const& b)const{
    return true ;
}
//
// Wait: Wait for a specified amount of time
//

EndAuto::EndAuto(){
}

Step::Status EndAuto::done(Next_mode_info info){
    messageLogger &logger = messageLogger::get() ;
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
    logger << "Automode program completed in " << info.in.now - autoStartTime << " seconds" ;
    logger.endMessage() ;
    
    return Step::Status::FINISHED_SUCCESS ;
}

Toplevel::Goal EndAuto::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal EndAuto::run(Run_info info,Toplevel::Goal goals){
    return goals;
}

unique_ptr<Step_impl> EndAuto::clone()const{
    return unique_ptr<Step_impl>(new EndAuto(*this));
}

bool EndAuto::operator==(EndAuto const& b)const{
    return true;
}


//
// Drive: Drive straight a specified distance
//

Drive::Drive(Inch dist, bool end_on_stall)
{
	mTargetDistance = dist ;
	mEndOnStall = end_on_stall ;
	mCurve = false;
	mReturnFromCollect = false;
	mInited = false ;
}

Drive::Drive(Inch dist, double curve_start, double angle_offset, bool end_on_stall)
{
	mTargetDistance = dist ;
	mTargetAngleOffset = angle_offset;
	mCurve = true;
	mCurveStart = curve_start ;
	mEndOnStall = end_on_stall;
	mInited = false;
	mReturnFromCollect = false;
}

Drive::Drive(const char *param_p, Inch dist, bool end_on_stall)
{
	mParamName = param_p ;
	mTargetDistance = dist ;
	mEndOnStall = end_on_stall ;
	mCurve = false;
	mReturnFromCollect = false;
	mInited = false ;
}

Drive::Drive(const std::string &param, Inch dist, bool end_on_stall)
{
	mParamName = param ;
	mTargetDistance = dist ;
	mEndOnStall = end_on_stall ;
	mCurve = false;
	mReturnFromCollect = false;
	mInited = false ;
}

Drive::Drive(bool, Inch return_offset)
{
	mReturnOffset = return_offset;
	mCurve = false;
	mReturnFromCollect = true;
	mInited = false;
}

Step::Status Drive::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::drive_straight()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Drive step complete" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Drive::run(Run_info info)
{
    return run(info, {});
}

Toplevel::Goal Drive::run(Run_info info, Toplevel::Goal goals)
{
    if(!mInited)
	{
		if (mParamName.length() > 0)
		{
			//
			// Based on a parameter name, go get the value from the
			// parameter file
			//
			paramsInput *params_p = paramsInput::get() ;
			mTargetDistance = params_p->getValue(mParamName, mTargetDistance) ;
		} else if (mReturnFromCollect) {
			mTargetDistance = -(Drive_and_collect::distance_travelled + mReturnOffset);
		}
		
		double avg_status = (info.status.drive.distances.l + info.status.drive.distances.r) / 2.0;
		if(!mCurve) {
			Drivebase::drivebase_controller.initDistance(avg_status + mTargetDistance, info.status.drive.angle,
													 info.in.now, mEndOnStall, mTargetDistance >= 0.0);
		} else {
			Drivebase::drivebase_controller.initCurve(avg_status, avg_status + mTargetDistance, mCurveStart, info.status.drive.angle,
													  mTargetAngleOffset, info.in.now, mEndOnStall, mTargetDistance >= 0.0);
		}
		mInited = true ;
    }
    goals.drive = Drivebase::Goal::drive_straight();
    return goals;
}

unique_ptr<Step_impl> Drive::clone()const{
    return unique_ptr<Step_impl>(new Drive(*this));
}

bool Drive::operator==(Drive const& a)const{
    return mTargetDistance == a.mTargetDistance && mEndOnStall == a.mEndOnStall && mInited == a.mInited ;
}

//
// Drive_timed: Drive the motors at the specified powers for a specified amount of time
//
Drive_timed::Drive_timed(double l, double r, double t){
    left_power = l;
    right_power = r;
    timer.set(t);
}

Step::Status Drive_timed::done(Next_mode_info /*info*/){
    Step::Status ret =  timer.done() ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Timed Drive step complete" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Drive_timed::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Drive_timed::run(Run_info info,Toplevel::Goal goals){
    timer.update(info.in.now, info.in.robot_mode.enabled);
    goals.drive = Drivebase::Goal::absolute(left_power, right_power);
    return goals;
}

unique_ptr<Step_impl> Drive_timed::clone()const{
    return unique_ptr<Step_impl>(new Drive_timed(*this));
}

bool Drive_timed::operator==(Drive_timed const& b)const{
    return left_power == b.left_power && right_power == b.right_power && timer == b.timer;
}

//
// Ram: Drive the motors at the specified powers for the specified distances
//

Ram::Ram(double l_power, double r_power, Inch l_target, Inch r_target){
    left_power = l_power;
    right_power = r_power;
    target_distances = Drivebase::Distances(l_target, r_target);
    initial_distances = Drivebase::Distances(0.0, 0.0);
    init = false;
}

Step::Status Ram::done(Next_mode_info info){
    bool left_reached = (target_distances.l > initial_distances.l) ? (target_distances.l < info.status.drive.distances.l) : (target_distances.l > info.status.drive.distances.l);
    bool right_reached = (target_distances.r > initial_distances.r) ? (target_distances.r < info.status.drive.distances.r) : (target_distances.r > info.status.drive.distances.r);
    return (init && left_reached && right_reached) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
}

Toplevel::Goal Ram::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Ram::run(Run_info info,Toplevel::Goal goals){
    if(!init){
		initial_distances = info.status.drive.distances;
		target_distances = target_distances + initial_distances;
		init = true;
    }

    goals.drive = Drivebase::Goal::absolute(left_power, right_power);
    return goals;
}

unique_ptr<Step_impl> Ram::clone()const{
    return unique_ptr<Step_impl>(new Ram(*this));
}

bool Ram::operator==(Ram const& b)const{
    return left_power == b.left_power && right_power == b.right_power && target_distances == b.target_distances && initial_distances == b.initial_distances && init == b.init;
}

//
// Rotate: Rotate the robot by a specified angle
//
Rotate::Rotate(double a, bool open_grabber)
{
	target_angle = a ;
	opengrabber = open_grabber;
	init = false ;
	tolprovided = false ;
}

Rotate::Rotate(double a, double tol, bool open_grabber)
{
	target_angle = a ;
	opengrabber = open_grabber;
	init = false ;
	tolprovided = true ;
	tolerance = tol ;
}

Toplevel::Goal Rotate::run(Run_info info)
{
    return run(info,{});
}

double lastrotate ;
Toplevel::Goal Rotate::run(Run_info info,Toplevel::Goal goals)
{
    if(!init) {
		if (tolprovided)
		{
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target_angle, info.in.now, target_angle > 0, tolerance) ;
		}
		else
		{
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target_angle, info.in.now, target_angle > 0) ;
		}
		lastrotate = info.status.drive.angle ;
		init = true;
    }

    goals.drive = Drivebase::Goal::rotate();
	if(opengrabber)
		goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
    return goals;
}

Step::Status Rotate::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::rotate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Rotate step complete" ;
		logger.endMessage() ;
    }

    return ret ;
}

std::unique_ptr<Step_impl> Rotate::clone()const
{
    return unique_ptr<Step_impl>(new Rotate(*this));
}

bool Rotate::operator==(Rotate const& b)const
{
    return target_angle == b.target_angle && init == b.init;
}

//////////////////////////////////////////////////////////////////////////
//
// Rotate: Rotate the robot by a specified angle
//
Rotate_finish::Rotate_finish(double prev, double a)
{
	prev_angle = prev ;
	target_angle = a ;
	init = false ;
	tolprovided = false ;
}

Rotate_finish::Rotate_finish(double prev, double a, double tol)
{
	prev_angle = prev ;
	target_angle = a ;
	tolerance = tol ;
	init = false ;
	tolprovided = true ;
}

Toplevel::Goal Rotate_finish::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Rotate_finish::run(Run_info info,Toplevel::Goal goals)
{
    if(!init) {
		// double last = Drivebase::drivebase_controller.getLastAngle() ;
		// double target = prev_angle - last + info.status.drive.angle + target_angle ;
		double target = lastrotate + target_angle + prev_angle ;
		
		if (tolprovided)
			Drivebase::drivebase_controller.initAngle(target, info.in.now, target_angle > 0, tolerance) ;
		else
			Drivebase::drivebase_controller.initAngle(target, info.in.now, target_angle > 0) ;
		init = true;
    }

    goals.drive = Drivebase::Goal::rotate();
    return goals;
}

Step::Status Rotate_finish::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::rotate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Rotate_finish step complete" ;
		logger.endMessage() ;
    }

    return ret ;
}

std::unique_ptr<Step_impl> Rotate_finish::clone()const
{
    return unique_ptr<Step_impl>(new Rotate_finish(*this));
}

bool Rotate_finish::operator==(Rotate_finish const& b)const
{
    return target_angle == b.target_angle && init == b.init;
}


//////////////////////////////////////////////////////////////////////////

Rotate_back::Rotate_back()
{
	init = false ;
	mOffset = 0 ;
	mTolSpecified = false ;
}

Rotate_back::Rotate_back(double offset)
{
	init = false ;
	mOffset = offset ;
	mTolSpecified = false ;
}

Rotate_back::Rotate_back(double offset, double tolval)
{
	init = false ;
	mOffset = offset ;
	mTolSpecified = true ;
	mTolerance = tolval ;
}

Toplevel::Goal Rotate_back::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Rotate_back::run(Run_info info,Toplevel::Goal goals)
{
    if(!init) {
		double target = -Drivebase::drivebase_controller.getLastAngle() ;
		if (mTolSpecified)
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target + mOffset, info.in.now, target > 0, mTolerance) ;
		else
			Drivebase::drivebase_controller.initAngle(info.status.drive.angle + target + mOffset, info.in.now, target > 0) ;
		init = true;
    }

    goals.drive = Drivebase::Goal::rotate();
    return goals;
}

Step::Status Rotate_back::done(Next_mode_info info)
{
    Step::Status ret =  ready(info.status.drive, Drivebase::Goal::rotate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;	
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Rotate_back step complete" ;
		logger.endMessage() ;
    }

    return ret ;
}

std::unique_ptr<Step_impl> Rotate_back::clone()const
{
    return unique_ptr<Step_impl>(new Rotate_back(*this));
}

bool Rotate_back::operator==(Rotate_back const& b)const
{
    return true ;
}

//////////////////////////////////////////////////////////////////////////

//
// Start_lifter_in_background: Start moving the lifter to a specified preset in the background
//

Background_lifter_to_preset::Background_lifter_to_preset(LifterController::Preset preset, double time):preset(preset),time(time),init(false)
{
	delay = 0.0 ;
	heightgiven = false ;
}

Background_lifter_to_preset::Background_lifter_to_preset(double h, double time):time(time),init(false)
{
	delay = 0.0 ;
	heightgiven = true ;
	height = h ;
}

Background_lifter_to_preset::Background_lifter_to_preset(LifterController::Preset preset, double del, double time):preset(preset),time(time),init(false)
{
	delay = del ;
	heightgiven = false ;
}

Step::Status Background_lifter_to_preset::done(Next_mode_info)
{
    messageLogger &logger = messageLogger::get() ;
    logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
    logger << "Background_lifter_to_preset step complete" ;
    logger.endMessage() ;
    return Step::Status::FINISHED_SUCCESS;
}

Toplevel::Goal Background_lifter_to_preset::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Background_lifter_to_preset::run(Run_info info,Toplevel::Goal goals)
{
    if(!init)
	{
		if (heightgiven)
			Lifter::lifter_controller.moveToHeight(height, info.in.now, true);
		else
			Lifter::lifter_controller.moveToHeight(preset, info.in.now, true);
		
		init = false;
    }
    return goals;
}

unique_ptr<Step_impl> Background_lifter_to_preset::clone()const
{
    return unique_ptr<Step_impl>(new Background_lifter_to_preset(*this));
}

bool Background_lifter_to_preset::operator==(Background_lifter_to_preset const& b)const
{
    return preset == b.preset && time == b.time && init == b.init;
}

//
// Calibrate_lifter: Calibrate the lifter at the current height
//

Calibrate_lifter::Calibrate_lifter()
{
	mInited = false ;
}

Step::Status Calibrate_lifter::done(Next_mode_info info)
{
    Step::Status ret = ready(status(info.status.lifter), Lifter::Goal::calibrate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Calibrate Lifter Step done" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Calibrate_lifter::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Calibrate_lifter::run(Run_info info,Toplevel::Goal goals)
{
    goals.lifter = Lifter::Goal::calibrate();
    return goals;
}

unique_ptr<Step_impl> Calibrate_lifter::clone()const
{
    return unique_ptr<Step_impl>(new Calibrate_lifter(*this));
}

bool Calibrate_lifter::operator==(Calibrate_lifter const& b)const
{
    return true;
}

//
// Lifter_to_height: Move the lifter to a specified height
//
Lifter_to_height::Lifter_to_height(double target_height)
{
	mTargetHeight = target_height ;
	mInited = false ;
}

Lifter_to_height::Lifter_to_height(const char *param_p, double height)
{
	mParamName = param_p ;
	mTargetHeight = height ;
	mInited = false ;
}

Lifter_to_height::Lifter_to_height(const std::string &param, double height)
{
	mParamName = param ;
	mTargetHeight = height ;
	mInited = false ;
}

Step::Status Lifter_to_height::done(Next_mode_info info)
{
    Step::Status ret =  ready(status(info.status.lifter), Lifter::Goal::go_to_height(mTargetHeight)) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Lifter_to_height step complete" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Lifter_to_height::run(Run_info info)
{
    return run(info,{});
}

Toplevel::Goal Lifter_to_height::run(Run_info info,Toplevel::Goal goals)
{
    if(!mInited) {
		if (mParamName.length() > 0)
		{
			paramsInput *params_p = paramsInput::get() ;
			mTargetHeight = params_p->getValue(mParamName, mTargetHeight) ;
		}
		
		Lifter::lifter_controller.moveToHeight(mTargetHeight, info.in.now) ;
		mInited = false ;
    }
    goals.lifter = Lifter::Goal::go_to_height(mTargetHeight);
    if(info.status.grabber.has_cube)
        goals.grabber = Grabber::Goal::hold();
    return goals;
}

unique_ptr<Step_impl> Lifter_to_height::clone()const{
    return unique_ptr<Step_impl>(new Lifter_to_height(*this));
}

bool Lifter_to_height::operator==(Lifter_to_height const& b)const
{
    return mTargetHeight == b.mTargetHeight && mInited == b.mInited ;
}

//
// Lifter_to_preset: Move the lifter to a specified preset
//

Lifter_to_preset::Lifter_to_preset(LifterController::Preset preset, double time)
{
    mPreset = preset ;
    mTime = time ;
    mInit = false ;
}

Step::Status Lifter_to_preset::done(Next_mode_info info){
    Step::Status ret = ready(status(info.status.lifter), Lifter::Goal::go_to_preset(mPreset)) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;

	fail_timer.update(info.in.now, info.in.robot_mode.enabled);
	if (fail_timer.done())
		ret = Step::Status::FINISHED_FAILURE;

    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Lifter_to_preset step complete" ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Lifter_to_preset::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Lifter_to_preset::run(Run_info info,Toplevel::Goal goals){
    if(!mInit) {
		Lifter::lifter_controller.moveToHeight(mPreset, info.in.now);
		fail_timer.set(5.0);
		mInit = false;
    }
    goals.lifter = Lifter::Goal::go_to_preset(mPreset);
    if(info.status.grabber.has_cube)
        goals.grabber = Grabber::Goal::hold();
    return goals;
}

unique_ptr<Step_impl> Lifter_to_preset::clone()const{
    return unique_ptr<Step_impl>(new Lifter_to_preset(*this));
}

bool Lifter_to_preset::operator==(Lifter_to_preset const& b)const{
    return mPreset == b.mPreset ;
}

//
// Calibrate_grabber: Calibrate the grabber at the current angle
//

Calibrate_grabber::Calibrate_grabber()
{
    mInited = false ;
}

Step::Status Calibrate_grabber::done(Next_mode_info info){
    Step::Status ret =  ready(status(info.status.grabber), Grabber::Goal::calibrate()) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
	messageLogger &logger = messageLogger::get() ;
	logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
	logger << "Calibrate grabber step done" ;
	logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Calibrate_grabber::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Calibrate_grabber::run(Run_info info,Toplevel::Goal goals){
    if (!mInited)
    {
	Grabber::grabber_controller.calibrate() ;
	mInited = true ;
    }
    goals.grabber = Grabber::Goal::calibrate();
    return goals;
}

unique_ptr<Step_impl> Calibrate_grabber::clone()const{
    return unique_ptr<Step_impl>(new Calibrate_grabber(*this));
}

bool Calibrate_grabber::operator==(Calibrate_grabber const& b)const{
    return true;
}

//
// Grabber_to_preset: Move the grabber to a specified angle preset
//

Grabber_to_preset::Grabber_to_preset(GrabberController::Preset target_preset, double time):target_preset(target_preset),time(time),init(false){}

Step::Status Grabber_to_preset::done(Next_mode_info info){
    Step::Status ret =  ready(status(info.status.grabber), Grabber::Goal::go_to_preset(target_preset)) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Grabber To Preset complete" ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Grabber_to_preset::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Grabber_to_preset::run(Run_info info,Toplevel::Goal goals){
    if(!init) {
		Grabber::grabber_controller.moveToAngle(target_preset, time);
		init = false;
    }
    goals.grabber = Grabber::Goal::go_to_preset(target_preset);
    return goals;
}

unique_ptr<Step_impl> Grabber_to_preset::clone()const{
    return unique_ptr<Step_impl>(new Grabber_to_preset(*this));
}

bool Grabber_to_preset::operator==(Grabber_to_preset const& b)const{
    return target_preset == b.target_preset && time == b.time && init == b.init;
}

//
// Collect: Put the grabber into collecting mode until a cube is collected
//

Collect::Collect(double time):time(time),init(false){}

Step::Status Collect::done(Next_mode_info info){
    Step::Status ret = info.status.grabber.has_cube ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Collect complete" ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Collect::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Collect::run(Run_info info,Toplevel::Goal goals){
    if(!init) {
		Grabber::grabber_controller.moveToAngle(GrabberController::Preset::OPEN, time);
		init = false;
    }
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
	return goals;
}

unique_ptr<Step_impl> Collect::clone()const{
    return unique_ptr<Step_impl>(new Collect(*this));
}

bool Collect::operator==(Collect const& b)const{
    return time == b.time && init == b.init;
}

//
// Eject: Eject a cube
//

Eject::Eject()
{
	mState = EjectState::Start ;
	mPowerApplied = false ;
}

Eject::Eject(double power)
{
	mState = EjectState::Start ;
	mPower = power ;
	mPowerApplied = true ;
}

Step::Status Eject::done(Next_mode_info info)
{
    Step::Status ret = Step::Status::UNFINISHED ;
	if (mState == EjectState::Done)
		ret = Step::Status::FINISHED_SUCCESS ;
		
    if (ret == Step::Status::FINISHED_SUCCESS)
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Eject step complete, elapsed " << info.in.now - mStart ;
		logger.endMessage() ;
    }
    return ret ;
}

Toplevel::Goal Eject::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Eject::run(Run_info info,Toplevel::Goal goals)
{
	switch(mState)
	{
	case EjectState::Start:
		mStart = info.in.now ;
		if (info.status.grabber.has_cube)
		{
			//
			// The cube sensor is on, wait for it to go off
			// We also set a timer just in case the sensor never
			// goes off
			//
			mState = EjectState::WaitingCubeSensorOff ;
			eject_timer.set(1.0) ;
		}
		else
		{
			//
			// The cube sensor is off, it may not be working as
			// we are trying to eject.  Just assume a fixed time
			// for the eject operation
			//
			mState = EjectState::WaitingOnTime ;
			eject_timer.set(1.0) ;
		}
		break ;

	case EjectState::WaitingCubeSensorOff:
		//
		// We get here if the cube sensor was on when we started
		// an eject operation.  There are two ways to leave this state.
		// First, if the cube sensor goes off, or if the fail safe time
		// we started expires
		//
		if (!info.status.grabber.has_cube)
		{
			//
			// The cube has disappeared.  Start a timer to leave the intake
			// in eject mode for a little longer to be sure the cube is fully
			// ejected
			//
			eject_timer.set(0.05) ;
			mState = EjectState::WaitingOnTime ;
		}
		else if (eject_timer.done())
		{
			//
			// We were waiting for the cube sensor to turn off but it has
			// not after our fail safe timeout.  Assume the sensor is broken
			// and stuck on and stop the eject operation.
			//
			mState = EjectState::Done ;
		}
		break ;

	case EjectState::WaitingOnTime:
		//
		// The timer has expired while we were waiting on the timer.  We are done.
		//
		if (eject_timer.done())
			mState = EjectState::Done ;
		break ;

	case EjectState::Done:
		Grabber::grabber_controller.setIdle() ;
		break ;
	}		

    //
    // Update the timer.  When it expires we consider this step done no matter
    // what else is happening
    //
    eject_timer.update(info.in.now, info.in.robot_mode.enabled);

    //
    // Tell the grabber/intake to eject the cube
    //
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
	if (mPowerApplied)
		goals.intake = Intake::Goal::out(mPower);
	else
		goals.intake = Intake::Goal::out();
    
    return goals;
}

unique_ptr<Step_impl> Eject::clone()const{
    return unique_ptr<Step_impl>(new Eject(*this));
}

bool Eject::operator==(Eject const& b)const{
    return eject_timer == b.eject_timer;
}

//
// Drop_grabber: Drop the grabber down at the beginning of the match
//
Drop_grabber::Drop_grabber(){}

Step::Status Drop_grabber::done(Next_mode_info info){
    Step::Status ret =  ready(status(info.status.lifter), Lifter::Goal::go_to_preset(LifterController::Preset::FLOOR)) ? Step::Status::FINISHED_SUCCESS : Step::Status::UNFINISHED;
    if (ret == Step::Status::FINISHED_SUCCESS) 
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Drop grabber step complete" ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Drop_grabber::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Drop_grabber::run(Run_info info,Toplevel::Goal goals){
    Lifter::Goal drop_grabber_goal = Lifter::Goal::go_to_preset(LifterController::Preset::DROP_GRABBER);
    if(!ready(status(info.status.lifter), drop_grabber_goal))
		goals.lifter = drop_grabber_goal;
    else
    	goals.lifter = Lifter::Goal::go_to_preset(LifterController::Preset::FLOOR);
	
    goals.grabber = Grabber::Goal::hold();
    return goals;
}

unique_ptr<Step_impl> Drop_grabber::clone()const{
    return unique_ptr<Step_impl>(new Drop_grabber(*this));
}

bool Drop_grabber::operator==(Drop_grabber const& b)const{
    return true;
}

//
// Drive_and_collect: Drive forward and collect until a cube is collected
//

double Drive_and_collect::distance_travelled;

Drive_and_collect::Drive_and_collect(double maxdist):init(false)
{
	maxdistance = maxdist ;
}

Step::Status Drive_and_collect::done(Next_mode_info info){
	Drivebase::Distances distances_travelled = info.status.drive.distances - initial_distances;

    Step::Status ret = Step::Status::UNFINISHED ;

    bool distdone =  ready(info.status.drive, Drivebase::Goal::drive_straight()) ;
	if (distdone || Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
		ret = Step::Status::FINISHED_SUCCESS ;
	
    if (ret == Step::Status::FINISHED_SUCCESS) 
    {
		distance_travelled = (distances_travelled.l + distances_travelled.r) / 2.0;

		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Drive and collect step complete, elapsed " << info.in.now - mStart ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Drive_and_collect::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Drive_and_collect::run(Run_info info,Toplevel::Goal goals){
	if(!init) {
		mStart = info.in.now ;
		double avg_status = (info.status.drive.distances.l + info.status.drive.distances.r) / 2.0;
		Drivebase::drivebase_controller.initDistance(avg_status + maxdistance, info.status.drive.angle,
													 info.in.now, false, true, true) ;
		initial_distances = info.status.drive.distances;
		init = true;
	}

    goals.drive = Drivebase::Goal::drive_straight();
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::OPEN);
	goals.intake = Intake::Goal::in();
	if(Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::GraspCube)
		goals.grabber = Grabber::Goal::clamp();
	
    return goals;
}

unique_ptr<Step_impl> Drive_and_collect::clone()const{
    return unique_ptr<Step_impl>(new Drive_and_collect(*this));
}

bool Drive_and_collect::operator==(Drive_and_collect const& b)const{
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////
Close_collect_no_cube::Close_collect_no_cube(double len)
{
	mInit = false ;
	mTime = len ;
}

Step::Status Close_collect_no_cube::done(Next_mode_info info)
{
    Step::Status ret = Step::Status::UNFINISHED ;
	if (timeout_timer.done() || Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
		ret = Step::Status::FINISHED_SUCCESS ;
	
    if (ret == Step::Status::FINISHED_SUCCESS) 
    {
		messageLogger &logger = messageLogger::get() ;
		logger.startMessage(messageLogger::messageType::debug, SUBSYSTEM_AUTONOMOUS) ;
		logger << "Close_collect_no_cube step complete"  ;
		if (Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::HasCube)
			logger << " - has cube" ;
		else
			logger << " - timed out" ;

		logger << "\n" ;
		logger << "    Time " << info.in.now - mStart ;
		logger.endMessage() ;
    }
    return ret ;
	
}

Toplevel::Goal Close_collect_no_cube::run(Run_info info){
    return run(info,{});
}

Toplevel::Goal Close_collect_no_cube::run(Run_info info,Toplevel::Goal goals){
	if(!mInit) {
		timeout_timer.set(mTime) ;
		mInit = true ;
		mStart = info.in.now ;
	}

	timeout_timer.update(info.in.now, true);
    goals.grabber = Grabber::Goal::go_to_preset(GrabberController::Preset::CLOSED);
	goals.intake = Intake::Goal::in();
	if(Grabber::grabber_controller.getCubeState() == GrabberController::CubeState::GraspCube)
		goals.grabber = Grabber::Goal::clamp();
	
    return goals;
}

unique_ptr<Step_impl> Close_collect_no_cube::clone()const{
    return unique_ptr<Step_impl>(new Close_collect_no_cube(*this));
}

bool Close_collect_no_cube::operator==(Close_collect_no_cube const& b)const{
    return true;
}

///////////////////////////////////////////////////////////////////////////
#ifdef STEP_TEST
void test_step(Step a){
    PRINT(a);
    auto b=a;
    assert(b==a);
    //see that can at least run "run", and "done"
    //a.run(example
    //TODO: try to visit all the possible states that you can get to from the initial one, and see that things don't fail
    set<Step> visited;
    queue<Step> to_visit;
    to_visit|=a;

    //might want to put some limit on the amount of state space that's used.
    //TODO: make a list of the inputs to try
    //first, run all the "run"s
    //second, run ready on all the different things and see if any of them change
    //abort if they do change

    while(to_visit.size()){
		auto f=to_visit.front();
		to_visit.pop();

		if(visited.find(f)!=visited.end()) continue;

		visited|=f;

		//TODO: Actually run some data on it.
    }

    //see that will at some point "done" can be set
}

template<typename T>
void test_step(T t){
    return test_step(Step{t});
}

int main(){
    /*Drive_straight a;
      Chain b{Step{a},Executive{Teleop{}}};
      Chain c{b};
      auto d=c.next_mode(example((Next_mode_info*)0));
      PRINT(c.step);
      PRINT(d);*/
}
#endif
