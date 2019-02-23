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
            
            if (action_ != nullptr){
                action_->run() ;
                if (pending_ != nullptr && action_->isDone()){
                    action_ = pending_ ;
                    pending_ = nullptr ;
                    action_->start() ;
                }
            }
        }
        
        void Subsystem::computeState() {
            for(auto sub: children_)
                sub->computeState() ;     
        }

        void Subsystem::cancelAction() {
            if (action_ != nullptr)
                action_->cancel() ;
        }

        bool Subsystem::setAction(ActionPtr action) {
            
            if (action != nullptr && !canAcceptAction(action)) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_ACTIONS) ;
                logger << "Actions: subsystem '" << getName() << "' rejected action '" << action->toString() << "'" ;
                logger.endMessage() ;
                return false ;
            }

            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
            if (action == nullptr)
                logger << "Actions: subsystem '" << getName() << "' was assigned NULL action" ;
            else
                logger << "Actions: subsystem '" << getName() << "' was assigned action '" << action->toString() << "'" ;    
                        
            logger.endMessage() ;            

            if (action_ != nullptr && !action_->isDone()) {
                //
                // The current Action is still running, interrupt it
                //
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_ACTIONS_VERBOSE) ;
                logger << "Actions: subsystem '" << getName() << "' action '" << action_->toString() << "' was canceled by setAction" ;
                logger.endMessage() ;   

                cancelAction();

                if(!action_->isDone()) {
                    pending_ = action ;

                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::error, MSG_GROUP_ACTIONS_VERBOSE) ;
                    logger << "Actions: subsystem '" << getName() << "' action '" << action->toString() << "' was pended" ;
                    logger.endMessage() ;                    
                }
            }

            if(pending_ == nullptr){

                //
                // And now start the Action
                //
                action_ = action ;
                if (action_ != nullptr) {
                    action_->start() ;
                }
            }
            return true ;
        }

        void Subsystem::reset() {
            action_ = nullptr ;

            for(auto child: children_)
                child->reset() ;
        }

        void Subsystem::init(LoopType ltype) {
            for(auto child: children_)
                child->init(ltype) ;            
        }
    }
}
