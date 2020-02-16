#pragma once

#include "droidids.h"
#include "Intake.h"
#include "IntakeAction.h"
#include <actions/SequenceAction.h>
#include <motorencodersubsystem/MotorEncoderGoToAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        class CollectOnAction : public xero::base::MotorEncoderGoToAction {   
           
        public :
            CollectOnAction(Intake &subsystem) ;
            virtual ~CollectOnAction() {}

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }

            static constexpr const char *Target = "intake:arm:collecton:pos" ;
                         
        private :
            static std::string action_name ;
            double collector_power_;
        } ;
    }
}
