#include "TerminateAction.h"

namespace xero {
    namespace base{
        TerminateAction::TerminateAction(ActionPtr a, ITerminator &t): term_(t)
        {
            action_ = a ;
        }

        void TerminateAction::start(){
            action_->start() ;
            is_done_ = action_->isDone() ;
        }

        void TerminateAction::run(){
            action_->run() ;
            is_done_ = action_->isDone() ;
            if(!is_done_ && term_.shouldTerminate()) 
                is_done_ = true;
        }

        void TerminateAction::cancel(){
            action_->cancel() ;
            is_done_ = true ;
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

