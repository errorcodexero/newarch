#include "actions/DispatchAction.h"
#include "basegroups.h"
#include "Subsystem.h"
#include "Robot.h"
#include <MessageLogger.h>

using namespace xero::misc ;

namespace xero {
    namespace base {

        DispatchAction::DispatchAction(SubsystemPtr subsystem, ActionPtr action, bool block) 
                        : Action(action->getMessageLogger()), subsystem_(subsystem), action_(action), block_(block) {
        }

        void DispatchAction::start() {
            Action::start();
            
            if (!Subsystem::isAccepted(subsystem_->setAction(action_, true))) {
                MessageLogger &logger = subsystem_->getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_ACTIONS) ;
                logger << "Actions: subsystem '" << subsystem_->getName() << "' rejected action '" ;
                logger << action_->toString() << "'" ;
                logger.endMessage() ;
                setDone();
            }
            else if (!block_)
                setDone();
        }

        void DispatchAction::run() {
            Action::run();
            //
            // The action is assigned to the subsystem so the action runs when
            // the subsystem gets it shot at running.
            //
            if (block_ && action_->isDone())
                setDone();
        }

        void DispatchAction::cancel() {
            Action::cancel();
            if (block_) {
                action_->cancel() ;
                subsystem_->setAction(nullptr) ;
            }
        }

        std::string DispatchAction::toString() {
            std::string result ;

            result = "DispatchAction(" ;
            result += subsystem_->getName() + "," + std::to_string(action_->getID()) + "{{" + action_->toString() ;
            result += "}}, " ;
            result += block_ ? "BLOCK" : "NONBLOCKING" ;
            result += ")" ;
            return result ;
        }
    }
}
