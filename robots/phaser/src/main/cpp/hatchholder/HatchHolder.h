#pragma once

#include <Subsystem.h>
#include <frc/DigitalInput.h>
#include <frc/Solenoid.h>
#include <PIDCtrl.h>
#include <Robot.h>
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
            virtual void run() ;

            void autoHatchEnable(bool b) {
                auto_hatch_enabled_ = b ;
            }

            bool hasHatch() const {
                return has_hatch_ ;
            }

            void extendArm() ;
            void retractArm() ;
            void stopArm() ;            
            void extendFinger() ;
            void retractFinger() ;
            bool holdingHatch() ;

            bool isArmDeployed() {
                return arm_deployed_ ;
            }

            bool isFingerDeployed() {
                return finger_deployed_ ;
            }


        private:
            bool has_hatch_ ;
            bool arm_deployed_ ;
            bool finger_deployed_ ;
            bool auto_hatch_enabled_ ;
            std::shared_ptr<frc::DigitalInput> sensor_ ;
            std::shared_ptr<frc::Solenoid> arm_extend_ ;
            std::shared_ptr<frc::Solenoid> arm_retract_ ;
            std::shared_ptr<frc::Solenoid> finger_ ;

            bool last_switch_state_;
            bool pending_ ;
            double last_switch_time_ ;
            double duration_ ;
        } ;
    }
}
