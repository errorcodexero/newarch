#include "DriveByVisionAction.h"
#include "phaserids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        std::list<std::string> DriveByVisionAction::plot_columns_ = {
            "time", 
            "tyaw", "ayaw", "error", "adj", "left", "right"
        } ;

        DriveByVisionAction::DriveByVisionAction(TankDrive &tank_drive, PhaserCameraTracker &camera, bool reverse) : TankDriveAction(tank_drive), camera_(camera)
        {
            yaw_base_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_base_power") ;
            yaw_p_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_p") ;

            reverse_ = reverse ;
            if (reverse) {
                yaw_base_power_ = -yaw_base_power_ ;
            }
        }

        std::string DriveByVisionAction::toString(DriveByVisionAction::State st) {
            std::string ret = "???" ;

            switch(st) {
            case State::DriveYaw:
                ret = "DriveYaw" ;
                break ;

            case State::Tracking:
                ret = "Tracking" ;
                break ;                

            case State::Done:
                ret = "Done" ;
                break ;                
            }

            return ret ;
        }

        void DriveByVisionAction::start() {
            state_ = State::DriveYaw ;
            lost_count_ = 0 ;

            plotid_ = getTankDrive().getRobot().startPlot("DriveByVision", plot_columns_) ;            
            start_ = getTankDrive().getRobot().getTime() ;
            index_ = 0 ;
        }

        void DriveByVisionAction::driveTracking() {
        }

        void DriveByVisionAction::driveByYaw() {
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;
            double elapsed = getTankDrive().getRobot().getTime() - start_ ;

            if (!camera_.isValid()) {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                logger << "DriveByVision: camera lost target" ;
                logger.endMessage() ;                

                lost_count_ ++ ;

                if (lost_count_ > 8) {
                    state_ = State::Done ;

                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision: action done due to too many lost targets conditions" ;
                    logger.endMessage() ; 
                    getTankDrive().getRobot().endPlot(plotid_) ;                                            
                }
            }
            else {
                double yaw = camera_.getYaw() ;
                double dist = camera_.getDistance() ;

                if (dist < 12.0) {
                    state_ = State::Done ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision: action done due to being too close" ;
                    logger.endMessage() ;     
                    getTankDrive().getRobot().endPlot(plotid_) ;                                        
                }
                else {

                    lost_count_ = 0 ;

                    double desired_yaw = 0.0031 * dist * dist - 0.3855 * dist + 14.938 ;
                    double yawerror = yaw - desired_yaw ;

                    double yawadj = yawerror * yaw_p_ ;
                    double left = yaw_base_power_ + yawadj ;
                    double right = yaw_base_power_ - yawadj ;
                    setMotorsToPercents(left, right) ;

                    getTankDrive().getRobot().addPlotData(plotid_, index_, 0, elapsed) ;
                    getTankDrive().getRobot().addPlotData(plotid_, index_, 1, desired_yaw) ;
                    getTankDrive().getRobot().addPlotData(plotid_, index_, 2, yaw) ;
                    getTankDrive().getRobot().addPlotData(plotid_, index_, 3, yawerror) ;
                    getTankDrive().getRobot().addPlotData(plotid_, index_, 4, yawadj) ;
                    getTankDrive().getRobot().addPlotData(plotid_, index_, 5, left) ;                    
                    getTankDrive().getRobot().addPlotData(plotid_, index_, 6, right) ;  

                    index_++ ;    

                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision:" ;
                    logger << " state " << toString(state_) ;
                    logger << " yaw " << yaw ;
                    logger << " desired " << desired_yaw ;
                    logger << " yawerror " << yawerror ; 
                    logger << " yawadj " << yawadj ;
                    logger << " left " << left ;
                    logger << " right " << right ;
                    logger << " dist " << camera_.getDistance() ;
                    logger << " valid " << camera_.isValid() ;
                    logger.endMessage() ;
                }
            }
        }

        /// \brief Manage the action; called each time through the robot loop
        void DriveByVisionAction::run() {
            switch(state_) {
            case State::DriveYaw:
                driveByYaw() ;
                break ;

            case State::Tracking:
                driveTracking() ;
                break ;

            case State::Done:
                setMotorsToPercents(0.0, 0.0) ;
                break ;                
            }
        }
        
        /// \brief Cancel the action
        void DriveByVisionAction::cancel() { 
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            state_ = State::Done ;
            getTankDrive().getRobot().endPlot(plotid_) ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision: action canceled" ;
            logger << " yaw " << camera_.getYaw() ;
            logger << " dist " << camera_.getDistance() ;
            logger << " rect " << camera_.getRectRatio() ;
            logger.endMessage() ;             
        }

        /// \brief Return true if the action is complete
        /// \returns True if the action is complete
        bool DriveByVisionAction::isDone() {
            return state_ == State::Done ;
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable str
        std::string DriveByVisionAction::toString() {
            std::string ret("DriveByVision") ;
            return ret ;
        }
    }
}
