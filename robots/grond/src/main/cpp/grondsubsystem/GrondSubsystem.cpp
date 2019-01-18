#include "GrondSubsystem.h"
#include "grondids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace grond {
        GrondSubsystem::GrondSubsystem(Robot &robot) : RobotSubsystem(robot, "grond") {
            //
            // Add the tank drive.  This is handled by the base class RobotSubsystem since all robots have a drivebase
            // and for now they are all tank drives
            //
            addTankDrive() ;

            //
            // Add the OI to the robot.  The OI is specific to this robot.
            //
            oi_ = std::make_shared<GrondOISubsystem>(robot) ;
            addChild(oi_) ;            
        }

        GrondSubsystem::~GrondSubsystem() {
        }
    }
}
