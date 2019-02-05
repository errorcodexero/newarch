#pragma once

#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveAction.h>
#include "PhaserCameraTracker.h"

namespace xero {
    namespace phaser {
        class DriveByVisionAction : public xero::base::TankDriveAction
        {
        public:
            DriveByVisionAction(xero::base::TankDrive &tank_drive, PhaserCameraTracker &camera) ;

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
                InitialDriveByYaw,
                DriveFirstSCurve,
                DriveSecondSCurve,
                FinalDriveByYaw,
                Done,
            } ;            

        private:
            void driveByYaw() ;    
            bool driveSCurve() ;
            void evaluatePosition() ;
            int computeQuadrant() ;
            std::string toString(State st) ;

        private:
            State state_ ;
            PhaserCameraTracker &camera_ ;

            // Drive by Yaw constants
            double yaw_base_power_ ;
            double yaw_p_ ;

            // Threshold for evaluation quadrant
            double yaw_threshold_ ;

            // Threshold to be in the outer ring
            double outer_dist_threshold_ ;

            // Thresholds for the quadrant based on rect ratio
            double inner_boundaries_[4] ;
            double outer_boundaries_[4] ;

            // For S curve
            double scurve_base_power_ ;
            double scurve_turn_offset_ ;
            double scurve_left_offset_ ;
            double scurve_right_offset_ ;
            int scurve_cycles_ ;
            int cycles_ ;
        } ;
    }
}