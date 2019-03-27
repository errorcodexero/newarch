#include "CarlosHatchArmAction.h"
#include "CarlosHatch.h"
#include "Robot.h"

using namespace xero::misc ;


namespace xero {
    namespace phaser {
        void CarlosHatchArmAction::start() {
            start_ = getSubsystem().getRobot().getTime() ;
            is_done_ = false ;
            switch(operation_) {
                case Operation::EXTEND:
                    getSubsystem().extendArm() ;
                    break;

                case Operation::RETRACT:
                    getSubsystem().retractArm() ;
                    break;
                default:
                    getSubsystem().getRobot().getMessageLogger().startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                    getSubsystem().getRobot().getMessageLogger() << "Hatch Holder Action: Unexpected Operation";              
                    getSubsystem().getRobot().getMessageLogger().endMessage() ;
                    break;
            }

            if (duration_ < epsilon_)
                is_done_ = true ;
        }

        void CarlosHatchArmAction::run() {
            if (!is_done_) {
                if (getSubsystem().getRobot().getTime() - start_ > duration_) {
                    getSubsystem().stopArm() ;
                    is_done_ = true ;
                }
            }
        }

        bool CarlosHatchArmAction::isDone() {
            return is_done_ ;
        }

        void CarlosHatchArmAction::cancel() {
        }

        std::string CarlosHatchArmAction::toString(Operation oper)
        {
            std::string ret ;
            return ret ;              
        }

        std::string CarlosHatchArmAction::toString() {
            std::string result = "CarlosHatchArmAction " ;
            result += toString(operation_) ;
            return result ;
        }
    }
}
