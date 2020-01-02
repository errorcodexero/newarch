#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class LifterCalibrateAction : public LifterAction {
        public:
            LifterCalibrateAction(Lifter &lifter) ;
            virtual ~LifterCalibrateAction() ;

            virtual void start() {
                LifterAction::start();
                getLifter().calibrate();
                setDone();
            }

            virtual void run() {
                LifterAction::run();
            }

            virtual void cancel() {
                setDone();
            }

            virtual std::string toString() {
                return action_name ;
            }

        private:
            static std::string action_name ;
        } ;
    }
}
