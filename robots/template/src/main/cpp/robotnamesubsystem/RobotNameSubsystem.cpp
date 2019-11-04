#include "$$RobotName$$Subsystem.h"
#include "$$robotname$$ids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace $$robotname$$ {
        $$RobotName$$Subsystem::$$RobotName$$Subsystem(Robot &robot) : RobotSubsystem(robot, "$$robotname$$") {
            //
            // Add the tank drive.  This is handled by the base class RobotSubsystem since all robots have a drivebase
            // and for now they are all tank drives
            //
            addTankDrive() ;

            //
            // Add the OI to the robot.  The OI is specific to this robot.
            //
            oi_ = std::make_shared<$$RobotName$$OISubsystem>(robot) ;
            addChild(oi_) ;
        }

        $$RobotName$$Subsystem::~$$RobotName$$Subsystem() {
        }

        bool $$RobotName$$Subsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
