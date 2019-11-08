#pragma once

#include <actions/Action.h>
#include <actions/DispatchAction.h>
#include <actions/ParallelAction.h>

namespace xero {
    namespace ranseur {
        class TubManipulator ; 

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

            TubManipulatorAction(TubManipulator &tubmanipulator) : tubmanipulator_(tubmanipulator) {                
            }

            virtual ~TubManipulatorAction() {                
            }

            TubManipulator &getTubManipulator() {
                return tubmanipulator_ ;
            }      

        private:
            xero::base::ParallelAction action_ ;    
            TubManipulator &tubmanipulator_ ;
        };
    }
}
