#pragma once

#include "Action.h"
#include "SwerveDriveBase.h"

/// \file


namespace xero {
    namespace base {
        class SwerveDriveModuleTestAction : public Action {
        public:
            SwerveDriveModuleTestAction(SwerveDriveBase &db, double dur, double rotate, double drive) ;

            /// \brief destroy the action
            virtual ~SwerveDriveModuleTestAction() ;

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
            SwerveDriveBase &db_ ;
            bool running_ ;
            double start_time_ ;
            double duration_ ;
            double rotate_power_ ;
            double drive_power_ ;
        } ;
    }
}

