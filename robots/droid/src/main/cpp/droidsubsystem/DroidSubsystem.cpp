#include "DroidSubsystem.h"
#include "droidids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {
        DroidSubsystem::DroidSubsystem(Robot &robot) : RobotSubsystem(robot, "droid") {

            ////%% Start AddDriveBase
            addTankDrive() ;
            ////%% End AddDriveBase

            ////%% Start AddChildSubsystems
            oi_ = std::make_shared<DroidOISubsystem>(this) ;
            addChild(oi_) ;
            ////%% End AddChildSubsystems
        }

        DroidSubsystem::~DroidSubsystem() {
        }

        bool DroidSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
