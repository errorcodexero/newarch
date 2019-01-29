#include "HatchHolderAction.h"
#include "HatchHolder.h"
#include "Robot.h"

using namespace xero::misc ;


namespace xero {
    namespace phaser {
            void HatchHolderAction::start() {
                switch(operation_){
                    case EXTEND_ARM:
                        subsystem_.extendArm() ;
                        break;
                    case RETRACT_ARM:
                        subsystem_.retractArm() ;
                        break;
                    case EXTEND_FINGER:
                        subsystem_.extendFinger() ;
                        break;
                    case RETRACT_FINGER:
                        subsystem_.retractFinger() ;
                        break;
                    default:
                        subsystem_.getRobot().getMessageLogger().startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                        subsystem_.getRobot().getMessageLogger() << "Hatch Holder Action: Unexpected Operation";              
                        subsystem_.getRobot().getMessageLogger().endMessage() ;
                        break;
                }
                is_done_ = true ;
            }

            void HatchHolderAction::run() {
            
            }

            bool HatchHolderAction::isDone() {
                return is_done_ ;
            }

            void HatchHolderAction::cancel() {
            }

            std::string HatchHolderAction::toString() {
                std::string result = "HatchHolderAction " ;
                result += std::to_string(operation_) ;
                return result ;
            }
    }
}
