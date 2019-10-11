#pragma once

#include "Subsystem.h" 
#include <frc/VictorSP.h>
#include <memory>

namespace xero {
    namespace phoenix {
        /// \brief This class is the intake class.  
        /// The Intake consits of a pair of motors that drives two belts, on on
        /// each side of the grabber.  These belts are used to either collect
        /// Tubs into the grabber or eject Tubs to a desired destination.
        class Intake : public xero::base::Subsystem {
            friend class IntakeDutyCycleAction ;
        public:
            /// \brief create a new Intake subsystem
            /// \param robot the robot this Intak subsystem belongs to
            Intake(xero::base::Robot &robot);

            /// \brief destroy the intake subsystem
            virtual ~Intake();

            /// \brief returns true if this action is valid for this subsystem
            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

            void createNamedSequences() {
            }

        private:
            /// The first motor
            std::shared_ptr<xero::base::MotorController> motorIntake;

        }  ;
    }
}
