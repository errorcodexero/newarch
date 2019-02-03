#pragma once

#include "TankDriveAction.h"
#include "PIDACtrl.h"
#include <XeroPath.h>
/// \file


namespace xero {
    namespace base {
        /// \brief This action supplies power to the left and right sides of the drive base
        class TankDriveFollowPathAction : public TankDriveAction {
        public:
            /// \brief this action applies power to the left and right side of the robot
            /// \param db the drivebase this action applies to
            /// \param path the name of the path to follow
            TankDriveFollowPathAction(TankDrive &db, const std::string &path) ;

            /// \brief destroy the action object
            virtual ~TankDriveFollowPathAction() ;

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
            size_t index_ ;
            double left_start_ ;
            double right_start_ ;
            double start_time_ ;
            double turn_correction_ ;
            double angle_error_ ;
            double angle_decay_ ;
            std::shared_ptr<xero::misc::XeroPath> path_ ;
            std::shared_ptr<xero::misc::PIDACtrl> left_follower_ ;
            std::shared_ptr<xero::misc::PIDACtrl> right_follower_ ;

            int plotid_ ;
            static std::list<std::string> plot_columns_ ;            
        } ;
    }
}