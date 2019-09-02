#include "StraightCharAutoMode.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveCharAction.h>
#include <tankdrive/TankDriveTimedPowerAction.h>
using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace base {
        StraightCharAutoMode::StraightCharAutoMode(Robot &robot, double power, double time, bool highgear) : AutoMode(robot, "Characterize the drivebase", "Characterize the drivebase")
        {            
            auto db = robot.getDriveBase() ;
            auto tank = std::dynamic_pointer_cast<TankDrive>(db) ;

            pushSubActionPair(tank, std::make_shared<TankDriveCharAction>(*tank, time, power, highgear)) ;
        }

        StraightCharAutoMode::~StraightCharAutoMode()
        {            
        }
    }
}