#pragma once

#include <Robot.h>
#include "TubManipulatorSubsystem.h"
#include "TubManipulatorAction.h"
#include <actions/Action.h>
#include <actions/ParallelAction.h>

namespace xero {
    namespace ranseur {

        class TubManipulatorCollectAction : public xero::ranseur::TubManipulatorAction {
  
        public:
            TubManipulatorCollectAction(TubManipulatorSubsystem &) ;
            virtual ~TubManipulatorCollectAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }
                         
        private:
            static std::string action_name ;
            xero::base::ParallelAction parallel_ ;

        };
    }
}
