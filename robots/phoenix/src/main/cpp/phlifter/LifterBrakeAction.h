#pragma once

#include "LifterAction.h"
#include "Lifter.h"

namespace xero
{
    namespace phoenix
    {
        class LifterBreakAction : public LifterAction
        {
        public:
            LifterBreakAction(Lifter &lifter, bool on) : LifterAction(lifter) {
                break_ = on;
            }

            virtual ~LifterBreakAction() {                
            }

            virtual void start() {
                LifterAction::start() ;

                if (break_)
                    getLifter().setBrakeOn();
                else
                    getLifter().setBrakeOff();

                setDone();
            }

            virtual void run() {
                LifterAction::run();
            }

            virtual void cancel() {
                LifterAction::cancel();
            }

            virtual std::string toString() {
                std::string ret = "LifterBrake - ";
                if (break_)
                    ret += "ON";
                else
                    ret += "OFF";

                return ret;
            }            

        private:
            bool break_;
        };
    }
}