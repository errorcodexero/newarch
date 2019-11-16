#pragma once

#include <actions/Action.h>
#include <actions/DispatchAction.h>
#include <actions/ParallelAction.h>

namespace xero {
    namespace ranseur {
        class RanseurRobotSubsystem ;

        class RanseurSubsystemAction : public xero::base::Action {

        public:

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

            RanseurSubsystemAction(RanseurRobotSubsystem &ranseurrobotsubsystem) : subsystem_(ranseurrobotsubsystem) {
            }

            virtual ~RanseurSubsystemAction() {                
            }

            RanseurRobotSubsystem &getRanseurSubsystem() {
                return subsystem_ ;
            }      

        private:
            xero::base::ParallelAction action_ ;    
            RanseurRobotSubsystem &subsystem_ ;
        };
    }
}
