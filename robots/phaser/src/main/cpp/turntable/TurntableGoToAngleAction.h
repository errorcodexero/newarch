#pragma once

#include "TurntableAction.h"
#include "Turntable.h"
#include <PIDACtrl.h>
#include <PIDCtrl.h>
#include <TrapezoidalProfile.h>
#include <vector>

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

            double getTargetAngle() const {
                return target_ ;
            }

        private:
            double getAngleDifference(double start, double end) ;

        private:
            bool is_done_ ;
            bool lost_encoders_ ;
            double target_ ;
            double output_ ;
            double start_time_ ;
            double last_pos_ ;

            double min_keepout_angle_ ;
            double max_keepout_angle_ ;
            
            double threshold_ ;
            std::shared_ptr<xero::misc::PIDACtrl> ctrl_ ;
            xero::misc::PIDCtrl pidctrl_ ;            
            std::shared_ptr<xero::misc::TrapezoidalProfile> profile_ ;
            double start_angle_ ;

            static std::vector<std::string> plot_columns_ ;  
        } ;
    }
}
