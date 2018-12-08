#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class LifterLowGearAction : public LifterAction {
        public:
            LifterLowGearAction(Lifter &lifter) ;

            virtual void start() {
                getLifter().setLowGear() ;
            }

            virtual void run() {                
            }

            virtual bool isDone() {
                return true ;
            }

            virtual void cancel() {
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:    
            static std::string action_name ;
        } ;
    }
}
