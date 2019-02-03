#include "DriveByVisionHier2Action.h"
#include "phaserids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        DriveByVisionHier2Action::DriveByVisionHier2Action(TankDrive &tank_drive, PhaserCameraTracker &camera) : TankDriveAction(tank_drive), camera_(camera)
        {
            yaw_adjust_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:2:yaw_adjust_power") ;
            base_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:2:base_power") ;
            yaw_switch_threshold_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:2:yaw_switch_threshold") ; 
            turn_offset_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:2:turn_offset") ;
            rect_threshold_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:2:rect_threshold") ;
        }


        void DriveByVisionHier2Action::start() {
            state_ = State::TrackingYaw ;
            is_done_ = false ;
            fast_ = false ;
            getTankDrive().lowGear() ;
        }

        /// \brief Manage the action; called each time through the robot loop
        void DriveByVisionHier2Action::run() {
            std::string state = "TrackingYaw" ;
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            double dist = camera_.getDistance() ;
            double yaw = camera_.getYaw() ;
            double rect = camera_.getRectRatio() ;

            double left = base_power_ ;
            double right = base_power_ ;

            double yawadj = yaw * yaw_adjust_power_ ;  
            double rectdiff = std::fabs(rect - 1.0) ;          

            if (state_ == State::TrackingYaw) {
                //
                // We are using a simple proportional adjustment to track the
                // yaw angle of the target
                //
                if (yaw < yaw_switch_threshold_)
                { 
                    if (rectdiff > rect_threshold_) {
                        if (fast_) {
                            fast_ = false ;
                            base_power_ /= 2.0 ;
                        }
                        //
                        // If the YAW angle is below a threshold, we are aimed straight
                        // enough at the target that we figure out how to curve to the
                        // middle of the target.  
                        //
                        calcCurvePath(dist, rect) ;
                        state_ = State::TrackingCenter ;
                    }
                    else {
                        //
                        // We are aimed at the target and we are in the center,
                        // full speed straight ahead captain.
                        //
                        base_power_ *= 2.0 ;
                    }
                }
                else {
                    if (fast_) {
                        fast_ = false ;
                        base_power_ /= 2.0 ;
                    }

                    left += yawadj ;
                    right -= yawadj ;
                }
            }
            else {
                //
                // We are tracking a simple curve by adding a fixed offset to 
                // one side of the robot and removing it from the other side
                // of the robot.
                //
                left += left_offset_ ;
                right += right_offset_ ;
                cycles_-- ;

                if (cycles_ == 0)
                    state_ = State::TrackingYaw ;
            }

            setMotorsToPercents(left, right) ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision:" ;
            logger << " state " << state ;
            logger << " dist " << dist ;
            logger << " yaw " << yaw ;
            logger << " rect " << rect ;
            logger << " left " << left ;
            logger << " right " << right ;                        
            logger.endMessage() ; 

        }

        void DriveByVisionHier2Action::calcCurvePath(double dist, double rect) {
            //
            // We calculate the magnitude of the left and right based on the
            // distance and the rect ratio
            //
            if (rect < 1.0) {
                left_offset_ = -turn_offset_ ;
                right_offset_ = turn_offset_ ;
            } else {
                left_offset_ = turn_offset_ ;
                right_offset_ = -turn_offset_ ;                
            }
            cycles_ =  50 ;
        }

        /// \brief Cancel the action
        void DriveByVisionHier2Action::cancel() {            
        }

        /// \brief Return true if the action is complete
        /// \returns True if the action is complete
        bool DriveByVisionHier2Action::isDone() {
            return is_done_ ;
        }

        /// \brief return a human readable string representing the action
        /// \returns a human readable str
        std::string DriveByVisionHier2Action::toString() {
            std::string ret("DriveByVisionHier2") ;
            return ret ;
        }
    }
}
