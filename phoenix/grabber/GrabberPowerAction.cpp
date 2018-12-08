#include "GrabberPowerAction.h"
#include "Grabber.h"
#include <Robot.h>
#include <algorithm>
#include <numeric>

namespace xero {
    namespace phoenix {
        GrabberPowerAction::GrabberPowerAction(Grabber &g, double duty_cycle) : GrabberAction(g) {
            duty_cycle_ = duty_cycle ;
        }

        GrabberPowerAction::GrabberPowerAction(Grabber &g, const std::string &name) : GrabberAction(g) {
            duty_cycle_ = g.getRobot().getSettingsParser().getDouble(name) ;
        }

        GrabberPowerAction::~GrabberPowerAction() {            
        }

        std::string GrabberPowerAction::toString() {
            std::string result = "GrabberVoltage " + std::to_string(duty_cycle_) ;
            return result ;
        }

        void GrabberPowerAction::run() {
            getGrabber().motor_->Set(duty_cycle_) ;
        }
    }
}
