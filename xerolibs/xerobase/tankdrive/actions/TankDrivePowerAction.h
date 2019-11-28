#pragma once

#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveAction.h>

/// \file


namespace xero {
    namespace base {
        /// \brief This action supplies power to the left and right sides of the drive base
        class TankDrivePowerAction : public TankDriveAction {
        public:
            /// \brief this action applies power to the left and right side of the robot
            /// \param db the drivebase this action applies to
            /// \param left the power to apply to the left side
            /// \param right the power to apply to the right 
            /// \param highgear if true run this action in high gear               
            TankDrivePowerAction(TankDrive &db, double left, double right, bool highgear=true) ;

            /// \brief create the action
            /// \param db the drivebase this action will act on
            /// \param left the name of the parameter that contains the power for the left side
            /// \param right the name of the parameter that contains the power for the right side
            /// \param highgear if true run this action in high gear            
            TankDrivePowerAction(TankDrive &db, const std::string &left, const std::string &right, bool highgear=true) ;

            /// \brief destroy the action object
            virtual ~TankDrivePowerAction() ;

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
            double left_ ;
            double right_ ;
            bool highgear_ ;
        } ;
    }
}