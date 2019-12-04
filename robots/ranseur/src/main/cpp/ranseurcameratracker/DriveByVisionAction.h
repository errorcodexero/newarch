#pragma once

#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveAction.h>
#include "RanseurLimeLight.h"
#include <TrapezoidalProfile.h>
#include <PIDACtrl.h>
#include <vector>

namespace xero {
    namespace ranseur {
        class DriveByVisionAction : public xero::base::TankDriveAction
        {
        public:
            DriveByVisionAction(xero::base::TankDrive &tank_drive, RanseurLimeLight &camera) ;

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
            
            RanseurLimeLight &camera_ ;

            std::shared_ptr<xero::misc::TrapezoidalProfile> profile_ ;
            std::shared_ptr<xero::misc::PIDACtrl> follower_ ;
            int plotid_ ;

            double yaw_p_ ;

            int lost_count_ ;
            bool is_done_ ;       

            double profile_start_time_ ;
            double profile_start_dist_ ;

            static std::vector<std::string> cols_ ;  
        } ;
    }
}