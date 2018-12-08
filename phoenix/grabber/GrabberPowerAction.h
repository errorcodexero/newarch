#pragma once

#include "GrabberAction.h"
#include <vector>

namespace xero {
    namespace phoenix {
        class GrabberPowerAction : public GrabberAction {
        public:
            GrabberPowerAction(Grabber &g, double duty_cycle) ;
            GrabberPowerAction(Grabber &g, const std::string &name) ;
            virtual ~GrabberPowerAction() ;

            virtual void start() {
            }

            virtual void run()  ;

            virtual bool isDone() {
                return false ;
            }

            virtual void cancel() {
            }

            virtual std::string toString() ;

        private:
            double duty_cycle_ ;
        } ;
    }
}
