#include "Subsystem.h"
#include "Action.h"
#include "Robot.h"
#include "basegroups.h"
#include <MessageLogger.h>

using namespace xero::misc ;

namespace xero {
    namespace base {

        Subsystem::Subsystem(Robot &robot, const std::string &name) : robot_(robot) , name_(name) {
			action_ = nullptr ;
		}

        Subsystem::~Subsystem() {
        }

        void Subsystem::run() {
            for(auto sub: children_) {
                sub->run() ;
            }
            
            if (action_ != nullptr)
                action_->run() ;
		}
		
        void Subsystem::computeState() {
            for(auto sub: children_)
                sub->computeState() ;     
        }

		void Subsystem::cancelAction() {
			if (action_ != nullptr)
				action_->cancel() ;

			action_ = nullptr ;
		}

        bool Subsystem::setAction(ActionPtr action) {

            if (action_ != nullptr && !action_->isDone()) {
                //
                // The current Action is still running, interrupt it
                //
                cancelAction();
            }

            if (action != nullptr && !canAcceptAction(action)) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_ACTIONS) ;
                logger << "subsystem '" << getName() << "' rejected action '" << action->toString() << "'" ;
                logger.endMessage() ;
                return false ;
            }

			//
			// And now start the Action
			//
            action_ = action ;
            if (action_ != nullptr) {
                action_->start() ;
			}
			return true ;
        }

		void Subsystem::reset() {
			action_ = nullptr ;

			for(auto child: children_)
				child->reset() ;
		}
    }
}
