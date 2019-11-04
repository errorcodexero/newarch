#include "TurntablePowerAction.h"
#include "Turntable.h"
#include <Robot.h>

namespace xero {
    namespace phaser {
        TurntablePowerAction::TurntablePowerAction(Turntable &turntable, double duty_cycle) : TurntableAction(turntable) {
            duty_cycle_ = duty_cycle ;
        }

        TurntablePowerAction::TurntablePowerAction(Turntable &turntable, const std::string &name) : TurntableAction(turntable) {
            duty_cycle_ = getTurntable().getRobot().getSettingsParser().getDouble(name) ;
        }        

        TurntablePowerAction::~TurntablePowerAction() {            
        }

        std::string TurntablePowerAction::toString() {
            std::string result = "TurntableDutyCycle " + std::to_string(duty_cycle_) ;
            return result ;
        }

        void TurntablePowerAction::start() {
            Turntable &turntable = getTurntable() ;
            turntable.setMotorPower(duty_cycle_) ;
        }

        void TurntablePowerAction::run() {         
        }
    }
}
