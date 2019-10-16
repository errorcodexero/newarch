#include "CarlosHatchArmAction.h"
#include "CarlosHatch.h"
#include "Robot.h"

using namespace xero::misc ;


namespace xero {
    namespace phaser {
        void CarlosHatchArmAction::start() {
            is_done_ = false ;
            start_ = getSubsystem().getRobot().getTime() ;
            switch(operation_) {
                case Operation::EXTEND:
                    getSubsystem().extendArm() ;
                    break;

                case Operation::RETRACT:
                    getSubsystem().retractArm() ;
                    break;

                case Operation::OPEN:
                    getSubsystem().enableHooks() ;
                    break ;

                case Operation::CLOSE:
                    getSubsystem().disableHooks() ;
                    break ;

                default:
                    getSubsystem().getRobot().getMessageLogger().startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                    getSubsystem().getRobot().getMessageLogger() << "Hatch Holder Action: Unexpected Operation";              
                    getSubsystem().getRobot().getMessageLogger().endMessage() ;
                    break;
            }
        }

        void CarlosHatchArmAction::run() {
            if (getSubsystem().getRobot().getTime() - start_ > delay_)
                is_done_ = true ;
        }

        bool CarlosHatchArmAction::isDone() {
            return is_done_ ;
        }

        void CarlosHatchArmAction::cancel() {
        }

        std::string CarlosHatchArmAction::toString(Operation oper)
        {
            std::string ret;
            switch(oper) {
            case Operation::EXTEND:
                ret = "EXTEND" ;
                break ;

            case Operation::RETRACT:
                ret = "RETRACT" ;
                break ;

            case Operation::OPEN:
                ret = "OPEN" ;
                break ;

            case Operation::CLOSE:
                ret = "CLOSE" ;
                break ;
            }
            return ret ;              
        }

        std::string CarlosHatchArmAction::toString() {
            std::string result = "CarlosHatchArmAction " ;
            result += toString(operation_) ;
            return result ;
        }
    }
}
