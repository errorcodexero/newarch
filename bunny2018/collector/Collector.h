#pragma once

#include "BunnySubsystem.h" 
#include <VictorSP.h>
#include <memory>

namespace xero {
    namespace bunny2018 {

        class Collector : public xero::base::Subsystem {
        public:

            Collector(xero::base::Robot &robot);


            virtual ~Collector();

            virtual bool canAcceptAction(xero::base::ActionPtr action) ;

        private:
            std::shared_ptr<frc::VictorSP> motor_;

        }  ;
    }
}