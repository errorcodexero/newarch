#pragma once

#include <Robot.h>
#include "TubManipulatorSubsystem.h"
#include "TubManipulatorAction.h"
#include <actions/Action.h>

namespace xero {
    namespace ranseur {

        class TubManipulatorCollectAction : public xero::ranseur::TubManipulatorAction {
  
        public:
            TubManipulatorCollectAction(TubManipulator &) ;
            virtual ~TubManipulatorCollectAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }
                         
        private:
            enum class State
            { 
                reset,              // Base state when not collecting
                waiting,            // Waiting to collect
                grabbing,           // Running the intake to ensure we complete collection
                clamp,              // Clamping the grabber down and stopping the intake
                cancel
            } ; 

            std::string toString(State st) ;

            static std::string action_name ;

            double grab_time_ ;
            double start_ ;
            State state_ ;
            double speed_ ;
            double delay_ ;
        } ;
    }
}
