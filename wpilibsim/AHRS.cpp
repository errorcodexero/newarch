#include "AHRS.h"
#include "RobotSimBase.h"

using namespace xero::sim ;

AHRS::AHRS(const char *port_p)
{
	port_ = port_p ;

    RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
    sim.connect(this) ;	
}

AHRS::~AHRS()
{
    RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
    sim.disconnect(this) ;		
}

double AHRS::GetYaw()
{
    std::lock_guard<std::mutex> lock(getLockMutex()) ;
	return yaw_ ;
}

void AHRS::ZeroYaw()
{
	yaw_ = 0 ;
    changed() ;
}
