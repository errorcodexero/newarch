#pragma once

#include <Robot.h>
#include "RanseurRobotSubsystem.h"
#include "RanseurSubsystemAction.h"
#include <actions/Action.h>
#include <actions/ParallelAction.h>

namespace xero {
    namespace ranseur {

        class RanseurDiagnosticAction : public xero::ranseur::RanseurSubsystemAction {
  
        public:
            RanseurDiagnosticAction(RanseurRobotSubsystem &) ;
            virtual ~RanseurDiagnosticAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return std::string("Diagnostic Action") ;
            }

        private:
            static constexpr const double blinktime = 0.5 ;
            static constexpr const double betweentime = 3.0 ;
                         
        private:
            int index_ ;
            int count_ ;
            bool on_ ;
            double start_ ;
            bool between_ ;
        };
    }
}