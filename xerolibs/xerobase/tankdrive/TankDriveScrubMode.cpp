#include "TankDriveScrubMode.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveScrubCharAction.h>

namespace xero {
    namespace base {
        TankDriveScrubMode::TankDriveScrubMode(Robot &robot, double power, double duration, bool highgear) : AutoMode(robot, "Scrub Characterization", "Scrub Characterization")
        {            
            auto db = robot.getDriveBase() ;
            auto tank = std::dynamic_pointer_cast<TankDrive>(db) ;

            pushSubActionPair(tank, std::make_shared<TankDriveScrubCharAction>(*tank, duration, power, -power, highgear)) ;
        }

        TankDriveScrubMode::~TankDriveScrubMode()
        {            
        }
    }
}