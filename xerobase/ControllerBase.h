#pragma once

#include <string>

/// \file


namespace xero {
    namespace base {
        class Robot ;
        
        /// \brief This is the base class for all robot controllers.
        /// See Robot for more information about controllers
        class ControllerBase {
        public:
            /// \brief create a new controller base
            /// \param robot the robot this controller controls
            ControllerBase(Robot &robot) ;

            /// \brief destroy the robot controller
            virtual ~ControllerBase() ;

            /// \brief run the controller, called once per robot loop.
            /// This method is abstract and will be defined by a derived controller
            /// class.
            virtual void run() = 0 ;

        protected:
            /// \brief Returns a reference to the robot this robot is controlling
            /// \returns a reference to a robot
            Robot &getRobot() {
                return robot_ ;
            }

        private:
            Robot &robot_ ;
        } ;
    }
}
