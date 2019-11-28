#include "$$RobotName$$Subsystem.h"
#include "$$robotname$$ids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace $$robotname$$ {
        $$RobotName$$Subsystem::$$RobotName$$Subsystem(Robot &robot) : RobotSubsystem(robot, "$$robotname$$") {

            ////%% Start AddDriveBase
            addTankDrive() ;
            ////%% End AddDriveBase

            ////%% Start AddChildSubsystems
            oi_ = std::make_shared<$$RobotName$$OISubsystem>(this) ;
            addChild(oi_) ;
            ////%% End AddChildSubsystems
        }

        $$RobotName$$Subsystem::~$$RobotName$$Subsystem() {
        }

        bool $$RobotName$$Subsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
