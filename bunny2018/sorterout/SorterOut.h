#pragma once

#include "BunnySubsystem.h" 
#include <VictorSP.h>
#include <memory>

namespace xero {
    namespace bunny2018 {

        class SorterOut : public xero::base::Subsystem {
        public:

            SorterOut(xero::base::Robot &robot);


            virtual ~SorterOut();

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        private:
            std::shared_ptr<frc::VictorSP> motor_;

        }  ;
    }
}