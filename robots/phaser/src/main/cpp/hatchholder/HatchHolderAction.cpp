#include "HatchHolderAction.h"
#include "HatchHolder.h"
#include "Robot.h"

using namespace xero::misc ;


namespace xero {
    namespace phaser {
            void HatchHolderAction::start() {
                start_ = subsystem_.getRobot().getTime() ;
                switch(operation_){
                    case Operation::EXTEND_ARM:
                        subsystem_.extendArm() ;
                        break;
                    case Operation::RETRACT_ARM:
                        subsystem_.retractArm() ;
                        break;
                    case Operation::EXTEND_FINGER:
                        subsystem_.extendFinger() ;
                        break;
                    case Operation::RETRACT_FINGER:
                        subsystem_.retractFinger() ;
                        break;
                    default:
                        subsystem_.getRobot().getMessageLogger().startMessage(MessageLogger::MessageType::debug, MSG_GROUP_TANKDRIVE) ;
                        subsystem_.getRobot().getMessageLogger() << "Hatch Holder Action: Unexpected Operation";              
                        subsystem_.getRobot().getMessageLogger().endMessage() ;
                        break;
                }

                if (duration_ < 0.001)
                    is_done_ = true ;
            }

            void HatchHolderAction::run() {
                if (!is_done_) {
                    if (subsystem_.getRobot().getTime() - start_ > duration_) {
                        if (operation_ == Operation::EXTEND_ARM || operation_ == Operation::RETRACT_ARM)
                            subsystem_.stopArm() ;

                        is_done_ = true ;
                    }
                }
            }

            bool HatchHolderAction::isDone() {
                return is_done_ ;
            }

            void HatchHolderAction::cancel() {
            }

            std::string HatchHolderAction::toString(Operation oper)
            {
                std::string ret = "Unknown" ;
                
                switch(oper)
                {
                case Operation::EXTEND_ARM:
                    ret = "EXTEND_ARM" ;
                    break;
                case Operation::RETRACT_ARM:
                    ret = "RETRACT_ARM" ;
                    break;
                case Operation::EXTEND_FINGER:
                    ret = "EXTEND_FINGER" ;
                    break;
                case Operation::RETRACT_FINGER:
                    ret = "RETRACT_FINGER" ;
                    break;  
                }

                return ret ;              
            }

            std::string HatchHolderAction::toString() {
                std::string result = "HatchHolderAction " ;
                result += toString(operation_) ;
                return result ;
            }
    }
}
