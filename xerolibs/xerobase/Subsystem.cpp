#include "Subsystem.h"
#include "Action.h"
#include "Robot.h"
#include "basegroups.h"
#include <MessageLogger.h>
#include <cassert>

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
                if (pending_ != nullptr && action_->isDone()) {
                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                    logger << "Actions: subsystem '" << getName() << "' pending action '" << pending_->toString() ;
                    logger << "' was started" ;
                    logger.endMessage() ;

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

        bool Subsystem::setAction(ActionPtr action, bool force) {

            //
            // Check that the action is valid for this subsystem.  If not, print and error
            // and do nothing else.  Any existing action remains attached to the subsystem and
            // continue to perform its function.
            //
            if (action != nullptr && !canAcceptAction(action)) {
                MessageLogger &logger = getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                logger << "Actions: subsystem '" << getName() << "' rejected action '" << action->toString() << "'" ;
                logger.endMessage() ;
                return false ;
            }

            //
            // Print information about what is being requested
            //
            MessageLogger &logger = getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
            if (action == nullptr)
                logger << "Actions: subsystem '" << getName() << "' was assigned NULL action" ;
            else
                logger << "Actions: subsystem '" << getName() << "' was assigned action '" << action->toString() << "'" ;    
            if (force)
                logger << " - FORCED" ;
            logger.endMessage() ;            

            if (action_ != nullptr && !action_->isDone()) {
                //
                // The current Action is still running, interrupt it
                //

                if (force) {

                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
                    logger << "Actions: subsystem '" << getName() << "' action '" << action_->toString() << "' was aborted" ;
                    logger.endMessage() ;                    

                    //
                    // We want to force the action here, so abort the current action
                    // and assign this one
                    //
                    action_->abort() ;
                    pending_ = nullptr ;
                    assert(action_->isDone()) ;
                }
                else {
                    MessageLogger &logger = getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
                    logger << "Actions: subsystem '" << getName() << "' action '" << action_->toString() << "' was canceled" ;
                    logger.endMessage() ; 

                    //
                    // We are not forcing the issue (e.g. force was false) so cancel the action and
                    // pend the new action.  THe new action will get assigned when the old action
                    // finishes its cancel.
                    //
                    cancelAction();

                    if(!action_->isDone()) {
                        //
                        // The old action did not finish immedately after cancel was called.  Pend the
                        // new action and wait for the canceled action to complete
                        //
                        pending_ = action ;

                        MessageLogger &logger = getRobot().getMessageLogger() ;
                        logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS_VERBOSE) ;
                        if (action == nullptr) {
                            logger << "Actions: subsystem '" << getName() << "' action NULL was pended" ;
                        }
                        else {
                            logger << "Actions: subsystem '" << getName() << "' action '" << action->toString() << "' was pended" ;                            
                        }
                        logger.endMessage() ;                    
                    }
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
            for(auto child: children_) {
                child->init(ltype) ;
            }
        }
    }
}
