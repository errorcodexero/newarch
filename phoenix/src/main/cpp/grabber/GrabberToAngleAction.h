#pragma once

#include "GrabberAction.h"
#include <PIDCtrl.h>

namespace xero {
    namespace phoenix {
        class GrabberToAngleAction : public GrabberAction {
        public:
            GrabberToAngleAction(Grabber &g, double a) ;
            GrabberToAngleAction(Grabber &g, const std::string &name) ;
            virtual ~GrabberToAngleAction() ;

            double getTarget() const {
                return target_ ;
            }

            bool isAtTargetAngle() const ;

            virtual void start() {
            }

            virtual void run() ;

            virtual bool isDone() {
                return false ;
            }

            virtual void cancel() ;

            virtual std::string toString() ;

        private:
            xero::misc::PIDCtrl pid_ctrl_ ;            
            double target_ ;
            double output_ ;
            double threshold_ ;
        } ;
    }
}
