#pragma once

#include <Subsystem.h>
#include <memory>
#include <frc/Solenoid.h>

namespace xero {
    namespace ranseur {

        class BunnyArm : public xero::base::Subsystem {
            friend class BunnyArmDeployAction ;

        public:
            BunnyArm(xero::base::Subsystem *parent) ;

            virtual ~BunnyArm() ;

            virtual void computeState() ;

            bool isRaised() {
                return raised_ ;
            }

            void raise() {
                raised_ = true ;
                solenoid_->Set(false) ;
            }

            void lower() {
                raised_ = false ;
                solenoid_->Set(true) ;
            }

        protected:
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            bool raised_ ;
            std::shared_ptr<frc::Solenoid> solenoid_ ;
        } ;
    }
}
