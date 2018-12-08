#include "LifterPowerAction.h"
#include "Lifter.h"
#include <Robot.h>

namespace xero {
    namespace phoenix {
        LifterPowerAction::LifterPowerAction(Lifter &lifter, double duty_cycle, bool lowgear) : LifterAction(lifter) {
            duty_cycle_ = duty_cycle ;
            low_gear_ = lowgear ;
        }

        LifterPowerAction::LifterPowerAction(Lifter &lifter, const std::string &name, bool lowgear) : LifterAction(lifter) {
            duty_cycle_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
            low_gear_ = lowgear ;
        }        

        LifterPowerAction::~LifterPowerAction() {            
        }

        std::string LifterPowerAction::toString() {
            std::string result = "LifterDutyCycle " + std::to_string(duty_cycle_) ;
			return result ;
        }

        void LifterPowerAction::start() {
            Lifter &lifter = getLifter() ;

            if (low_gear_)
                lifter.setLowGear() ;
			else
				lifter.setHighGear() ;
                                
            lifter.setBrakeOff() ;
			lifter.setMotorDutyCycle(duty_cycle_) ;
        }

		void LifterPowerAction::run() {			
		}
    }
}
