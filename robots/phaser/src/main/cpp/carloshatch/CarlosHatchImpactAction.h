#pragma once

#include "Action.h"
#include "basegroups.h"
#include "CarlosHatchAction.h"

/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class CarlosHatchImpactAction : public CarlosHatchAction {
        public:
            CarlosHatchImpactAction(CarlosHatch &subsystem) ;
            virtual ~CarlosHatchImpactAction()  ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State
            {
                Idle,
                WaitForSwitch,
                WaitForImpact,
                WaitForHooks
            } ;
            
        private:
            bool collecting_ ;
            State state_ ;
            double start_ ;
        };
    }
}
