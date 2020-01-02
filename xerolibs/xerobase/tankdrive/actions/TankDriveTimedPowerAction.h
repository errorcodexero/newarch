#pragma once

#include <tankdrive/TankDrive.h>
#include <tankdrive/actions/TankDriveAction.h>

/// \file


namespace xero {
    namespace base {
        /// \brief this class is an action that applies a fixed power to the left and right side for a fixed amount of time
        class TankDriveTimedPowerAction : public TankDriveAction {
        public:
            /// \brief create the action
            /// \param db the drivebase this action will act on
            /// \param left the power to apply to the left side
            /// \param right the power to apply to the right side
            /// \param time the amount of time to apply the power
            /// \param lowgear if true run this action in low gear            
            TankDriveTimedPowerAction(TankDrive &db, double left, double right, double time, bool lowgear = false) ;

            /// \brief create the action
            /// \param db the drivebase this action will act on
            /// \param left the name of the parameter that contains the power for the left side
            /// \param right the name of the parameter that contains the power for the right side
            /// \param time the amount of time to apply the power
            /// \param lowgear if true run this action in low gear
            TankDriveTimedPowerAction(TankDrive &db, const std::string &left, const std::string &right, const std::string &time, bool lowgear = false) ;

            /// \brief destroy the action object
            virtual ~TankDriveTimedPowerAction() ;

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
            double left_ ;
            double right_ ;
            double duration_ ;
            double start_ ;
            bool lowgear_ ;
        } ;
    }
}
