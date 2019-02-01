#pragma once

#include "TurntableAction.h"
#include "Turntable.h"
#include <PIDCtrl.h>

namespace xero {
    namespace phaser {
        class TurntableGoToAngleAction : public TurntableAction {
        public:
            TurntableGoToAngleAction(Turntable &turntable, double target) ;
            TurntableGoToAngleAction(Turntable &turntable, const std::string &name) ;
            virtual ~TurntableGoToAngleAction() ;

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
