#include "TerminateAction.h"
#include "DispatchAction.h"
#include "basegroups.h"
#include <iostream>

using namespace xero::misc ;

namespace xero {
    namespace base{
        TerminateAction::TerminateAction(ActionPtr a, Robot &robot, double delay) : CompositeAction(a->getMessageLogger()), robot_(robot)
        {
            action_ = a ;
            delay_ = delay ;
        }

        TerminateAction::TerminateAction(std::shared_ptr<Subsystem> sub, ActionPtr a, Robot &robot, double delay) : CompositeAction(a->getMessageLogger()), robot_(robot)
        {
            action_ = std::make_shared<DispatchAction>(sub, a, true) ;
            delay_ = delay ;
        }        

        void TerminateAction::start() {
            Action::start();

            action_->start() ;
            if (action_->isDone())
                setDone();
            start_ = robot_.getTime() ;
        }

        void TerminateAction::run() {
            Action::run();

            auto &logger = robot_.getMessageLogger() ;
            action_->run() ;

            if (action_->isDone())
            {
                setDone();
            }
            else if (robot_.getTime() - start_ > delay_) {
                //
                // For the terminate action, we have the ability to delay a fixed amount
                // of time before we start looking for the termination condition.
                // 
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
                    setDone();
                }
            }
        }

        void TerminateAction::cancel() {
            Action::cancel();

            if (!action_->isDone())
                action_->cancel();

            setDone();
        }

        std::string TerminateAction::toString(){
            std::string ret = "TerminateAction " ;
            ret += action_->toString() ;

            return ret ;
        }
    }
}

