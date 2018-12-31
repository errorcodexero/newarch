#include "CenterSwitchLeft.h"

namespace xero {
    namespace phoenix {
        CenterSwitchLeft::CenterSwitchLeft(Phoenix &robot) : PhoenixAutoMode(robot, "CenterSwitch") {
            create() ;
        }

        CenterSwitchLeft::~CenterSwitchLeft() {
        }

        void CenterSwitchLeft::create() {
            calibrate() ;
            driveStraight("auto:c_switch_l:segment1") ;
            rotate(-90) ;
            driveStraight("auto:c_switch_l:segment2") ;         
            rotate(90) ;
            lift("lifter:height:switch", false) ;           
            driveStraight("auto:c_switch_l:segment3") ;
            eject("collector:eject:fast") ;
        }
    }
}