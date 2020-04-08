#pragma once

#include "singlemotorsubsystem/SingleMotorSubsystem.h"
#include <frc/Solenoid.h>
#include <frc/DigitalInput.h>
#include <iostream>

namespace xero {
    namespace phaser {
        
        class CargoIntake : public xero::base::SingleMotorSubsystem {

            friend class CargoIntakeAction ;
        public:
            CargoIntake(xero::base::Robot &robot,  uint64_t id) ;
            ~CargoIntake();
            void deployCollector();
            void retractCollector();
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeMyState() ;

            bool isDeployed() const {
                return is_deployed_ ;
            }

            bool hasCargo() const {
                return has_cargo_ ;
            }

        private:
            bool is_deployed_ ;
            bool has_cargo_ ;
            std::shared_ptr<frc::Solenoid> solenoid_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;
         } ;
        
    }
}
