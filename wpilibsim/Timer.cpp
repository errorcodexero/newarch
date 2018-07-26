#include "Timer.h"
#include "RobotSimBase.h"
#include <chrono>

using namespace xero::sim ;

namespace frc
{
	Timer::Timer()
	{
		RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;
	}

	Timer::~Timer()
	{
		RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.disconnect(this) ;
	}

	double Timer::GetFPGATimestamp()
	{
		RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
		return sim.getTime() ;
	}

	void Wait(double secs)
	{
		RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
		sim.wait(secs) ;
	}
}
