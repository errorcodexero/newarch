#pragma once

#include "actions/Action.h"
#include "basegroups.h"
#include "CameraTracker.h"

/// \file
namespace xero {
    namespace base {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class CameraChangeAction : public Action {
        
        public:

            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            CameraChangeAction(CameraTracker &subsystem, size_t which, CameraTracker::CameraMode mode) : subsystem_(subsystem) {
                camera_ = which ;
                mode_ = mode ;
            }

            ~CameraChangeAction(){
            }

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;
            
        private:
            /// The tank drive subsystem
            CameraTracker &subsystem_;
            CameraTracker::CameraMode mode_ ;
            size_t camera_  ;
        };
    }
}
