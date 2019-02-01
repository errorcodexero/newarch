#pragma once

#include <Subsystem.h>
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>
#include <PIDCtrl.h>
#include <limits>
#include <vector>

namespace xero {
    namespace phaser {
        class HatchHolder : public xero::base::Subsystem {
        public:
            HatchHolder(xero::base::Robot &robot) ;
            virtual ~HatchHolder() ;
            
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;
            virtual void computeState() ;

            bool hasHatch() const {
                return has_hatch_ ;
            }

            void extendArm() ;
            void retractArm() ;
            void extendFinger() ;
            void retractFinger() ;
            bool holdingHatch() ;


        private:
            bool has_hatch_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;
            std::shared_ptr<frc::Solenoid> arm_ ;
            std::shared_ptr<frc::Solenoid> finger_ ;
        } ;
    }
}
