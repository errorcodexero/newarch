#include "TurntableGoToAngleAction.h"
#include "Turntable.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace phaser {
        std::list<std::string> TurntableGoToAngleAction::plot_columns_ = {
            "time", 
            "tangle", "aangle", "tvel", "avel", "out"
        } ;

        TurntableGoToAngleAction::TurntableGoToAngleAction(Turntable &turntable, double target) : TurntableAction(turntable) {
            target_ = target ;
            threshold_ = getTurntable().getRobot().getSettingsParser().getDouble("turntable:threshold") ;
            ctrl_ = std::make_shared<PIDACtrl>(turntable.getRobot().getSettingsParser(), "turntable:follower:kv", 
                                "turntable:follower:ka", "turntable:follower:kp", "turntable:follower:kd") ;

            double maxv = getTurntable().getRobot().getSettingsParser().getDouble("turntable:maxv")  ;
            double maxa = getTurntable().getRobot().getSettingsParser().getDouble("turntable:maxa")  ;
            double maxd = getTurntable().getRobot().getSettingsParser().getDouble("turntable:maxd")  ;                        
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;
        }

        TurntableGoToAngleAction::TurntableGoToAngleAction(Turntable &turntable, const std::string &name) : TurntableAction(turntable) {
            target_ = getTurntable().getRobot().getSettingsParser().getDouble(name) ;
            threshold_ = getTurntable().getRobot().getSettingsParser().getDouble("turntable:threshold") ;
            ctrl_ = std::make_shared<PIDACtrl>(turntable.getRobot().getSettingsParser(), "turntable:follower:kv", 
                                "turntable:follower:ka", "turntable:follower:kp", "turntable:follower:kd") ;  
                                  
            double maxv = getTurntable().getRobot().getSettingsParser().getDouble("turntable:maxv")  ;
            double maxa = getTurntable().getRobot().getSettingsParser().getDouble("turntable:maxa")  ;
            double maxd = getTurntable().getRobot().getSettingsParser().getDouble("turntable:maxd")  ;                        
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;   
        }

        TurntableGoToAngleAction::~TurntableGoToAngleAction() {
        }

        double TurntableGoToAngleAction::getAngleDifference(double start, double end) {
            double i1, i2, result;

            double minangle = getTurntable().getMinKeepOutAngle() ;
            double maxangle = getTurntable().gettMaxKeepoutAngle() ;

            if (start <= minangle && (end > minangle || end < start))
            {
                i1 = start - maxangle + 360;

                i2 = maxangle - end;
                if (i2 >= 360.0)
                    i2 -= 360.0;

                result = -(i1 + i2);
                if (result <= -360)
                    result += 360;
            }
            else 
            {
                i1 = maxangle - start;

                i2 = end - maxangle;
                if (i2 < 0)
                    i2 += 360.0;

                result = i1 + i2;
                if (result >= 360)
                    result -= 360;
            }

            return result;            
        }

        void TurntableGoToAngleAction::start() {
            start_time_ = getTurntable().getRobot().getTime() ;

            Turntable &turntable = getTurntable() ;

            if (!turntable.isCalibrated()) {
                MessageLogger &logger = turntable.getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "requested TurntableGoToAngleAction when the turntable was not calibrated" ;
                logger.endMessage() ;
                is_done_ = true ;
            }
            else {
                double dist = getAngleDifference(getTurntable().getAngleValue(), target_) ;
                if (std::fabs(dist) < threshold_)
                    is_done_ = true ;
                else {
                    is_done_ = false ;
                    profile_->update(dist, 0.0, 0.0) ;
                    start_time_ = getTurntable().getRobot().getTime() ;
                    start_angle_ = getTurntable().getAngleValue() ;

                    MessageLogger &logger = turntable.getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::error) ;
                    logger << "TurntableGoToAngle Velocity Profile: " << profile_->toString() ;
                    logger.endMessage() ;                    

                    plotid_ = getTurntable().getRobot().startPlot("TurntableGoToAngle", plot_columns_) ;
                    index_ = 0 ;
                }
            }
        }

        void TurntableGoToAngleAction::run() {
            Turntable &turntable = getTurntable() ;
            double dt = turntable.getRobot().getDeltaTime() ;
            double elapsed = turntable.getRobot().getTime() - start_time_ ;
            double speed = turntable.getVelocity() ;
            double traveled = getTurntable().getAngleValue() - start_angle_ ;
            double delta = getAngleDifference(getTurntable().getAngleValue(), target_) ;

            if (elapsed > profile_->getTotalTime())
            {
                if (std::fabs(delta) < threshold_) {
                    is_done_ = true ;
                    turntable.setMotorPower(0.0) ;
                    turntable.getRobot().endPlot(plotid_) ;
                } else {
                    is_done_ = true ;
                    //
                    // We reached the end of the profile, but are not where we
                    // want to be.  Create a new profile to get us there.
                    //
                    profile_->update(delta, speed, 0.0) ;
                    start_angle_ = getTurntable().getAngleValue() ;
                    start_time_ = getTurntable().getRobot().getTime() ;
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
                turntable.setMotorPower(out) ;

                turntable.getRobot().addPlotData(plotid_, index_, 0, elapsed) ;
                turntable.getRobot().addPlotData(plotid_, index_, 1, tdist + start_angle_) ;
                turntable.getRobot().addPlotData(plotid_, index_, 2, traveled + start_angle_) ;
                turntable.getRobot().addPlotData(plotid_, index_, 3, tvel) ;
                turntable.getRobot().addPlotData(plotid_, index_, 4, speed) ;
                turntable.getRobot().addPlotData(plotid_, index_, 5, out) ;

                index_++ ;
            }
        }

        bool TurntableGoToAngleAction::isDone() {
            return is_done_ ;
        }

        void TurntableGoToAngleAction::cancel() {
            is_done_ = true ;
        }

        std::string TurntableGoToAngleAction::toString() {
            std::string result = "TurntableGoToAngle " + std::to_string(target_) ;
            return result ;
        }
    }
}