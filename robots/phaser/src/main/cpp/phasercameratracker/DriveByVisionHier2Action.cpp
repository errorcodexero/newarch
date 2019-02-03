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
            fast_factor_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:2:fast_factor") ;
            turn_cycles_ = tank_drive.getRobot().getSettingsParser().getInteger("drivevision:2:turn_cycles") ;
            turn_straight_ = tank_drive.getRobot().getSettingsParser().getInteger("drivevision:2:turn_straight") ;
            yaw_max_threshold_ = tank_drive.getRobot().getSettingsParser().getDouble("drivevision:2:yaw_max_threshold") ;
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

            if (is_done_)
                return ;

#ifdef NOTYET
            if (dist <= 15.0) {
                is_done_ = true ;
                setMotorsToPercents(0.0, 0.0) ;
                return ;
            }
#endif
            if (!camera_.isValid() && state_ == State::TrackingYaw) {
                setMotorsToPercents(0.0, 0.0) ;
                return ;
            }

            if (fast_) {
                left += yawadj ;
                right -= yawadj ;     
            }
            else {
                if (state_ == State::TrackingYaw) {
                    //
                    // We are using a simple proportional adjustment to track the
                    // yaw angle of the target
                    //
                    if (std::fabs(yaw) < yaw_switch_threshold_)
                    { 
                        if (rectdiff > rect_threshold_) {
                            if (fast_) {
                                fast_ = false ;
                                base_power_ /= fast_factor_ ;
                            }
                            //
                            // If the YAW angle is below a threshold, we are aimed straight
                            // enough at the target that we figure out how to curve to the
                            // middle of the target.  
                            //
                            calcCurvePath(dist, rect) ;
                            state_ = State::FirstCurve ;
                        }
                        else {
                            if (fast_ == false) {
                                //
                                // We are aimed at the target and we are in the center,
                                // full speed straight ahead captain.
                                //
                                base_power_ *= fast_factor_ ;
                                fast_ = true ;
                            }
                            left += yawadj ;
                            right -= yawadj ;                        
                        }
                    }
                    else {
                        if (fast_) {
                            fast_ = false ;
                            base_power_ /= fast_factor_ ;
                        }

                        left += yawadj ;
                        right -= yawadj ;
                    }
                }
                else if (state_ == State::FirstCurve) {
                    state = "FirstCurve" ;                        
                    //
                    // We are tracking a simple curve by adding a fixed offset to 
                    // one side of the robot and removing it from the other side
                    // of the robot.
                    //
                    left += left_offset_ ;
                    right += right_offset_ ;
                    cycles_-- ;

                    if (cycles_ == 0) {
                        if (turn_straight_ != 0) {
                            state_ = State::DrivingStraight ;
                            cycles_ = turn_straight_ ;
                        }
                        else {
                            state_ = State::SecondCurve ;
                            cycles_ = this_turn_cycles_ ;
                        }
                    }
                }
                else if (state_ == State::DrivingStraight) {
                    state = "DrivingStraight" ;
                    cycles_-- ;
                    if (cycles_ == 0) {
                        state_ = State::SecondCurve ;
                        cycles_ = this_turn_cycles_ ;
                    }
                }
                else {
                    state = "SecondCurve" ;
                    left -= left_offset_ ;
                    right -= right_offset_ ;
                    cycles_-- ;
                    if (cycles_ == 0)
                        state_ = State::TrackingYaw ;
                }
            }

            setMotorsToPercents(left, right) ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision:" ;
            logger << " state " << state ;
            logger << " valid " << camera_.isValid() ;
            logger << " dist " << dist ;
            logger << " yaw " << yaw ;
            logger << " rect " << rect ;
            logger << " rectproc " << rectdiff ;
            logger << " left " << left ;
            logger << " right " << right ;   
            logger << " fast " << fast_ ;    
            logger << " cycles " << cycles_ ;                 
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
            this_turn_cycles_ = static_cast<int>(dist * 1.5) ;
            if (this_turn_cycles_ > 125)
                this_turn_cycles_ = 125 ;
            cycles_ =  this_turn_cycles_ ;
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
