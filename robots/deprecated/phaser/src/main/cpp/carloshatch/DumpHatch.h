#pragma once

#include "CarlosHatchAction.h"
#include "basegroups.h"
#include "CarlosHatch.h"



/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class DumpHatch : public CarlosHatchAction {
        
        public:
            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            DumpHatch(CarlosHatch &subsystem) ;
            virtual ~DumpHatch() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            enum class State {
                ExtendArm,
                CloseFingers,
                RetractArm,
                Idle
            } ;
            
        private:
            State state_ ;
            double start_ ;
        };
    }
}
