#include "LifterGoToHeightAction.h"
#include "Lifter.h"
#include <Robot.h>
#include <MessageLogger.h>
#include <cmath>

using namespace xero::misc ;

namespace xero {
    namespace base {
        std::vector<std::string> LifterGoToHeightAction::plot_columns_ = {
            "time", 
            "tpos", "apos", "tvel", "avel", "out"
        } ;

        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, double target, bool relative) : LifterAction(lifter) {
            relative_ = relative ;
            target_ = target ;
            offset_ = target ;
            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;


            double maxv = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxv") ;
            double maxa = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxa") ;
            double maxd = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxd") ;                        
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;

            pid_ctrl_.initFromSettingsExtended(lifter.getRobot().getSettingsParser(), "lifter:hold") ;    

            plot_id_ = getLifter().initPlot(toString()) ;         
        }

        LifterGoToHeightAction::LifterGoToHeightAction(Lifter &lifter, const std::string &name, bool relative) : LifterAction(lifter) {
            relative_ = relative ;            
            target_ = getLifter().getRobot().getSettingsParser().getDouble(name) ;
            offset_ = target_ ;

            threshold_ = getLifter().getRobot().getSettingsParser().getDouble("lifter:threshold") ;
  
                                
            double maxv = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxv") ;
            double maxa = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxa") ;
            double maxd = getLifter().getRobot().getSettingsParser().getDouble("lifter:maxd") ;                        
            profile_ = std::make_shared<TrapezoidalProfile>(maxa, maxd, maxv) ;     

            pid_ctrl_.initFromSettingsExtended(lifter.getRobot().getSettingsParser(), "lifter:hold") ;

            plot_id_ = getLifter().initPlot(toString()) ;
        }

        LifterGoToHeightAction::~LifterGoToHeightAction() {
        }

        void LifterGoToHeightAction::start() {
            Lifter &lifter = getLifter() ;

            if (relative_)
                target_ = lifter.expected_height_ + offset_ ;

            if (!lifter.isCalibrated()) {
                MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::error) ;
                logger << "requested LifterGoToHeightAction when the lifter was not calibrated" ;
                logger.endMessage() ;
                is_done_ = true ;
            }
            else {
                double dist = target_ - getLifter().getHeight() ;
                if (std::fabs(dist) < threshold_) {
                    is_done_ = true ;

                    MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, getLifter().getMsgID()) ;
                    logger << "LifterGoToHeightAction: action completed sucessfully in start" ;
                    logger.endMessage() ;

                } else {

                    if (dist < 0) {
                        ctrl_ = std::make_shared<PIDACtrl>(lifter.getRobot().getSettingsParser(), "lifter:follower:down:kv", 
                                "lifter:follower:down:ka", "lifter:follower:down:kp", "lifter:follower:down:kd") ;     
                    }
                    else {
                        ctrl_ = std::make_shared<PIDACtrl>(lifter.getRobot().getSettingsParser(), "lifter:follower:up:kv", 
                                "lifter:follower:up:ka", "lifter:follower:up:kp", "lifter:follower:up:kd") ;                          
                    }
                    is_done_ = false ;
                    profile_->update(dist, 0.0, 0.0) ;
                    start_time_ = getLifter().getRobot().getTime() ;
                    start_height_ = getLifter().getHeight() ;
                    std::string targetstr = std::to_string(target_) ;
                    getLifter().startPlot(plot_id_, plot_columns_) ;

                    MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, getLifter().getMsgID()) ;
                    logger << "Lifter Target Distance: " << dist ;
                    logger.endMessage() ;

                    logger.startMessage(MessageLogger::MessageType::debug, getLifter().getMsgID()) ;                    
                    logger << "Lifter Velocity Profile: " << profile_->toString() ;
                    logger.endMessage() ;                    
                }
            }

            lifter.expected_height_ = target_ ;
        }

        void LifterGoToHeightAction::run() {
            Lifter &lifter = getLifter() ;

            if (is_done_) {
                double dist = target_ - lifter.getHeight() ;
                double out = pid_ctrl_.getOutput(0, dist, getLifter().getRobot().getDeltaTime()) ;
                lifter.setMotorPower(out) ;                
            } 
            else 
            {
                double dt = lifter.getRobot().getDeltaTime() ;
                double elapsed = lifter.getRobot().getTime() - start_time_ ;
                double speed = lifter.getVelocity() ;
                double traveled = getLifter().getHeight() - start_height_ ;
                double delta = target_ - getLifter().getHeight() ;

                if (elapsed > profile_->getTotalTime())
                {
                    MessageLogger &logger = lifter.getRobot().getMessageLogger() ;
                    logger.startMessage(MessageLogger::MessageType::debug, getLifter().getMsgID()) ;
                    logger << "LifterGoToHeightAction: action completed" ;
                    logger << ", delta = " << delta ;
                    logger.endMessage() ;

                    is_done_ = true ;
                    lifter.endPlot(plot_id_) ;
                    lifter.setMotorPower(0.0) ;
                }

                if (!is_done_)
                {
                    std::vector<double> data ;
                    double tdist = profile_->getDistance(elapsed) ;
                    double tvel = profile_->getVelocity(elapsed) ;
                    double tacc = profile_->getAccel(elapsed) ;

                    double out = ctrl_->getOutput(tacc, tvel, tdist, traveled, dt) ;
                    lifter.setMotorPower(out) ;

                    data.push_back(elapsed) ;
                    data.push_back(tdist + start_height_) ;
                    data.push_back(traveled + start_height_) ;
                    data.push_back(tvel) ;
                    data.push_back(speed) ;
                    data.push_back(out) ;
                    lifter.addPlotData(plot_id_, data) ;
                }
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
            if (relative_)
                result += " RELATIVE" ;
            return result ;
        }
    }
}