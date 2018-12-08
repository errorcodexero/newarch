#include "LifterHoldClimbAction.h"
#include "Lifter.h"
#include <Robot.h>

namespace xero {
    namespace phoenix {
        LifterHoldClimbAction::LifterHoldClimbAction(Lifter &lifter, int value) : LifterAction(lifter) {
            delta_ = value ;
        }

        LifterHoldClimbAction::LifterHoldClimbAction(Lifter &lifter, const std::string &name) : LifterAction(lifter) {
            delta_ = getLifter().getRobot().getSettingsParser().getInteger(name) ;
        }        

        LifterHoldClimbAction::~LifterHoldClimbAction() {            
        }

        std::string LifterHoldClimbAction::toString() {
            std::string result = "LifterHoldClimbAction " + std::to_string(delta_) ;
            return result ;
        }

        void LifterHoldClimbAction::start() {
            encoder_value_ = getLifter().getEncoderValue() ;
        }

        void LifterHoldClimbAction::run() {
            Lifter &lifter = getLifter() ;            
            if (lifter.getEncoderValue() < encoder_value_ - delta_) {
                //
                // The lift has slipped,
                //
                lifter.setBrakeOff() ;
                lifter.setLowGear() ;
                if (lifter.getEncoderValue() < encoder_value_) {
					lifter.setMotorDutyCycle(1.0) ;
                }
            }
            else
            {
				lifter.setMotorDutyCycle(0.0) ;
                lifter.setBrakeOn() ;
            }
        }
    }
}
