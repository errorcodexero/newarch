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
            enum class State {
                DriveYaw,
                Tracking,
                Done,
            } ;            

        private:
            void driveByYaw() ;    
            void driveTracking() ;
            std::string toString(State st) ;

        private:
            State state_ ;
            RanseurCameraTracker &camera_ ;

            // Drive by Yaw constants
            double yaw_base_power_ ;
            double yaw_p_ ;

            int lost_count_ ;

            double start_ ;
            int plot_id_ ;
            static std::vector<std::string> plot_columns_ ;             
        } ;
    }
}