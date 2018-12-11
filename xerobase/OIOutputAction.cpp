#include "OIOutputAction.h"
#include <DriverStation.h>
#include <Joystick.h>

namespace xero {
    namespace base {
        OIOutputAction::OIOutputAction(int hid, int output, int state) {
            hid_ = hid ;
            output_ = output ;
            state_ = state ;
        }

        OIOutputAction::~OIOutputAction() {
        }

        /// \brief Start the action; called once per action when it starts
        void OIOutputAction::start() {
            frc::Joystick joy(hid_) ;
            joy.SetOutput(output_, state_ ? true : false) ;
        }

        /// \brief Manage the action; called each time through the robot loop
        void OIOutputAction::run() {
        }

        /// \brief Cancel the action
        void OIOutputAction::cancel() {
        }

        /// \brief Return true if the action is complete
        /// \returns True if the action is complete
        bool OIOutputAction::isDone() {
            return true ;
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable string representing the action
        std::string OIOutputAction::toString() {
            std::string ret = "OIOutputAction " ;
            ret += ", hid " + std::to_string(hid_) ;
            ret += ", output " + std::to_string(output_) ;
            ret += ", state " + std::to_string(state_) ;                        

            return ret ;
        }
    }
}