#include "TerminateAction.h"
#include "DispatchAction.h"
#include "basegroups.h"
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base{
        TerminateAction::TerminateAction(ActionPtr a, Robot &robot, double delay) : robot_(robot)
        {
            action_ = a ;
            delay_ = delay ;
        }

        TerminateAction::TerminateAction(std::shared_ptr<Subsystem> sub, ActionPtr a, Robot &robot, double delay) : robot_(robot)
        {
            action_ = std::make_shared<DispatchAction>(sub, a, true) ;
            delay_ = delay ;
        }        

        void TerminateAction::start(){
            action_->start() ;
            is_done_ = action_->isDone() ;
            start_ = robot_.getTime() ;
        }

        void TerminateAction::run() {
            auto &logger = robot_.getMessageLogger() ;
            if (!is_done_) {
                action_->run() ;
                is_done_ = action_->isDone() ;

                if (robot_.getTime() - start_ > delay_) {
                    bool termstate = false ;
                    for(auto term : terminators_) {
                        if (term->shouldTerminate())
                        {
                            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_ACTIONS) ;
                            logger << "TerminateAction: " << term->getTerminatorName() << " terminated action" ;
                            logger << " " << action_->toString() ;
                            logger.endMessage() ;
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
        }

        void TerminateAction::cancel() {
            if (!is_done_) {
                if (action_->isDone())
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

