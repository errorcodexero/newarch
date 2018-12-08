#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class LifterPowerAction : public LifterAction {
        public:
            LifterPowerAction(Lifter &lifter, double duty_cycle, bool lowgear = false) ;
            LifterPowerAction(Lifter &lifter, const std::string &name, bool lowgear = false) ;
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
            bool low_gear_ ;
            double duty_cycle_ ;
        } ;
    }
}
