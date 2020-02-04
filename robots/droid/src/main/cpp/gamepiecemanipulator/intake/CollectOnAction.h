#pragma once

#include "droidids.h"
#include "Intake.h"
#include "IntakeAction.h"
#include <actions/SequenceAction.h>

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        class CollectOnAction : public IntakeAction {   
           
        public :
            CollectOnAction(Intake &subsystem) ;
            virtual ~CollectOnAction() {}

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
