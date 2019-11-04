#pragma once

#include "TurntableAction.h"
#include "Turntable.h"

namespace xero {
    namespace phaser {
        class TurntablePowerAction : public TurntableAction {
        public:
            TurntablePowerAction(Turntable &turntable, double duty_cycle) ;
            TurntablePowerAction(Turntable &turntable, const std::string &name) ;
            virtual ~TurntablePowerAction() ;

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
