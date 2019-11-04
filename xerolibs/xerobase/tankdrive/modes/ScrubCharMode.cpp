#include <tankdrive/TankDrive.h>
#include <tankdrive/modes/ScrubCharMode.h>
#include <tankdrive/actions/TankDriveScrubCharAction.h>

namespace xero {
    namespace base {
        ScrubCharMode::ScrubCharMode(Robot &robot, double power, double total, bool highgear) : AutoMode(robot, "Scrub Characterization", "Scrub Characterization")
        {            
            auto db = robot.getDriveBase() ;
            auto tank = std::dynamic_pointer_cast<TankDrive>(db) ;

            pushSubActionPair(tank, std::make_shared<TankDriveScrubCharAction>(*tank, power, total, highgear)) ;
        }

        ScrubCharMode::~ScrubCharMode()
        {            
        }
    }
}