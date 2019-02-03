#include "TerminateAction.h"
#include "basegroups.h"
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base{
        TerminateAction::TerminateAction(ActionPtr a, ITerminator &t, MessageLogger &logger): term_(t), logger_(logger)
        {
            action_ = a ;
        }

        void TerminateAction::start(){
            action_->start() ;
            is_done_ = action_->isDone() ;

            logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
            logger_ << "Child action finished in start" ;
            logger_.endMessage() ;            
        }

        void TerminateAction::run() {
            if (!is_done_) {
                action_->run() ;
                is_done_ = action_->isDone() ;

                if (is_done_) {
                    logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                    logger_ << "Child action finished in run" ;
                    logger_.endMessage() ;                     
                }
                if(!is_done_ && term_.shouldTerminate()) {
                    logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                    logger_ << "Terminating action based on terminator" ;
                    logger_.endMessage() ;
                    is_done_ = true;
                }
            }
        }

        void TerminateAction::cancel() {
            if (!is_done_) {
                action_->cancel() ;
                is_done_ = true ;
            }
        }

        bool TerminateAction::isDone(){
            return is_done_ ;
        }

        std::string TerminateAction::toString(){
            std::string ret = "TerminateAction " ;
            ret += action_->toString() ;

            return ret ;
        }
    }
}

