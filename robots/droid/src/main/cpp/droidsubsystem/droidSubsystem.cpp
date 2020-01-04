#include "droidSubsystem.h"
#include "droidids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {
        droidSubsystem::droidSubsystem(Robot &robot) : RobotSubsystem(robot, "droid") {

            ////%% Start AddDriveBase
            addTankDrive() ;
            ////%% End AddDriveBase

            ////%% Start AddChildSubsystems
            oi_ = std::make_shared<droidOISubsystem>(this) ;
            addChild(oi_) ;
            ////%% End AddChildSubsystems
        }

        droidSubsystem::~droidSubsystem() {
        }

        bool droidSubsystem::canAcceptAction(ActionPtr act) {
            return false ;
        }
    }
}
