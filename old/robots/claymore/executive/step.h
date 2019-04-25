#ifndef STEP_H
#define STEP_H

#include "executive.h"
#include "motion_profile.h"
#include "settable_constant.h"
#include "robot_constants.h"

struct Step_impl;

class Step{
public:
    enum class Status{UNFINISHED,FINISHED_SUCCESS,FINISHED_FAILURE};

private:
    std::unique_ptr<Step_impl> impl;
	std::vector<Step>* fail_branch;

public:
    explicit Step(Step_impl const&);
	explicit Step(Step_impl const&, std::vector<Step>);
    Step(Step const&);
	Step(Step const&, std::vector<Step>);

	std::vector<Step>* get_fail_branch();
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Status done(Next_mode_info);
    Step_impl const& get()const;
    void display(std::ostream&)const;
    bool operator==(Step const&)const;
    bool operator<(Step const&)const;
};

std::ostream& operator<<(std::ostream&,Step const&);

struct Step_impl{
    virtual ~Step_impl();

    virtual Toplevel::Goal run(Run_info,Toplevel::Goal)=0;
    virtual Toplevel::Goal run(Run_info)=0;
    virtual Step::Status done(Next_mode_info)=0;//could try to make this const.
    virtual std::unique_ptr<Step_impl> clone()const=0;
    virtual void display(std::ostream&)const;
    virtual bool operator<(Step const&)const=0;
    virtual bool operator==(Step const&)const=0;
};

template<typename T>
struct Step_impl_inner:Step_impl{
    std::unique_ptr<Step_impl> clone()const{
	T const& t=dynamic_cast<T const&>(*this);
	return std::unique_ptr<Step_impl>(new T(t));
    }
	
    void display(std::ostream& o)const{
	o<<type(*(T*)this);
    }

    bool operator==(Step const& a)const{
	//Step_impl const& b=a.get();
	//if(type_index(typeid(*this))!=type_index
	T const& t=dynamic_cast<T const&>(a.get());
	return operator==(t);
    }

    virtual bool operator==(T const&)const=0;

    bool operator<(Step const&)const{
	nyi
	    }
};

using Inch=double;

//Run two steps simultaneously
class Combo: public Step_impl_inner<Combo>{
    Step step_a;
    Step step_b;
public:
    explicit Combo(Step,Step);//the second step will overwrite goals from the first one if they both modify the same parts of the robot

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    void display(std::ostream& o)const;
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Combo const&)const;
};

//Wait for a specified amount of time
class Wait: public Step_impl_inner<Wait>{
    Countdown_timer wait_timer;
public:
    explicit Wait(Time);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Wait const&)const;
};

class StartAuto: public Step_impl_inner<StartAuto>{
public:
	std::string mProgramName ;
	bool mInited ;
	
    StartAuto(const char *name_p) ;
    
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(StartAuto const &)const;
} ;

class EndAuto: public Step_impl_inner<EndAuto>{
public:
    EndAuto() ;
    
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(EndAuto const &)const;
} ;


//Drive straight a specified distance
class Drive:public Step_impl_inner<Drive>{
protected:
    Inch mTargetDistance;
	Inch mTargetAngleOffset;
	Inch mReturnOffset;
	std::string mParamName ;
    bool mEndOnStall ;
	bool mCurve;
	Inch mCurveStart ;
	bool mReturnFromCollect ;
    bool mInited ;

public:
    explicit Drive(Inch dist, bool end_on_stall=false);
	explicit Drive(Inch dist, double curve_start, double angle_offset, bool end_on_stall=false);
    explicit Drive(const char *param_p, Inch dist, bool end_on_stall=false);
    explicit Drive(const std::string &param, Inch dist, bool end_on_stall=false);
	explicit Drive(bool dummy, Inch return_offset=0);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Drive const&)const;
};

//Drive the motors at the specified powers for a specified amount of time
class Drive_timed:public Step_impl_inner<Drive_timed>{
    double left_power;
    double right_power;
    Countdown_timer timer;
	
public:
    explicit Drive_timed(double, double, double);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Drive_timed const&)const;
};

//Drive the motors at the specified powers for the specified distances
class Ram:public Step_impl_inner<Ram>{
    double left_power;
    double right_power;
    Drivebase::Distances target_distances;
    Drivebase::Distances initial_distances;
    bool init;

public:
    explicit Ram(double, double, Inch, Inch);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Ram const&)const;
};

//Rotate the robot by a specified angle
struct Rotate: Step_impl_inner<Rotate>{
    double target_angle;
    bool init;
	double tolerance ;
	bool tolprovided ;
	bool opengrabber;

    explicit Rotate(double angle, bool open_grabber=false);
	explicit Rotate(double angle, double tolerance, bool open_grabber=false) ;
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Rotate const&)const;
};

//Rotate the robot by a specified angle
struct Rotate_finish: Step_impl_inner<Rotate_finish>{
    double target_angle;
	double prev_angle ;
	double tolerance ;
    bool init;
	bool tolprovided ;

    explicit Rotate_finish(double prevangle, double angle);
    explicit Rotate_finish(double prevangle, double angle, double tol);
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Rotate_finish const&)const;
};

//Rotate the robot by a specified angle
struct Rotate_back: Step_impl_inner<Rotate_back>{
    bool init;
	double mOffset ;
	double mTolerance ;
	bool mTolSpecified ;

    explicit Rotate_back() ;
	explicit Rotate_back(double offset) ;
	explicit Rotate_back(double offset, double tolerance) ;
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Rotate_back const&)const;
};


//Start moving the lifter to a specified preset in the background
struct Background_lifter_to_preset: Step_impl_inner<Background_lifter_to_preset>{
    LifterController::Preset preset;
    double time;
	double delay ;
	double height ;
	bool heightgiven ;
    bool init;

    explicit Background_lifter_to_preset(LifterController::Preset, double);
    explicit Background_lifter_to_preset(double, double);
    explicit Background_lifter_to_preset(LifterController::Preset, double, double);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Background_lifter_to_preset const&)const;
};

//Calibrate the lifter at the current height
struct Calibrate_lifter: Step_impl_inner<Calibrate_lifter>{
	bool mInited ;
    explicit Calibrate_lifter();
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Calibrate_lifter const&)const;
};

//Move the lifter to a sepecified height
struct Lifter_to_height: Step_impl_inner<Lifter_to_height>{
	double mTargetHeight ;
	std::string mParamName ;
	bool mInited ;
	
    explicit Lifter_to_height(double height);
    explicit Lifter_to_height(const char *param_p, double height);
    explicit Lifter_to_height(const std::string &param, double height);
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Lifter_to_height const&)const;
};

//Move the lifter to a specified preset
struct Lifter_to_preset: Step_impl_inner<Lifter_to_preset>
{
    LifterController::Preset mPreset;
    double mTime;
	Countdown_timer fail_timer;
    bool mInit;

    explicit Lifter_to_preset(LifterController::Preset, double);
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Lifter_to_preset const&)const;
};

//Calibrate the grabber at the current angle
struct Calibrate_grabber: Step_impl_inner<Calibrate_grabber>{
    bool mInited ;
    
    explicit Calibrate_grabber();
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Calibrate_grabber const&)const;
};

//Move the grabber to a sepecified angle preset
struct Grabber_to_preset: Step_impl_inner<Grabber_to_preset>{
    GrabberController::Preset target_preset;
    double time;
    bool init;

    explicit Grabber_to_preset(GrabberController::Preset, double);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Grabber_to_preset const&)const;
};

//Put the grabber into collecting mode until a cube is collected
struct Collect: Step_impl_inner<Collect>{
    double time;
    bool init;

    explicit Collect(double);

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Collect const&)const;
};

//Eject a cube
struct Eject: Step_impl_inner<Eject>
{
	enum class EjectState
	{
		//
		// We are ready to start an eject operation
		//
		Start,

		//
		// We are waiting on a timer to expire to be done
		//
		WaitingOnTime,

		//
		// Waiting for the cube sensor to turn off (or the timer)
		//
		WaitingCubeSensorOff,

		//
		// Eject is done
		//
		Done
	} ;
	
	EjectState mState ;
    Countdown_timer eject_timer;
	double mStart ;
	bool mPowerApplied ;
	double mPower ;

    explicit Eject();
	explicit Eject(double power) ;
	
    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Eject const&)const;
};

//Drop the grabber down at the beginning of the match
struct Drop_grabber: Step_impl_inner<Drop_grabber>{
    explicit Drop_grabber();

    Toplevel::Goal run(Run_info,Toplevel::Goal);
    Toplevel::Goal run(Run_info);
    Step::Status done(Next_mode_info);
    std::unique_ptr<Step_impl> clone()const;
    bool operator==(Drop_grabber const&)const;
};

//Drive forward and collect until a cube is collected
struct Close_collect_no_cube: Step_impl_inner<Close_collect_no_cube>{
	bool mInit ;
	double mTime ;
	double mStart ;
	Countdown_timer timeout_timer;
	
	explicit Close_collect_no_cube(double length);

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Close_collect_no_cube const&)const;
};

//Drive forward and collect until a cube is collected
struct Drive_and_collect: Step_impl_inner<Drive_and_collect>
{
	static double distance_travelled;

	Drivebase::Distances initial_distances;
	bool init;
	double mStart ;
	double maxdistance ;

	explicit Drive_and_collect(double maxdist) ;

	Toplevel::Goal run(Run_info,Toplevel::Goal);
	Toplevel::Goal run(Run_info);
	Step::Status done(Next_mode_info);
	std::unique_ptr<Step_impl> clone()const;
	bool operator==(Drive_and_collect const&)const;
};

#endif
