#pragma once

#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveAction.h>
#include "PhaserCameraTracker.h"

namespace xero {
    namespace phaser {
        class DriveByVisionHier2Action : public xero::base::TankDriveAction
        {
        public:
            DriveByVisionHier2Action(xero::base::TankDrive &tank_drive, PhaserCameraTracker &camera) ;

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
            void calcCurvePath(double dist, double rect) ;                      

        private:
            enum class State {
                TrackingYaw,
                FirstCurve,
                DrivingStraight,
                SecondCurve,
                Done,
            } ;

        private:
            PhaserCameraTracker &camera_ ;
            double yaw_adjust_power_ ;
            double base_power_ ;
            double yaw_switch_threshold_ ;
            double yaw_max_threshold_ ;
            double turn_offset_ ;
            double rect_threshold_ ;
            double fast_factor_ ;
            bool is_done_ ;
            double left_offset_ ;
            double right_offset_ ;
            int cycles_ ;
            int turn_cycles_ ;
            int this_turn_cycles_ ;
            int turn_straight_ ;
            State state_ ;
            bool fast_ ;
        } ;
    }
}