#pragma once

#include <PIDACtrl.h>
#include <PIDCtrl.h>
#include <TrapezoidalProfile.h>
#include <xeromath.h>

#include "MotorEncoderSubsystemAction.h"
#include "MotorEncoderSubsystem.h"

#include <string>
#include <memory>


namespace xero {
    namespace base {
        class MotorEncoderGoToAction : public MotorEncoderSubsystemAction {
        public:
            MotorEncoderGoToAction(MotorEncoderSubsystem &subsystem, double target, bool addhold = true);
            MotorEncoderGoToAction(MotorEncoderSubsystem &subsystem, const std::string &param, bool addhold = true) ;

            virtual void start();
            virtual void run();
            virtual void cancel();
            virtual std::string toString() ;

        private:
            double threshold_;
            double target_ ;
            double startTime_ ;
            double startPosition_ ;
            std::shared_ptr<xero::misc::PIDACtrl> ctrl_ ;
            std::shared_ptr<xero::misc::TrapezoidalProfile> profile_ ;

            bool addhold_;

            int plotid_ ;
            static std::vector<std::string> plot_columns_ ;

            // If a position is angular, normalize it so that it falls within [-180, 180).
            double normalizePosition(double pos) {
                if (getSubsystem().isAngular()) return xero::math::normalizeAngleDegrees(pos);
                else return pos;
            }
        };
    } 
}
