#include "TerminateAction.h"
#include "basegroups.h"
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base{
        TerminateAction::TerminateAction(ActionPtr a, MessageLogger &logger): logger_(logger)
        {
            action_ = a ;
        }

        void TerminateAction::start(){
            action_->start() ;
            is_done_ = action_->isDone() ;
        }

        void TerminateAction::run() {
            if (!is_done_) {
                action_->run() ;
                is_done_ = action_->isDone() ;

                bool termstate = false ;
                for(auto term : terminators_) {
                    if (term->shouldTerminate())
                    {
                        logger_.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                        logger_ << "TerminateAction: " << term->getTerminatorName() << " terminated action" ;
                        logger_.endMessage() ;
                        termstate = true ;
                        break ;
                    }
                }

                if(termstate) {
                    action_->cancel() ;
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

