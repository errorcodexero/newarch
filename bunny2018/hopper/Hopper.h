#pragma once

#include "bunnysubsystem/BunnySubsystem.h" 
#include <VictorSP.h>
#include <memory>

namespace xero {
    namespace bunny2018 {

        class Hopper : public xero::base::Subsystem {
        public:

            Hopper(xero::base::Robot &robot);


            virtual ~Hopper();

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        private:
            std::shared_ptr<frc::VictorSP> motor_;

        }  ;
    }
}