#pragma once

#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveAction.h>
#include "RanseurCameraTracker.h"
#include <vector>

namespace xero {
    namespace ranseur {
        class DriveByVisionAction : public xero::base::TankDriveAction
        {
        public:
            DriveByVisionAction(xero::base::TankDrive &tank_drive, RanseurCameraTracker &camera) ;

            /// \brief Start the action; called once per action when it starts
            virtual void start() ;

            /// \brief Manage the action; called each time through the robot loop
            virtual void run() ;

            /// \brief Cancel the action
            virtual void cancel() ;

            /// \brief Return true if the action is complete
            /// \returns True if the action is complete
            virtual bool isDone() ;

            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString() ;  

        private:
            // The camera subsystem
            RanseurCameraTracker &camera_ ;

            // If true, this action is done
            bool is_done_ ;

            // Drive by Yaw constants
            double yaw_base_power_ ;
            double yaw_p_ ;

            int lost_count_ ;
            int max_lost_count_ ;
            double done_dist_ ;
        } ;
    }
}