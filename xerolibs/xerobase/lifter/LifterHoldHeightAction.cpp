#include "LifterHoldHeightAction.h"
#include "Lifter.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace base {
        LifterHoldHeightAction::LifterHoldHeightAction(Lifter &lifter, double target) : LifterAction(lifter) {
            target_ = target ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            pid_ctrl_.initFromSettingsExtended(lifter.getRobot().getSettingsParser(), "lifter:hold") ;            
        }

        LifterHoldHeightAction::LifterHoldHeightAction(Lifter &lifter, const std::string &name) : LifterAction(lifter) {
            target_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            pid_ctrl_.initFromSettingsExtended(lifter.getRobot().getSettingsParser(), "lifter:hold") ;
        }

        LifterHoldHeightAction::~LifterHoldHeightAction() {
        }

        void LifterHoldHeightAction::start() {
        }

        void LifterHoldHeightAction::run() {
            double dist = target_ - getLifter().getHeight() ;
            double out = pid_ctrl_.getOutput(0, dist, 0, getLifter().getRobot().getDeltaTime()) ;
            getLifter().setMotorPower(out) ;
        }

        bool LifterHoldHeightAction::isDone() {
            return false ;
        }

        void LifterHoldHeightAction::cancel() {
        }

        std::string LifterHoldHeightAction::toString() {
            std::string result = "LifterHoldHeight " + std::to_string(target_) ;
            return result ;
        }
    }
}