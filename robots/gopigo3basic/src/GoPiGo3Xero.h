#pragma once

#include <Robot.h>

namespace xero {
    namespace gopigo3 {

        class GoPiGo3Subsystem ;

        /// \brief the concrete class that represents the robot Phoenix
        class GoPiGo3Xero : public xero::base::Robot {
        public:

            GoPiGo3Xero() ;

            void enableSpecificMessages() ;
            void RobotHardwareInit() ;

            /// \brief called to create the autonomous controller
            /// \returns a robot controller
            virtual std::shared_ptr<xero::base::ControllerBase> createAutoController() ;
            
            /// \brief called to create the teleop controller
            /// \returns a robot controller
            virtual std::shared_ptr<xero::base::ControllerBase> createTeleopController() ;

            /// \brief called to create the test controller
            /// \returns a robot controller
            virtual std::shared_ptr<xero::base::ControllerBase> createTestController() ;

            /// \brief return the robot subsystem for the robot
            std::shared_ptr<GoPiGo3Subsystem> getGoPiGoSubsystem() ;

            virtual void loadPaths() ;

            virtual bool isCompBot() {
                return true ;
            }

        private:
        } ;
    }
}
