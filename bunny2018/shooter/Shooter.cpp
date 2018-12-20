#include "Shooter.h"
#include "ShooterAction.h"
#include "bunnyids.h"
#include <Robot.h>

using namespace xero::base;
using namespace xero::misc;

namespace xero {
    namespace bunny2018 {
        Shooter::Shooter(xero::base::Robot& robot) : SingleMotorSubsystem(robot, "shooter", "hw:shooter:motor", false /*not victor*/) {

            int sensor_addr = robot.getSettingsParser().getInteger("hw:shooter:sensor") ;
            ball_sensor_ = std::make_shared<frc::DigitalInput>(sensor_addr) ;

            stage_motor_power_ = robot.getSettingsParser().getDouble("shooter:stage_motor_power");
            eject_motor_power_ = robot.getSettingsParser().getDouble("shooter:eject_motor_power");
            eject_duration_    = robot.getSettingsParser().getDouble("shooter:eject_duration");

            ball_is_staged_ = false;
        }

        Shooter::~Shooter(){
        }

        void Shooter::computeState() {
            if (ball_sensor_->Get()) {
                ball_is_staged_ = true ;
            }

            auto& logger = getRobot().getMessageLogger() ;

			if (getBallIsStaged()) {
	            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_SHOOTER_VERBOSE) ;
            	logger << "Shooter: ball is staged\n" ;
	            logger.endMessage() ;
			}
            else {
            }
        }
    
		bool Shooter::canAcceptAction(ActionPtr action) {
            // Allow action to to set motor power that's derived from the base class
            if (SingleMotorSubsystem::canAcceptAction(action)) {
                return true;
            }

			std::shared_ptr<ShooterAction> act_p = std::dynamic_pointer_cast<ShooterAction>(action) ;
			if (act_p == nullptr)
				return false ;

			return true ;
		}
    }
}
