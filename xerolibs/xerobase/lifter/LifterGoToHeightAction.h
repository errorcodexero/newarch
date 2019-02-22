#pragma once

#include "LifterAction.h"
#include "Lifter.h"
#include <PIDACtrl.h>
#include <PIDCtrl.h>
#include <TrapezoidalProfile.h>

namespace xero {
    namespace base {
        class LifterGoToHeightAction : public LifterAction {
        public:
            LifterGoToHeightAction(Lifter &lifter, double target, bool relative = false) ;
            LifterGoToHeightAction(Lifter &lifter, const std::string &name, bool relative = false) ;
            virtual ~LifterGoToHeightAction() ;

            virtual void start() ;
            virtual void run() ;
            virtual bool isDone() ;
            virtual void cancel() ;
            virtual std::string toString() ;

        private:
            bool is_done_ ;
            double target_ ;
            double threshold_ ;
            double offset_ ;
            std::shared_ptr<xero::misc::PIDACtrl> ctrl_ ;
            std::shared_ptr<xero::misc::TrapezoidalProfile> profile_ ;
            xero::misc::PIDCtrl pid_ctrl_ ;            
            double start_time_ ;
            double start_height_ ;
            size_t index_ ;

            int plotid_ ;
            static std::list<std::string> plot_columns_ ; 
            bool relative_ ;               
        } ;
    }
}
