#include "LifterGoToHeightAction.h"
#include "Lifter.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::list<std::string> LifterGoToHeightAction::plot_columns_ = {
            "time", 
            "tpos", "apos", "tvel", "avel", "out"
        } ;

        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, double target) : LifterAction(lifter) {
            target_ = target ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            ctrl_ = std::make_shared<PIDACtrl>(lifter.getRobot().getSettingsParser(), "lifter:follower:kv", 
                                "lifter:follower:ka", "lifter:follower:kp", "lifter:follower:kd") ;

            double maxv = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxv") ;
            double maxa = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxa") ;
            double maxd = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxd") ;                        
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;
        }

        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, const std::string &name) : LifterAction(lifter) {
            target_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
            ctrl_ = std::make_shared<PIDACtrl>(lifter.getRobot().getSettingsParser(), "lifter:follower:kv", 
                                "lifter:follower:ka", "lifter:follower:kp", "lifter:follower:kd") ;    
                                
            double maxv = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxv") ;
            double maxa = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxa") ;
            double maxd = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxd") ;                        
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;                                     
        }

        LifterGoToHeightAction::~LifterGoToHeightAction() {
        }

        void LifterGoToHeightAction::start() {
            Lifter &lifter = getLifter() ;

            if (!lifter.isCalibrated()) {
                MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "requested LifterGoToHeightAction when the lifter was not calibrated" ;
                logger.endMessage() ;
                is_done_ = true ;
            }
            else {
                double dist = target_ - getLifter().getHeight() ;
                if (std::fabs(dist) < threshold_)
                    is_done_ = true ;
                else {
                    is_done_ = false ;
                    profile_->update(dist, 0.0, 0.0) ;
                    start_time_ = getLifter().getRobot().getTime() ;
                    start_height_ = getLifter().getHeight() ;
                    plotid_ = getLifter().getRobot().startPlot("LifterGoToHeight", plot_columns_) ;
                    index_ = 0 ;

                    MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, getLifter().getMsgID()) ;
                    logger << "Lifter Velocity Profile: " << profile_->toString() ;
                    logger.endMessage() ;                    
                }
            }
        }

        void LifterGoToHeightAction::run() {
            Lifter &lifter = getLifter() ;
            double dt = lifter.getRobot().getDeltaTime() ;
            double elapsed = lifter.getRobot().getTime() - start_time_ ;
            double speed = lifter.getVelocity() ;
            double traveled = getLifter().getHeight() - start_height_ ;
            double delta = target_ - getLifter().getHeight() ;

            if (elapsed > profile_->getTotalTime())
            {
                if (std::fabs(delta) < threshold_) {
                    is_done_ = true ;
                    lifter.getRobot().endPlot(plotid_) ;
                } else {
                    is_done_ = true ;
                    return ;
                    //
                    // We reached the end of the profile, but are not where we
                    // want to be.  Create a new profile to get us there.
                    //
                    profile_->update(delta, speed, 0.0) ;
                    start_height_ = getLifter().getHeight() ;
                    start_time_ = getLifter().getRobot().getTime() ;
                    elapsed = 0 ;
                    traveled = 0 ;
                }
            }

            if (!is_done_)
            {
                double tdist = profile_->getDistance(elapsed) ;
                double tvel = profile_->getVelocity(elapsed) ;
                double tacc = profile_->getAccel(elapsed) ;

                double out = ctrl_->getOutput(tacc, tvel, tdist, traveled, dt) ;
                lifter.setMotorPower(out) ;

                lifter.getRobot().addPlotData(plotid_, index_, 0, elapsed) ;
                lifter.getRobot().addPlotData(plotid_, index_, 1, tdist + start_height_) ;
                lifter.getRobot().addPlotData(plotid_, index_, 2, traveled + start_height_) ;
                lifter.getRobot().addPlotData(plotid_, index_, 3, tvel) ;
                lifter.getRobot().addPlotData(plotid_, index_, 4, speed) ;
                lifter.getRobot().addPlotData(plotid_, index_, 5, out) ;

                index_++ ;
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