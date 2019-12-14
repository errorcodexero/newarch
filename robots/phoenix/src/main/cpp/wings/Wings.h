#pragma once

#include <Subsystem.h>
#include <memory>
#include <frc/Solenoid.h>

namespace xero {
    namespace phoenix {
        /// \brief this is the wings subsystem
        /// The wings subsystem controls a single solenoid that when activated
        /// remove a pin holding the wings into the side of the robot.  This deploys
        /// the wings out beside the robot, allow other robots to climb on top of the
        /// wings and get lifted when our robot is lifted.  The actual hardware controlled
        /// is a single solenoid that is activated to deploy the wings.<br>
        /// See WingsAction<br>
        /// See WingsDeployAction
        class Wings : public xero::base::Subsystem {
            //
            // The list of action that can act on the subsystem hardware and therefore
            // needs more access than we give any other classes in the system.
            //
            friend class WingsDeployAction ;

        public:
            /// \brief create the wings subsystem
            /// \param robot the base robot class
            Wings(xero::base::Subsystem *parent) ;

            /// \brief destroy the wings subsystem
            virtual ~Wings() ;

            /// \brief Compute the state of the wings subsystem.
            /// The state of this subsystem is found by seeing if the solenoid is ever
            /// set.  This causes the deployed state to be set to true.  Note, that
            /// reseting the solenoid after it has been set does not clear the deployed
            /// state since once the wings are deployed they can be put back.
            virtual void computeState() ;

        protected:
            /// \brief Determine if the Wings subsystem can accept the given action.
            /// For this subsystem the only critera is that the action be derived from
            /// the WingsAction type.
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            //
            // Set to true if the wings are ever deployed
            //
            bool deployed_ ;

            //
            // The solenoid that controls the wings
            //
            std::shared_ptr<frc::Solenoid> solenoid_ ;
        } ;
    }
}
