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
                if (high_)
                    getLifter().setHighGear();
                else
                    getLifter().setLowGear();
            }

            virtual void run() {                
            }

            virtual bool isDone() {
                return true ;
            }

            virtual void cancel() {
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
