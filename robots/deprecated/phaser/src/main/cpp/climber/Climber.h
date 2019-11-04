#pragma once

#include <Subsystem.h>
#include <memory>
#include <frc/Solenoid.h>

namespace xero {
    namespace phaser {

        class Climber : public xero::base::Subsystem {
            friend class ClimberDeployAction ;

        public:
            Climber(xero::base::Robot &robot) ;

            virtual ~Climber() ;

            virtual void computeState() ;

        protected:
            virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            bool deployed_ ;

            std::shared_ptr<frc::Solenoid> solenoid_ ;
        } ;
    }
}
