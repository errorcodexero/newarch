#pragma once

#include <Subsystem.h>
#include <memory>
#include <Solenoid.h>

namespace xero {
    namespace phoenix {
        class Wings : public xero::base::Subsystem {
            friend class WingsDeployAction ;
        public:
            Wings(xero::base::Robot &robot) ;
            virtual ~Wings() ;

            virtual void run() ;
            virtual void computeState() ;

        protected:
			virtual bool canAcceptAction(xero::base::ActionPtr Action) ;

        private:
            bool deployed_ ;
            std::shared_ptr<frc::Solenoid> solenoid_ ;
        } ;
    }
}