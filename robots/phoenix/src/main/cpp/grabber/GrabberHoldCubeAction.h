#pragma once

#include "GrabberAction.h"
#include <vector>

namespace xero {
    namespace phoenix {
        class GrabberHoldCubeAction : public GrabberAction {
        public:
            GrabberHoldCubeAction(Grabber &g) ;
            virtual ~GrabberHoldCubeAction() ;

            virtual void start() {
            }

            virtual void run() ;

            virtual bool isDone() {
                return true ;
            }

            virtual void cancel() {
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:
            static std::string action_name ;

        private:
            double hold_value_ ;
        } ;
    }
}
