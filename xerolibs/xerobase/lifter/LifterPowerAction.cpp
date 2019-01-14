#include "LifterPowerAction.h"
#include "Lifter.h"
#include <Robot.h>

namespace xero {
    namespace base {
        LifterPowerAction::LifterPowerAction(Lifter &lifter, double duty_cycle) : LifterAction(lifter) {
            duty_cycle_ = duty_cycle ;
        }

        LifterPowerAction::LifterPowerAction(Lifter &lifter, const std::string &name) : LifterAction(lifter) {
            duty_cycle_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
        }        

        LifterPowerAction::~LifterPowerAction() {            
        }

        std::string LifterPowerAction::toString() {
            std::string result = "LifterDutyCycle " + std::to_string(duty_cycle_) ;
            return result ;
        }

        void LifterPowerAction::start() {
            Lifter &lifter = getLifter() ;
            lifter.setMotorPower(duty_cycle_) ;
        }

        void LifterPowerAction::run() {         
        }
    }
}
