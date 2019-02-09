#pragma once

#include "singlemotorsubsystem/SingleMotorSubsystem.h"
#include <frc/Solenoid.h>
#include <iostream>



namespace xero {
    namespace phaser {
        
        class CargoIntake : public xero::base::SingleMotorSubsystem {

            friend class CargoIntakeAction ;
        public:
            CargoIntake(xero::base::Robot &robot,  uint64_t id, bool victor=false) ;
            ~CargoIntake();
            void deployCollector();
            void retractCollector();

        private:
            std::shared_ptr<frc::Solenoid> solenoid_ ;

         } ;
        
    }
}
