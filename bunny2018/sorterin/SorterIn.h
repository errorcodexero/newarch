#pragma once

#include "BunnySubsystem.h" 
#include <VictorSP.h>
#include <memory>

namespace xero {
    namespace bunny2018 {

        class SorterIn : public xero::base::Subsystem {
        public:

            SorterIn(xero::base::Robot &robot);


            virtual ~SorterIn();

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        private:
            std::shared_ptr<frc::VictorSP> motor_;

        }  ;
    }
}