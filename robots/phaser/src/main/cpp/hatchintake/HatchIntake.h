#pragma once

#include "singlemotorsubsystem/SingleMotorSubsystem.h"
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>
#include <iostream>



namespace xero {
    namespace phaser {
        
        class HatchIntake : public xero::base::SingleMotorSubsystem {

            friend class HatchIntakeAction ;
        public:
            HatchIntake(xero::base::Robot &robot,  uint64_t id, bool victor=true) ;
            ~HatchIntake() ;
            void deployCollector() ;
            void retractCollector() ;
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeState() ;

            bool hasHatch() const {
                return has_hatch_ ;
            }

        private:
            std::shared_ptr<frc::Solenoid> solenoid_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;

            bool has_hatch_ ;
         } ;
        
    }
}
