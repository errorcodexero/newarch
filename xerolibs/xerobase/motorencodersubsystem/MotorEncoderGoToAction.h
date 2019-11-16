#pragma once

#include <PIDACtrl.h>
#include <PIDCtrl.h>
#include <TrapezoidalProfile.h>

#include "MotorEncoderSubsystemAction.h"
#include "MotorEncoderSubsystem.h"

#include <string>
#include <memory>


namespace xero {
    namespace base {
        class MotorEncoderGoToAction : public MotorEncoderSubsystemAction {
        public:
            MotorEncoderGoToAction(MotorEncoderSubsystem &subsystem, double target);

            virtual void start();
            virtual void run();
            virtual bool isDone() {
                return false ; ///temporary
            }
            virtual void cancel() {

            }
            virtual std::string toString() {
                return " " ;
            }

        private:
            bool isDone_ ;
            double threshold_;
            double target_ ;            
            double startTime_ ;
            double startPosition_ ;
            std::shared_ptr<xero::misc::PIDACtrl> ctrl_ ;
            std::shared_ptr<xero::misc::TrapezoidalProfile> profile_ ;
        };
    } 
}