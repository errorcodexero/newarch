#include "Wings.h"
#include "WingsAction.h"
#include <Robot.h>

using namespace xero::base ;

namespace xero {
    namespace phoenix {
        //
        // Create the wings subsystem object
        //
        Wings::Wings(xero::base::Robot &robot) : xero::base::Subsystem(robot, "wings") {
            // int sol = robot.getParams().getValue("hw:wings:solenoid").getInteger() ;

            int sol = 1 ;       // Replace with line above when we get the param parser updated

            //
            // Create the hardware object for the solenoid, the frc::Solenoid object is from the
            // FRC WPIlib library.
            //
            solenoid_ = std::make_shared<frc::Solenoid>(sol) ;

            //
            // The wings are initially not deployed
            //
            deployed_ = false ;
        }

        Wings::~Wings() {
        }

        void Wings::computeState() {
            //
            // If we ever see the solenoid set, we mark the wings as deployed.  In general
            // the code here should look at any of the robot subsystem hardware to compute
            // any state that might be needed by an action, other robot subsystem, or by
            // the robot class in general.  For example, if we had blinky lights, we could
            // use this state to indicate the state of the wings for the blinky lights.
            //
            if (solenoid_->Get())
                deployed_ = true ;
        }

        bool Wings::canAcceptAction(xero::base::ActionPtr action) {
            //
            // Try to cast the action given to the WingsAction type.  If action is nullptr, or action
            // is of some type that is not derived from WingsAction, the return value will be nullptr.
            // There if the return value from the dynamic_pointer_cast is nullptr, we disallow this action
            // from being assigned.
            //
            std::shared_ptr<WingsAction> action_p = std::dynamic_pointer_cast<WingsAction>(action) ;
            return action_p != nullptr ;
        }

        void Wings::run() {
            //
            // This does nothing for now for a simple subsystem.  If this is a more complex
            // subsystem, this method calls run() on each of the child subsystems.  It should always
            // be called incase we add more to the run method of the base class Subsystem.
            //
            Subsystem::run() ;

            //
            // Get the currently assigned action for this subsytem, and if it exists (is not nullptr)
            // the run the action for this subsystem.
            //
            ActionPtr action = getAction() ;
            if (action != nullptr)
                action->run() ;
        }
    }
}