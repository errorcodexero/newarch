#pragma once

#include <Robot.h>
#include "RanseurRobotSubsystem.h"
#include "RanseurSubsystemAction.h"
#include <actions/Action.h>
#include <actions/ParallelAction.h>

namespace xero {
    namespace ranseur {

        class RanseurRobotTurtleAction : public xero::ranseur::RanseurSubsystemAction {
  
        public:
            RanseurRobotTurtleAction(RanseurRobotSubsystem &) ;
            virtual ~RanseurRobotTurtleAction() ;

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