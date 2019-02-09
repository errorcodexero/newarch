#pragma once

#include "LifterAction.h"
#include "Lifter.h"
#include <PIDCtrl.h>

namespace xero {
    namespace base {
        class LifterHoldHeightAction : public LifterAction {
        public:
            LifterHoldHeightAction(Lifter &lifter, double target) ;
            LifterHoldHeightAction(Lifter &lifter, const std::string &name) ;
            virtual ~LifterHoldHeightAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:

        private:
            double target_ ;
            double threshold_ ;
            double start_ ;
            bool is_done_ ;
            xero::misc::PIDCtrl pid_ctrl_ ;

            int plotid_ ;
            static std::list<std::string> plot_columns_ ;
            size_t index_ ;
        } ;
    }
}
