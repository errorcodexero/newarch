#pragma once

#include "actions/Action.h"
#include "basegroups.h"
#include "LimeLightSubsystem.h"

/// \file
namespace xero {
    namespace base {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class LimeLightChangeModeAction : public Action {
        
        public:

            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            LimeLightChangeModeAction(LimeLightSubsystem &limelight, LimeLightSubsystem::CameraMode mode,
                LimeLightSubsystem::LEDMode led, int pipeline) : limelight_(limelight) {
                mode_ = mode ;
                led_ = led ;
                pipeline_ = pipeline ;
            }

            ~LimeLightChangeModeAction(){
            }

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
            
        private:
            /// The tank drive subsystem
            LimeLightSubsystem &limelight_;
            LimeLightSubsystem::CameraMode mode_ ;
            LimeLightSubsystem::LEDMode led_ ;
            int pipeline_ ;
        };
    }
}
