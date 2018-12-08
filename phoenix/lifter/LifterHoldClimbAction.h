#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class LifterHoldClimbAction : public LifterAction {
        public:
            LifterHoldClimbAction(Lifter &lifter, int value) ;
            LifterHoldClimbAction(Lifter &lifter, const std::string &name) ;
            virtual ~LifterHoldClimbAction() ;

            virtual void start()  ;
            virtual void run() ;

            virtual bool isDone() {
                return true ;
            }

            virtual void cancel() {
            }

            virtual std::string toString() ;

        private:
            int delta_ ;
            int encoder_value_ ;
        } ;
    }
}
