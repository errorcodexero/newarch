#pragma once

#include "TurntableAction.h"
#include "Turntable.h"
#include <PIDCtrl.h>

namespace xero {
    namespace phaser {
        class TurntableHoldAction : public TurntableAction {
        public:
            TurntableHoldAction(Turntable &turntable, double target) ;
            TurntableHoldAction(Turntable &turntable, const std::string &name) ;
            virtual ~TurntableHoldAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            bool atTarget() ;

        private:
            bool is_done_ ;
            double target_ ;
            double angle_threshold_ ;
            double speed_threshold_ ;
            xero::misc::PIDCtrl pidctrl_ ;
            double output_ ;
            double start_time_ ;
        } ;
    }
}
