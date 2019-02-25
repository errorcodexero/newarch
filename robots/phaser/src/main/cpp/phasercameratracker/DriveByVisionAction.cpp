#include "DriveByVisionAction.h"
#include "phaserids.h"
#include <cmath>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        DriveByVisionAction::DriveByVisionAction(TankDrive &tank_drive, PhaserCameraTracker &camera) : TankDriveAction(tank_drive), camera_(camera)
        {
            yaw_base_power_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_base_power") ;
            yaw_p_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_p") ;
            yaw_threshold_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:yaw_threshold") ;
            lost_target_threshold_ = tank_drive.getRobot().getSettingsParser().getInteger("drivebyvision:lost_target_threshold") ;

            outer_dist_threshold_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:outer_distance") ;

            inner_boundaries_[0] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:inner:0") ;
            inner_boundaries_[1] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:inner:1") ;
            inner_boundaries_[2] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:inner:2") ;
            inner_boundaries_[3] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:inner:3") ;

            outer_boundaries_[0] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:outer:0") ;
            outer_boundaries_[1] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:outer:1") ;
            outer_boundaries_[2] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:outer:2") ;
            outer_boundaries_[3] = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:outer:3") ;  

            scurve_base_power_ =  tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:scurve_base_power") ;
            scurve_turn_offset_ =  tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:scurve_turn_offset") ;
            scurve_cycles_ = tank_drive.getRobot().getSettingsParser().getInteger("drivebyvision:scurve_cycles") ;

            quit_threshold_ = tank_drive.getRobot().getSettingsParser().getDouble("drivebyvision:quit_threshold") ;
        }

        std::string DriveByVisionAction::toString(DriveByVisionAction::State st) {
            std::string ret = "???" ;

            switch(st) {
            case State::FinalDriveByYaw:
                ret = "FinalDriveByYaw" ;
                break ;                
            case State::InitialDriveByYaw:
                ret = "InitialDriveByYaw" ;
                break ;                
            case State::DriveFirstSCurve:
                ret = "DriveFirstSCurve" ;
                break ;  
            case State::DriveSecondSCurve:
                ret = "DriveSecondSCurve" ;
                break ;                                
            case State::Done:
                ret = "Done" ;
                break ;
            }

            return ret ;
        }

        void DriveByVisionAction::start() {
            state_ = State::FinalDriveByYaw ;
            lost_target_count_ = 0 ;
        }

        void DriveByVisionAction::driveByYaw() {
            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            double yaw = camera_.getYaw() ;

            yaw -= 6.0 ;

            double yawadj = yaw * yaw_p_ ;
            double left = yaw_base_power_ + yawadj ;
            double right = yaw_base_power_ - yawadj ;
            setMotorsToPercents(left, right) ;

            if (!camera_.isValid()) {
                lost_target_count_++ ;
                if (lost_target_count_ == lost_target_threshold_) {
                    logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                    logger << "DriveByVision: going to done, lost target" ;
                    logger << ", lost_target_count " << lost_target_count_  ;
                    logger.endMessage() ;                    
                    state_ = State::Done ;
                    left = 0 ;
                    right = 0 ;
                }
            }
            else {
                lost_target_count_ = 0 ;
            }

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

        bool DriveByVisionAction::driveSCurve() {
            bool ret = false ;

            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;

            double left = scurve_base_power_ + scurve_left_offset_ ;
            double right = scurve_base_power_ + scurve_right_offset_ ;

            setMotorsToPercents(left, right) ;

            cycles_-- ;

            if (cycles_ == 0)
                ret = true ;

            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "DriveByVision:" ;
            logger << " state " << toString(state_) ;
            logger << " cycles " << cycles_ ;
            logger << " left " << left ;
            logger << " right " << right ;
            logger.endMessage() ;

            return ret ;
        }

        int DriveByVisionAction::computeQuadrant() {
            double dist = camera_.getDistance() ;
            double rect = camera_.getRectRatio() ;
            int quad = -1 ;

            MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;
            logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
            logger << "dist " << dist << " rect " << rect ;
            logger.endMessage() ;


            if (dist > outer_dist_threshold_) {
                //
                // We are in the outer quadrants (1, 2, or 3)
                //
                if (rect >= outer_boundaries_[0] && rect <= outer_boundaries_[1])
                    quad = 1 ;
                else if (rect > outer_boundaries_[1] && rect <= outer_boundaries_[2])
                    quad = 3 ;
                else if (rect > outer_boundaries_[2] && rect <= outer_boundaries_[3])
                    quad = 2 ;
            }
            else {
                //
                // We are in the inner quadrants (4, 3, 5)
                //
                if (rect >= inner_boundaries_[0] && rect <= inner_boundaries_[1])
                    quad = 4 ;
                else if (rect > inner_boundaries_[1] && rect <= inner_boundaries_[2])
                    quad = 3 ;
                else if (rect > inner_boundaries_[2] && rect <= inner_boundaries_[3])
                    quad = 5 ;                
            }

            return quad ;
        }

        void DriveByVisionAction::evaluatePosition() {
            int quad = computeQuadrant() ;

            switch(quad) {
            case 1:
                state_ = State::DriveFirstSCurve ;
                scurve_left_offset_ = -scurve_turn_offset_ ;
                scurve_right_offset_ = scurve_turn_offset_ ;
                cycles_ = scurve_cycles_ ;
                break ;

            case 2:
                state_ = State::DriveFirstSCurve ;
                scurve_left_offset_ = scurve_turn_offset_ ;
                scurve_right_offset_ = -scurve_turn_offset_ ;
                cycles_ = scurve_cycles_ ;
                break ;            

            case 3:
                state_ = State::FinalDriveByYaw ;
                break ;

            default:
                state_ = State::Done ;
                setMotorsToPercents(0.0, 0.0) ;
                MessageLogger &logger = getTankDrive().getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_VISION_DRIVING) ;
                logger << "quit due to quadrant " << quad ;
                logger.endMessage() ;                
                break ;
            }
        }

        /// \brief Manage the action; called each time through the robot loop
        void DriveByVisionAction::run() {
            double dist = camera_.getDistance() ;

            switch(state_) {
            case State::InitialDriveByYaw:
                driveByYaw() ;
                if (std::fabs(camera_.getYaw()) < yaw_threshold_) {
                    //
                    // We are pointing enough at the target that we can evaluate our position
                    //
                    evaluatePosition() ;
                }
                break ;

            case State::DriveFirstSCurve:
                if (driveSCurve()) {
                    state_ = State::DriveSecondSCurve ;
                    scurve_left_offset_ = -scurve_left_offset_ ;
                    scurve_right_offset_ = -scurve_right_offset_ ;
                    cycles_ = scurve_cycles_ ;
                }
                break ;

            case State::DriveSecondSCurve:
                if (driveSCurve())
                    state_ = State::FinalDriveByYaw ;
                break ;

            case State::FinalDriveByYaw:
                driveByYaw() ;
                if (dist < quit_threshold_) {
                    state_ = State::Done ;
                    setMotorsToPercents(0.0, 0.0) ;
                }
                break ;

            case State::Done:
                setMotorsToPercents(0.0, 0.0) ;
                break ;                
            }
        }
        
        /// \brief Cancel the action
        void DriveByVisionAction::cancel() { 
            state_ = State::Done ;
            setMotorsToPercents(0, 0) ;           
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
