#pragma once

#include "droidids.h"
#include "Intake.h"
#include "IntakeAction.h"
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        class CollectOffAction : public xero::base::MotorEncoderGoToAction {   
        public :
            CollectOffAction(Intake &subsystem) ;
            virtual ~CollectOffAction() {}

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }

            static constexpr const char *Target = "intake:arm:collectoff:pos" ;            
                         
        private :
            static std::string action_name ;
        } ;
    }
}
