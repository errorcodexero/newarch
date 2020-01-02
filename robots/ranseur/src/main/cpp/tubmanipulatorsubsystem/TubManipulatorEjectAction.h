#pragma once 

#include <Robot.h>
#include "TubManipulatorSubsystem.h"
#include "TubManipulatorAction.h"
#include <actions/Action.h>
#include <actions/ParallelAction.h>
#include <actions/SequenceAction.h>

namespace xero {
    namespace ranseur {

        class TubManipulatorEjectAction : public xero::ranseur::TubManipulatorAction {
        public:

            TubManipulatorEjectAction(TubManipulatorSubsystem &) ;
            virtual ~TubManipulatorEjectAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }

        private:
            static std::string action_name ; 
            xero::base::SequenceAction seq_ ;
            
        };
    }
}
