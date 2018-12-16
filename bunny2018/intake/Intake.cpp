#include "Intake.h"
#include "IntakeAction.h"
#include "bunnyids.h"
#include <Robot.h>

using namespace xero::base;
using namespace xero::misc;

namespace xero {
    namespace bunny2018 {
        Intake::Intake(xero::base::Robot& robot) : SingleMotorSubsystem(robot, "shooter", "hw:shooter:motor", false /*not victor*/) {

            int sensor_addr = robot.getSettingsParser().getInteger("hw:intake:sensor") ;
            if (sensor_addr != -1)
                ball_sensor_ = std::make_shared<frc::DigitalInput>(sensor_addr) ;
        }

        Intake::~Intake(){
        }

        void Intake::computeState() {
        }
    
		bool Intake::canAcceptAction(ActionPtr action) {
            // Allow action to to set motor power that's derived from the base class
            if (SingleMotorSubsystem::canAcceptAction(action)) {
                return true;
            }

			std::shared_ptr<IntakeAction> act_p = std::dynamic_pointer_cast<IntakeAction>(action) ;
			if (act_p == nullptr)
				return false ;

			return true ;
		}
    }
}
