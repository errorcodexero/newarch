#include "CenterSwitchRight.h"

namespace xero {
    namespace phoenix {
        CenterSwitchRight::CenterSwitchRight(Phoenix &robot) : PhoenixAutoMode(robot, "CenterSwitch") {
            create() ;
        }

        CenterSwitchRight::~CenterSwitchRight() {
        }

        void CenterSwitchRight::create() {
            calibrate() ;
            lift("lifter:height:switch", false) ;
            driveStraight("auto:c_switch_r:segment1") ;
            eject("collector:eject:fast") ;
            driveStraight("auto:c_switch_r:segment2") ;
            rotate(90) ;
            driveAndCollect("auto:c_switch_r:segment3") ;
        }
    }
}