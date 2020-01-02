#pragma once

#include "wings/Wings.h"
#include <actions/Action.h>
#include <Robot.h>

namespace xero {
    namespace phoenix {
        class Wings ;

        /// \brief This is the base class for all actions that can operate on the Wings subsystem.
        /// This class serves two purposes.  First, the type is the only type of action class that can
        /// be assigned to the Wings subsystem.  This class gives is a runtime check of the validity
        /// of an action to be assigned to a subsystem.  Second, it stores a reference to the actual
        /// Wings subsystem so that derived classes has access to the subsystem object (i.e. the Wings
        /// objects).
        class WingsAction : public xero::base::GenericAction {
        public:
            /// \brief Create the WingAction object
            /// \param wings a reference to the wings subsystem object
            WingsAction(Wings &wings) : GenericAction(wings.getRobot().getMessageLogger()), wings_(wings) {                
            }
            
            /// \brief Return the wings subsystem object associated with this action
            /// \returns the wings subsystem object
            Wings &getWings() {
                return wings_ ;
            }

        private:
            //
            // A reference to the wings subsystem object.  A simple reference here is
            // good as subsystems always are around as long as the robot object is around
            // which is as long as the entire system is running.
            //
            Wings &wings_ ;
        } ;
    }
}
