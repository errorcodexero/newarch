#include "StrafeAction.h"
#include "gamepiecemanipulator/ScoreCargo.h"
#include "phaserids.h"
#include <tankdrive/TankDrive.h>
#include <tankdrive/TankDriveDistanceAction.h>
#include <tankdrive/TankDrivePowerAction.h>
#include <tankdrive/TankDriveTimedPowerAction.h>
#include <oi/DriverGamepadRumbleAction.h>
#include <MessageLogger.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        int StrafeAction::which_one_ = 0 ;

        StrafeAction::StrafeAction(PhaserRobotSubsystem &subsystem): subsystem_(subsystem)
        {
            auto db = subsystem_.getTankDrive() ;
            auto game = subsystem_.getGameManipulator() ;
            auto oi = subsystem_.getOI() ;

            ship_ = false ;
            count_ = 1 ;
            state_ = State::Idle ;

            shoot_dist_ = subsystem.getRobot().getSettingsParser().getDouble("strafe:rocket:shoot_distance") ;
            vel_factor_ = subsystem.getRobot().getSettingsParser().getDouble("strafe:rocket:velocity_factor") ;    

            shoot_ = std::make_shared<ScoreCargo>(*game) ;
            rumble_ = std::make_shared<DriverGamepadRumbleAction>(*oi, true, 1, 1.0, 1.0) ;

            which_ = which_one_++ ;
        }

        StrafeAction::StrafeAction(PhaserRobotSubsystem &subsystem, int count): subsystem_(subsystem)
        {
            auto db = subsystem_.getTankDrive() ;
            auto game = subsystem_.getGameManipulator() ;
            auto oi = subsystem_.getOI() ;

            count_ = count ;
            state_ = State::Idle ;
            ship_ = true ;

            shoot_dist_ = subsystem.getRobot().getSettingsParser().getDouble("strafe:ship:shoot_distance") ;
            vel_factor_ = subsystem.getRobot().getSettingsParser().getDouble("strafe:ship:velocity_factor") ;

            shoot_ = std::make_shared<ScoreCargo>(*game) ;
            rumble_ = std::make_shared<DriverGamepadRumbleAction>(*oi, true, 1, 1.0, 1.0) ;

            which_ = which_one_++ ;            
        }

        StrafeAction::~StrafeAction() {
        }

        void StrafeAction::start() {
            if (ship_)
                state_ = State::ArmedShip ;
            else
                state_ = State::ArmedRocket ;

            thiscount_ = count_ ;
            front_detected_ = false ;
            back_detected_ = false ;
        }

        bool StrafeAction::isLineDetectedFront() {
            bool ret = false ;
            auto front = subsystem_.getFrontLineSensor() ;

            if (front_detected_ && !front->detectedObject()) {
                ret = true ;
                front_detected_ = false ;
            }
            else if (front->detectedObject()) {
                front_detected_ = true ;
            }

            return ret ;
        }

        bool StrafeAction::isLineDetectedBack() {
            bool ret = false ;
            auto back = subsystem_.getBackLineSensor() ;

            if (back_detected_ && !back->detectedObject()) {
                ret = true ;
                back_detected_ = false ;
            }
            else if (back->detectedObject()) {
                back_detected_ = true ;
            }

            return ret ;
        }        

        void StrafeAction::run() {
            MessageLogger &logger = subsystem_.getRobot().getMessageLogger() ;
            auto db = subsystem_.getTankDrive() ;
            auto game = subsystem_.getGameManipulator() ;
            auto oi = subsystem_.getOI() ;
            bool front = isLineDetectedFront() ;
            bool back = isLineDetectedBack() ;
            // double traveled = db->getDist() - start_dist_ ;

            switch(state_) {
            case State::ArmedRocket:
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_STRAFE) ;
                logger << "Strafe ArmedRocket" ;
                logger.endMessage() ;
                if (front && db->getVelocity() > 0) {
                    start_dist_ = db->getDist() ;
                    target_dist_ = shoot_dist_ - vel_factor_ * db->getVelocity() ;
                    state_ = State::WaitForDistanceForward ;
                }
                else if (back && db->getVelocity() < 0) {
                    start_dist_ = db->getDist() ;
                    target_dist_ = -shoot_dist_ - vel_factor_ * db->getVelocity() ;
                    state_ = State::WaitForDistanceBack ;
                }
                break ;

            case State::ArmedShip: 
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_STRAFE) ;
                logger << "Strafe ArmedShip" ;
                logger.endMessage() ;
                if (front && db->getVelocity() > 0) {
                    thiscount_-- ;

                    if (thiscount_ == 0) {
                        start_dist_ = db->getDist() ;
                        target_dist_ = shoot_dist_ - vel_factor_ * db->getVelocity() ;                        
                        state_ = State::WaitForDistanceForward ;
                    }
                }
                else if (front && db->getVelocity() < 0) {
                    thiscount_++ ;
                }
                break ;

            case State::WaitForDistanceForward:
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_STRAFE) ;
                logger << "Strafe WaitForDistanceForward" ;
                logger << " start " << start_dist_ ;
                logger << " current " << db->getDist() ;
                logger << " target " << target_dist_ ;
                logger << " velocity " << db->getVelocity() ;
                logger.endMessage() ;         
      

                if (db->getDist() - start_dist_ > target_dist_) {
                    game->setAction(shoot_);
                    state_ = State::Shooting ;
                }
                else if (front && db->getVelocity() < 0 && ship_) {
                    thiscount_++ ;
                    state_ = State::ArmedShip ;
                }

                //start_dist_ = db->getDist() ;
                //target_dist_ = shoot_dist_ - traveled - vel_factor_ * db->getVelocity() ;
                break ;

            case State::WaitForDistanceBack:
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_STRAFE) ;
                logger << "Strafe WaitForDistanceBack" ;
                logger << " start " << start_dist_ ;
                logger << " current " << db->getDist() ;
                logger << " target " << target_dist_ ;
                logger << " velocity " << db->getVelocity() ;
                logger.endMessage() ;  ;            
                if (db->getDist() - start_dist_ < target_dist_) {
                    game->setAction(shoot_);
                    state_ = State::Shooting ;
                }

                //start_dist_ = db->getDist() ;
                //target_dist_ = -shoot_dist_ + traveled - vel_factor_ * db->getVelocity() ;
                break ;

            case State::Shooting:
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_STRAFE) ;
                logger << "Strafe Shooting" ;
                logger.endMessage() ;            
                if (shoot_->isDone()) {
                    oi->setAction(rumble_) ;
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_STRAFE) ;
                logger << "Strafe Idle" ;
                logger.endMessage() ;    
                break ;
            }
        }
        
        bool StrafeAction::isDone() {
            return state_ == State::Idle ;
        }

        void StrafeAction::abort() {
            state_ = State::Idle ;
        }

        void StrafeAction::cancel() {
        }

        std::string StrafeAction::toString() {
            std::string ret("StrafeAction") ;
            return ret ;
        }
    }
}