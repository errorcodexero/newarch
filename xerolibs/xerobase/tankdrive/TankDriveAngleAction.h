#pragma once

#include "TankDriveAction.h"
#include "TankDrive.h"
#include "PIDCtrl.h"
#include "TrapezoidalProfile.h"



/// \file

namespace xero {
    namespace base {
        /// This class represents a rotate action rotating a specific angle
        class TankDriveAngleAction : public TankDriveAction {
        public:
            /// \brief Create action to rotate the robot a specific angle
            /// \param db the tankdrive subsystem to rotate
            /// \param angle the angle of rotate for the robot
            TankDriveAngleAction(TankDrive &db, double angle) ;

            /// \brief Create action to rotate the robot a specific angle
            /// \param db the tankdrive subsystem to rotate
            /// \param name the name of the parameter for the angle to rotate the robot
            TankDriveAngleAction(TankDrive &db, const std::string &name) ;          

            /// \brief destroy the action to rotate the robot
            virtual ~TankDriveAngleAction() ;

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
            xero::misc::PIDCtrl velocity_pid_ ;
            std::shared_ptr<xero::misc::TrapezoidalProfile> profile_;
            double start_time_ ;
            double profile_start_time_; 
            double profile_initial_angle_;
            double angle_threshold_;
            double target_angle_;
            double profile_outdated_error_long_;
            double profile_outdated_error_short_ ;
            double profile_outdated_error_angle_ ;
            double total_angle_so_far_ ;
            bool is_done_;
        } ;
    }
}

