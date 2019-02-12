#pragma once

#include "singlemotorsubsystem/SingleMotorSubsystem.h"
#include <frc/Solenoid.h>
#include <iostream>



namespace xero {
    namespace phaser {
        
        class CargoIntake : public xero::base::SingleMotorSubsystem {

            friend class CargoIntakeAction ;
        public:
            CargoIntake(xero::base::Robot &robot,  uint64_t id, bool victor=true) ;
            ~CargoIntake();
            void deployCollector();
            void retractCollector();
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeState() ;

            bool isDeployed() const {
                return is_deployed_ ;
            }

        private:
            bool is_deployed_ ;
            std::shared_ptr<frc::Solenoid> solenoid_ ;

         } ;
        
    }
}
