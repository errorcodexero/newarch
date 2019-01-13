#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace base {
        class LifterPowerAction : public LifterAction {
        public:
            LifterPowerAction(Lifter &lifter, double duty_cycle) ;
            LifterPowerAction(Lifter &lifter, const std::string &name) ;
            virtual ~LifterPowerAction() ;

            virtual void start() ;

            virtual void run() ;

            virtual bool isDone() {
                return true ;
            }

            virtual void cancel() {
            }

            virtual std::string toString() ;

        private:
            double duty_cycle_ ;
        } ;
    }
}
