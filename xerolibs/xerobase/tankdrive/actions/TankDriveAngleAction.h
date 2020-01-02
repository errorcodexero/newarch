#pragma once

#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveAction.h>
#include <PIDACtrl.h>
#include "TrapezoidalProfile.h"
#include <vector>
#include <string>


/// \file

namespace xero {
    namespace base {
        /// This class represents a rotate action rotating a specific angle
        class TankDriveAngleAction : public TankDriveAction {
        public:
            /// \brief Create action to rotate the robot a specific angle
            /// \param db the tankdrive subsystem to rotate
            /// \param angle the angle of rotate for the robot
            TankDriveAngleAction(TankDrive &db, double angle, bool relative = false) ;

            /// \brief Create action to rotate the robot a specific angle
            /// \param db the tankdrive subsystem to rotate
            /// \param name the name of the parameter for the angle to rotate the robot
            TankDriveAngleAction(TankDrive &db, const std::string &name, bool relative = false);

            /// \brief destroy the action to rotate the robot
            virtual ~TankDriveAngleAction() ;

            /// \brief Start the action; called once per action when it starts
            virtual void start() ;

            /// \brief Manage the action; called each time through the robot loop
            virtual void run() ;

            /// \brief Cancel the action
            virtual void cancel() ;

            /// \brief return a human readable string representing the action
            /// \returns a human readable string representing the action
            virtual std::string toString() ;

        private:
            std::shared_ptr<xero::misc::PIDACtrl> velocity_pid_ ;
            std::shared_ptr<xero::misc::TrapezoidalProfile> profile_;
            double start_time_ ;
            double start_angle_;
            double angle_threshold_;
            double target_angle_;
            double total_angle_so_far_ ;
            bool relative_;

            int plot_id_;
            static std::vector<std::string> columns_;
        } ;
    }
}

