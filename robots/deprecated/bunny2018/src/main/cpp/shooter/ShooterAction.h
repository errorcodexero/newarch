#pragma once

#include <Action.h>
#include "Shooter.h"

namespace xero {
    namespace bunny2018 {
        /// \brief This is the base class for actions that can be assigned to the shooter
        class ShooterAction : public xero::base::Action {
        public:
            /// \brief Create a new shooter action
            /// \param subsystem Shooter subsystem
            ShooterAction(Shooter& subsystem) : subsystem_(subsystem) {
            }

        protected:
            Shooter& getSubsystem() {
                return subsystem_ ;
            }

        private:
            /// \brief The shooter subsystem
            Shooter& subsystem_;
        };
    }
}
