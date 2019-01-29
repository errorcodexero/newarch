#include "LifterGoToHeightAction.h"
#include "Lifter.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace base {
        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, double target) : LifterAction(lifter) {
            target_ = target ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            ctrl_ = std::make_shared<PIDACtrl>(lifter.getRobot().getSettingsParser(), "lifter:follower:kv", 
                                "lifter:follower:ka", "lifter:follower:kp", "lifter:follower:kd") ;
            profile_ = std::make_shared<TrapezoidalProfile>(40, -40, 40) ;
        }

        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, const std::string &name) : LifterAction(lifter) {
            target_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            ctrl_ = std::make_shared<PIDACtrl>(lifter.getRobot().getSettingsParser(), "lifter:follower:kv", 
                                "lifter:follower:ka", "lifter:follower:kp", "lifter:follower:kd") ;    
            profile_ = std::make_shared<TrapezoidalProfile>(40, -40, 40) ;                                      
        }

        LifterGoToHeightAction::~LifterGoToHeightAction() {
        }

        void LifterGoToHeightAction::start() {
            double dist = target_ - getLifter().getHeight() ;
            if (std::fabs(dist) < threshold_)
                is_done_ = true ;
            else {
                is_done_ = false ;
                profile_->update(dist, 0.0, 0.0) ;
                start_time_ = getLifter().getRobot().getTime() ;
            }
        }

        void LifterGoToHeightAction::run() {
            Lifter &lifter = getLifter() ;
            double dt = lifter.getRobot().getDeltaTime() ;
            double elapsed = lifter.getRobot().getTime() - start_time_ ;
            double speed = lifter.getVelocity() ;
            double dist = target_ - getLifter().getHeight() ;

            if (elapsed > profile_->getTotalTime())
            {
                if (std::fabs(dist) < threshold_)
                    is_done_ = true ;
                else {
                    //
                    // We reached the end of the profile, but are not where we
                    // want to be.
                    //
                    profile_->update(dist, speed, 0.0) ;
                }
            }

            if (!is_done_)
            {
                double tdist = profile_->getDistance(elapsed) ;
                double tvel = profile_->getVelocity(elapsed) ;
                double tacc = profile_->getAccel(elapsed) ;

                double out = ctrl_->getOutput(tacc, tvel, tdist, dist, dt) ;
                lifter.setMotorPower(out) ;
            }
        }

        bool LifterGoToHeightAction::isDone() {
            return is_done_ ;
        }

        void LifterGoToHeightAction::cancel() {
            is_done_ = true ;
        }

        std::string LifterGoToHeightAction::toString() {
            std::string result = "LifterGoToHeight " + std::to_string(target_) ;
            return result ;
        }
    }
}