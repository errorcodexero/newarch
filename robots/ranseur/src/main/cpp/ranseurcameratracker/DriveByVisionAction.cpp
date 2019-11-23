#include "DriveByVisionAction.h"
#include "ranseurids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace ranseur {
       
        DriveByVisionAction::DriveByVisionAction(TankDrive &tank_drive, RanseurLimeLight &camera) : TankDriveAction(tank_drive), camera_(camera)
        {
            yaw_base_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_base_power") ;
            yaw_p_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_p") ;
            distance_threshold_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:distance_threshold") ;
        }

        void DriveByVisionAction::start() {
         
            lost_count_ = 0 ;
            is_done_ = false ;

        }


        void DriveByVisionAction::run() {
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            if (!camera_.isTargetPresent()) {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                logger << "DriveByVision: camera lost target" ;
                logger.endMessage() ;                

                lost_count_++ ;

                if (lost_count_ > 8) {
                    is_done_ = true;

                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision: action done due to too many lost targets conditions" ;
                    logger.endMessage() ; 

                    setMotorsToPercents(0.0, 0.0) ;
                }
            }
            else {
                double yaw = camera_.getYaw() ;
                double dist = camera_.getDistance() ;

                if (dist < distance_threshold_) {
                    is_done_ = true ;
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision: action done due to being too close" ;
                    logger.endMessage() ;     
                }
                else {

                    lost_count_ = 0 ;

                   
                    double yawadj = camera_.getYaw() * yaw_p_ ;
                    double left = yaw_base_power_ + yawadj ;
                    double right = yaw_base_power_ - yawadj ;
                    setMotorsToPercents(left, right) ;

                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision:" ;
                    logger << " yaw " << yaw ; 
                    logger << " yawadj " << yawadj ;
                    logger << " left " << left ;
                    logger << " right " << right ;
                    logger << " dist " << camera_.getDistance() ;
                    logger << " valid " << camera_.isTargetPresent() ;
                    logger.endMessage() ;
                }
            }
        }

        /// \brief Cancel the action
        void DriveByVisionAction::cancel() { 
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;


            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision: action canceled" ;
            logger << " yaw " << camera_.getYaw() ;
            logger << " dist " << camera_.getDistance() ;
            logger.endMessage() ;             
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
