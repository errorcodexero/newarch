#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace base {
        class LifterCalibrateAction : public LifterAction {
        public:
            LifterCalibrateAction(Lifter &lifter) ;
            virtual ~LifterCalibrateAction() ;

            virtual void start() {
                getLifter().calibrate() ;
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
