#pragma once

#include "ControllerBase.h"


/// \file


namespace xero {
    namespace base {
        /// \brief base class for the operator mode controller
        class TeleopController : public ControllerBase {
        public:
            /// \brief create a new controller
            /// \param robot the robot this controller will control
            TeleopController(Robot &robot) ;

            /// \brief destroy the existing controller
            virtual ~TeleopController() ;

            virtual void run() ;

        private:
        } ;
    }
}