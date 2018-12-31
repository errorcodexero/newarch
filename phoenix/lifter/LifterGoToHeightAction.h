#pragma once

#include "LifterAction.h"
#include "Lifter.h"
#include <PIDCtrl.h>

namespace xero {
    namespace phoenix {
        class LifterGoToHeightAction : public LifterAction {
        public:
            LifterGoToHeightAction(Lifter &lifter, double target) ;
            LifterGoToHeightAction(Lifter &lifter, const std::string &name) ;
            virtual ~LifterGoToHeightAction() ;

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
            double height_threshold_ ;
            double speed_threshold_ ;
            xero::misc::PIDCtrl pidctrl_ ;
            double output_ ;
            double start_time_ ;
        } ;
    }
}
