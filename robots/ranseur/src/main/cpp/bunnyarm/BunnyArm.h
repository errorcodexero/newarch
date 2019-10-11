#pragma once

#include <Subsystem.h>
#include <memory>
#include <frc/Solenoid.h>

namespace xero {
    namespace phaser {

        class BunnyArm : public xero::base::Subsystem {
            friend class BunnyArmDeployAction ;

        public:
            BunnyArm(xero::base::Robot &robot) ;

            virtual ~BunnyArm() ;

            virtual void computeState() ;

        protected:
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            bool deployed_ ;

            std::shared_ptr<frc::Solenoid> solenoid_ ;
        } ;
    }
}
