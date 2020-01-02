#pragma once

#include "WingsAction.h"
#include "Wings.h"

namespace xero {
    namespace phoenix {
        /// \brief This action deploys the wings on the robot.
        /// See the \ref Wings class for more information.
        class WingsDeployAction : public WingsAction {
        public:
            /// \brief Create the wings deploy action
            /// \param wings a reference to the Wings subsystem
            WingsDeployAction(Wings &wings) : WingsAction(wings) { 
            }

            /// \brief Destroys a wings deploy action object
            virtual ~WingsDeployAction() {                
            }

            /// \brief called to start the action.  
            /// For this WingsDepolyAction this method does nothing
            virtual void start() {
                WingsAction::start();
            }

            /// \brief called during the robot loop to execute this action
            /// This method reaches into the Wings subsystem and sets the solenoid that
            /// deploys the wings.
            virtual void run() {
                WingsAction::run();
                Wings &wings = getWings();
                wings.solenoid_->Set(true);
            }

            /// \brief this method is called to cancel this action.
            /// This action can be canceled at any time and does not need to do anything
            /// to cancel.  If the wings are deployed already, we cannot do anything about
            /// it.  If the wings are not deployed, they will not be deployed after cancel
            /// is called.
            virtual void cancel() {
                setDone();
            }

            /// \brief return a name to this action.
            virtual std::string toString() {
                //
                // Some actions are parameterized.  For instance the drive straight action will have
                // a distance associated with the action.  Therefore the toString() method of the DriveStrightAction
                // would return "DriveStraight 100".  However this action has not parameters so this
                // method returns a reference to a static constant strings.
                return action_name ;
            }

        private:
            //
            // The name of this action to return via the toString() call.
            //
            static const std::string action_name ;
        } ;
    }
}
