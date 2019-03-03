#include "DriveByVisionAction.h"
#include "phaserids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        DriveByVisionAction::DriveByVisionAction(TankDrive &tank_drive, PhaserCameraTracker &camera, bool reverse) : TankDriveAction(tank_drive), camera_(camera)
        {
            yaw_base_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_base_power") ;
            yaw_p_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_p") ;

            if (reverse) {
                yaw_base_power_ = -yaw_base_power_ ;
                yaw_p_ = yaw_p_ ;
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
        }

        void DriveByVisionAction::driveTracking() {
        }

        void DriveByVisionAction::driveByYaw() {
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            if (!camera_.isValid()) {
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                logger << "DriveByVision: camera lost target" ;
                logger.endMessage() ;                
            }
            else {
                double yaw = camera_.getYaw() ;
                double dist = camera_.getDistance() ;

                double desired_yaw = 0.0031 * dist * dist - 0.3855 * dist + 14.938 ;
                yaw -= desired_yaw ;

                double yawadj = yaw * yaw_p_ ;
                double left = yaw_base_power_ + yawadj ;
                double right = yaw_base_power_ - yawadj ;
                setMotorsToPercents(left, right) ;

                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                logger << "DriveByVision:" ;
                logger << " state " << toString(state_) ;
                logger << " yaw " << yaw ;
                logger << " yawadj " << yawadj ;
                logger << " left " << left ;
                logger << " right " << right ;
                logger << " dist " << camera_.getDistance() ;
                logger << " valid " << camera_.isValid() ;
                logger.endMessage() ;
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
            state_ = State::Done ;
            std::cout << "DriveByVision canceled" << std::endl ;
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
