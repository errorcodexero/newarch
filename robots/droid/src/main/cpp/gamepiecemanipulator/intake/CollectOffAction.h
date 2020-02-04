#pragma once

#include "droidids.h"
#include "Intake.h"
#include "motorencodersubsystem/MotorEncoderGoToAction.h"
#include <actions/SequenceAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        class CollectOffAction : public xero::base::MotorEncoderGoToAction {   
           
        public :
            CollectOffAction(Intake &subsystem) ;
            virtual ~CollectOffAction() {}

            Intake &getSubsystem() {
                return dynamic_cast<Intake&>(xero::base::MotorEncoderGoToAction::getSubsystem()) ;
            }

            virtual void start() ;
            virtual void run() ;
            virtual void cancel() ;
            virtual std::string toString() {
                return action_name ;
            }
                         
        private :
            static std::string action_name ;
            xero::base::SequenceAction sequence_ ;
            double collector_power_;

        } ;
    }
}
