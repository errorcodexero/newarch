#include "TankDriveScrubMode.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveScrubCharAction.h>

namespace xero {
    namespace base {
        TankDriveScrubMode::TankDriveScrubMode(Robot &robot, double power, double total, bool highgear) : AutoMode(robot, "Scrub Characterization", "Scrub Characterization")
        {            
            auto db = robot.getDriveBase() ;
            auto tank = std::dynamic_pointer_cast<TankDrive>(db) ;

            pushSubActionPair(tank, std::make_shared<TankDriveScrubCharAction>(*tank, power, total, highgear)) ;
        }

        TankDriveScrubMode::~TankDriveScrubMode()
        {            
        }
    }
}