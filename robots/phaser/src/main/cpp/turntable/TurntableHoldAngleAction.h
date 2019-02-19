#pragma once

#include "TurntableAction.h"
#include "Turntable.h"
#include <PIDCtrl.h>

namespace xero {
    namespace phaser {
        class TurntableHoldAngleAction : public TurntableAction {
        public:
            TurntableHoldAngleAction(Turntable &turntable, double target) ;
            TurntableHoldAngleAction(Turntable &turntable, const std::string &name) ;
            virtual ~TurntableHoldAngleAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            bool is_done_ ;
            double target_ ;
            xero::misc::PIDCtrl pidctrl_ ;
            double output_ ;
            double start_time_ ;
        } ;
    }
}
