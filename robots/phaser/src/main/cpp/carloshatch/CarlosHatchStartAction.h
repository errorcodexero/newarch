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
        class CarlosHatchStartAction : public CarlosHatchAction {
        public:
            CarlosHatchStartAction(CarlosHatch &subsystem) ;
            virtual ~CarlosHatchStartAction()  ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            bool waiting_ ;
        };
    }
}
