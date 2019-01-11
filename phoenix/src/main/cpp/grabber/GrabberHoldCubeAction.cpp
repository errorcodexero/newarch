#include "GrabberHoldCubeAction.h"
#include "Grabber.h"
#include <Robot.h>
#include <algorithm>
#include <numeric>

namespace xero {
    namespace phoenix {

        std::string GrabberHoldCubeAction::action_name("GrabberHoldCube") ;

        GrabberHoldCubeAction::GrabberHoldCubeAction(Grabber &g) : GrabberAction(g) {
            hold_value_ = g.getRobot().getSettingsParser().getDouble("grabber:hold") ;
        }

        GrabberHoldCubeAction::~GrabberHoldCubeAction() {            
        }
        
        void GrabberHoldCubeAction::run() {
            getGrabber().motor_->Set(hold_value_) ;
        }
    }
}