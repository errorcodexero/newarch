#pragma once

#include "Action.h"
#include "basegroups.h"
#include "CarlosHatch.h"

/// \file
using namespace xero::base ;

namespace xero {
    namespace phaser {

        /// \brief This is the base class for all actions that can be assigned to the SingleMotorSubsystem
        /// \sa SingleMotorSubsystem
        class CarlosHatchAction : public Action {
        
        public:

            enum class Operation {
                EXTEND,
                RETRACT,
            };

            /// \brief Create a new SingleMotorSubsystemAction
            /// \param subsystem SingleMotor subsystem
            CarlosHatchAction(CarlosHatch &subsystem) : subsystem_(subsystem) {
            }

            virtual ~CarlosHatchAction(){
            }

        protected:
            CarlosHatch &getSubsystem() {
                return subsystem_ ;
            }
            
        private:
            /// The tank drive subsystem
            CarlosHatch &subsystem_;
        };
    }
}
