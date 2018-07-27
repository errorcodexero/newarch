#pragma once

#include "Subsystem.h" 
#include <VictorSP.h>
#include <memory>

namespace xero {
    namespace phoenix {
        class Intake : public xero::base::Subsystem {
            friend class IntakeDutyCycleAction ;
        public:
            Intake(xero::base::Robot & robot);
            virtual ~Intake();

             /// \brief Perform any per robot loop action needed by the wings subsystem.
            /// This implementation just checks for any assigned action and gives the
            /// action a chance to run.
            virtual void run() ;

            /// \brief Compute the state of the wings subsystem.
            /// The state of this subsystem is found by seeing if the solenoid is ever
            /// set.  This causes the deployed state to be set to true.  Note, that
            /// reseting the solenoid after it has been set does not clear the deployed
            /// state since once the wings are deployed they can be put back.
            virtual void computeState() ;
        private:
            std::shared_ptr<frc::VictorSP> motor1_;
            std::shared_ptr<frc::VictorSP> motor2_;
        }  ;
    }
}