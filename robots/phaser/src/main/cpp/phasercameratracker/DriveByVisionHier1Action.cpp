#include "DriveByVisionHier1Action.h"
#include "phaserids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        DriveByVisionHier1Action::DriveByVisionHier1Action(TankDrive &tank_drive, PhaserCameraTracker &camera) : TankDriveAction(tank_drive), camera_(camera)
        {
            base_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:base_power") ;
            yaw_adjust_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:yaw_adjust_power") ;
            rect_adjust_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:rect_adjust_power") ;            
            yaw_limit_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:yaw_limit") ; 
            dist_yaw_limit_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:dist_yaw_limit") ;           
        }


        void DriveByVisionHier1Action::start() {
            is_done_ = false ;
            getTankDrive().lowGear() ;
        }

        /// \brief Manage the action; called each time through the robot loop
        void DriveByVisionHier1Action::run() {
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            double left = base_power_ ;
            double right = base_power_ ;

            double yaw = camera_.getYaw() ;
            double rect = camera_.getRectRatio() ;
            double yawadj = yaw * yaw_adjust_power_ ;
            double rectadj = ( rect - 1.0 )* rect_adjust_power_ ;
            double dist = camera_.getDistance() ;
            bool yawdominate = false;

            if (std::fabs(yaw) > yaw_limit_ || dist > dist_yaw_limit_) {
                left += yawadj ;
                right -= yawadj ;
                yawdominate = true ;
            }
            else {
                left += rectadj ;
                right -= rectadj ;                
            }
            setMotorsToPercents(left, right) ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision:" ;
            logger << " dist " << dist ;
            logger << " yaw " << yaw ;
            logger << " yawadj " << yawadj ;
            logger << " rect " << rect ;
            logger << " rectadj " << rectadj ;
            logger << " left " << left ;
            logger << " right " << right ;
            logger << " yawdominate " << yawdominate ;
            logger.endMessage() ; 

            if (camera_.getDistance() < 15.0) {
                setMotorsToPercents(0.0, 0.0) ;
                is_done_ = true ;
            }           
        }

        /// \brief Cancel the action
        void DriveByVisionHier1Action::cancel() {            
        }

        /// \brief Return true if the action is complete
        /// \returns True if the action is complete
        bool DriveByVisionHier1Action::isDone() {
            return is_done_ ;
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable str
        std::string DriveByVisionHier1Action::toString() {
            std::string ret("DriveByVisionHier1") ;
            return ret ;
        }
    }
}
