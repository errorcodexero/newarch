#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero {
    namespace phoenix {
        class LifterShiftAction : public LifterAction {
        public:
            LifterShiftAction(Lifter &lifter, bool high) : LifterAction(lifter) {
                high_ = high;
            }

            virtual ~LifterShiftAction() {                
            }

            virtual void start() {
                LifterAction::start() ;

                if (high_)
                    getLifter().setHighGear();
                else
                    getLifter().setLowGear();

                setDone();
            }

            virtual void run() {
                LifterAction::run();
            }

            virtual void cancel() {
                LifterAction::cancel();
            }

            virtual std::string toString() {
                std::string ret = "LifterShift - ";
                
                if (high_)
                    ret += "HIGH";
                else
                    ret += "LOW";

                return ret;
            }

        private:
            bool high_;
        };
    }
}
