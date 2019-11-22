#include "DriveByVisionAction.h"
#include "ranseurids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
        DriveByVisionAction::DriveByVisionAction(TankDrive &tank_drive, RanseurCameraTracker &camera) : TankDriveAction(tank_drive), camera_(camera)
        {
            yaw_base_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_base_power") ;
            yaw_p_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_p") ;
            max_lost_count_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:max_lost_count") ;
            done_dist_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:done_distance") ;
        }

        void DriveByVisionAction::start() {
            lost_count_ = 0 ;
            is_done_ = false ;
        }

        void DriveByVisionAction::run() {
            std::vector<double> data ;
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            if (!camera_.isPresent())
            {
                // There is no limelight, the limelight subsystem reported this.
                return ;
            }

            if (!camera_.getTV())
            {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                logger << "DriveByVision: camera lost target" ;
                logger.endMessage() ;                

                lost_count_++ ;
                if (lost_count_ > max_lost_count_) {
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision: action done due to too many lost targets conditions" ;
                    logger.endMessage() ; 
                    setMotorsToPercents(0.0, 0.0) ;
                    is_done_ = true ;
                }
            }
            else {
                double yaw = camera_.getTX() ;
                double dist = camera_.getDistance() ;

                if (dist < done_dist_) {
                    is_done_ = true ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision: action complete, distance " << dist ;
                    logger.endMessage() ;     
                    setMotorsToPercents(0.0, 0.0) ;                    
                }
                else {
                    // We have a target, reset lost count
                    lost_count_ = 0 ;

                    double yawadj = yaw * yaw_p_ ;
                    double left = yaw_base_power_ + yawadj ;
                    double right = yaw_base_power_ - yawadj ;
                    setMotorsToPercents(left, right) ;
                }
            }
        }

        /// \brief Cancel the action
        void DriveByVisionAction::cancel() { 
            is_done_ = true ;
            setMotorsToPercents(0.0, 0.0) ;
        }

        /// \brief Return true if the action is complete
        /// \returns True if the action is complete
        bool DriveByVisionAction::isDone() {
            return is_done_ ;
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable str
        std::string DriveByVisionAction::toString() {
            std::string ret("DriveByVision") ;
            return ret ;
        }
    }
}
