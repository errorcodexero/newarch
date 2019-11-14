#pragma once

#include <actions/Action.h>
#include <actions/DispatchAction.h>
#include <actions/ParallelAction.h>

namespace xero {
    namespace ranseur {
        class TubManipulatorSubsystem ;

        class TubManipulatorAction : public xero::base::Action {

        public:
            // type name NewAction() {
            // action_ = parallelAction(DispatchAction()),
            //     DispatchAction() ;
            // }

            virtual void start() { 
                action_.start();
            }
            virtual void run() {
                action_.run();
            }
            virtual void cancel() {
                action_.cancel();
            }
            virtual bool isDone() {
                return action_.isDone();
            }

            TubManipulatorAction(TubManipulatorSubsystem &tubmanipulatorsubsystem) : tubmanipulator_(tubmanipulatorsubsystem) {                
            }

            virtual ~TubManipulatorAction() {                
            }

            TubManipulatorSubsystem &getTubManipulator() {
                return tubmanipulator_ ;
            }      

        private:
            xero::base::ParallelAction action_ ;    
            TubManipulatorSubsystem &tubmanipulator_ ;
        };
    }
}
