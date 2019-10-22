#include "gopigo3sub/GoPiGo3Subsystem.h"
#include "oi/GoPiGo3OISubsystem.h"
#include <RobotSubsystem.h>
#include <Robot.h>

using namespace xero::base ;

// hw:tankdrive:leftmotor           1
// hw:tankdrive:rightmotor          2
// hw:tankdrive:leftenc:1           0
// hw:tankdrive:leftenc:2           1
// hw:tankdrive:rightenc:1          2
// hw:tankdrive:rightenc:2          3

namespace xero {
    namespace gopigo3 {
        GoPiGo3Subsystem::GoPiGo3Subsystem(Robot &robot) : RobotSubsystem(robot, "gopigo") { 
            oi_ = std::make_shared<GoPiGo3OISubsystem>(robot) ;
            addChild(oi_) ;
            addTankDrive() ;
        }

        GoPiGo3Subsystem::~GoPiGo3Subsystem() {         
        }
    }
}
