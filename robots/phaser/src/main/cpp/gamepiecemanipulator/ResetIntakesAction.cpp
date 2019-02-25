#include "ResetIntakesAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <turntable/TurntableGoToAngleAction.h>
#include <cargointake/CargoIntake.h>
#include <cargointake/CargoIntakeAction.h>
#include <hatchintake/HatchIntake.h>
#include <hatchintake/HatchIntakeAction.h>
#include <hatchholder/HatchHolderAction.h>
#include <cmath>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ResetIntakeAction::ResetIntakeAction(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {

            auto lifter = getGamePiece().getLifter() ; 
            auto turntable = getGamePiece().getTurntable() ;
            auto hatch_intake = getGamePiece().getHatchIntake();
            auto cargo_intake = getGamePiece().getCargoIntake() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto cargo_holder = getGamePiece().getCargoHolder() ;

            set_lifter_safe_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:safe_turn") ;
            set_lifter_level_one_  = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:bottom") ;
            set_turntable_zero_ = std::make_shared<TurntableGoToAngleAction>(*turntable, 0.0) ;
            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, false) ;
            retract_hatch_intake_ = std::make_shared<HatchIntakeAction>(*hatch_intake, false) ;
            retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM, "hatcholder:default:delay") ;
            retract_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER, "hatchholder:default:delay") ;

            stop_hatch_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*hatch_intake, 0.0) ;
            stop_cargo_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_intake, 0.0) ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;

            state_ = State::Idle ;           
        }

        ResetIntakeAction::~ResetIntakeAction() {
        }

        void ResetIntakeAction::start() {
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable() ;
            auto hatch_intake = getGamePiece().getHatchIntake();
            auto cargo_intake = getGamePiece().getCargoIntake() ;
            auto cargo_holder = getGamePiece().getCargoHolder() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            hatch_intake->setAction(stop_hatch_intake_motor_) ;
            cargo_intake->setAction(stop_cargo_intake_motor_) ;
            cargo_holder->setAction(stop_cargo_holder_motor_) ;

            if (hatch_intake->isDeployed() || cargo_intake->isDeployed() || std::fabs(turntable->getAngleValue()) > epsilon) {
                lifter->setAction(set_lifter_safe_height_) ;
                state_ = State::SafeHeight ;
            }
            else if (hatch_holder->isArmDeployed()) {
                hatch_holder->setAction(retract_arm_) ;
                state_ = State::RetractArm ;

            }
            else if (hatch_holder->isFingerDepoyed()) {
                hatch_holder->setAction(retract_finger_) ;
                state_ = State::RetractFinger ;
            }
            else {
                lifter->setAction(set_lifter_level_one_) ;
                state_ = State::FinalHeight ;
            }
        }

        void ResetIntakeAction::run() {
            switch(state_) {
            case State::SafeHeight:
                if (set_lifter_safe_height_->isDone()) {
                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(set_turntable_zero_) ;
                    state_ = State::RotateZero ;
                }
                break ;
            case State::RotateZero:
                if (set_turntable_zero_->isDone()) {
                    auto hatch_intake = getGamePiece().getHatchIntake();
                    hatch_intake->setAction(retract_hatch_intake_) ;

                    auto cargo_intake = getGamePiece().getCargoIntake();  
                    cargo_intake->setAction(retract_cargo_intake_) ;

                    state_ = State::RetractIntakes ;
                }
                break ;

            case State::RetractIntakes:
                if (retract_hatch_intake_->isDone() && retract_cargo_intake_->isDone()) {
                    auto hatch_holder = getGamePiece().getHatchHolder() ;
                    if (hatch_holder->isArmDeployed()) {
                        hatch_holder->setAction(retract_arm_) ;
                        state_ = State::RetractArm ;
                    }
                    else if (hatch_holder->isFingerDepoyed()) {
                        hatch_holder->setAction(retract_finger_) ;
                        state_ = State::RetractFinger ;
                    }
                    else {
                        auto lifter = getGamePiece().getLifter() ;
                        lifter->setAction(set_lifter_level_one_) ;
                        state_ = State::FinalHeight ;
                    }
                }
                break ;

            case State::RetractArm:
                if (retract_arm_->isDone()) {
                    auto hatch_holder = getGamePiece().getHatchHolder() ;
                    if (hatch_holder->isFingerDepoyed()) {
                        hatch_holder->setAction(retract_finger_) ;
                        state_ = State::RetractFinger ;
                    }
                    else {
                        auto lifter = getGamePiece().getLifter() ;
                        lifter->setAction(set_lifter_level_one_) ;
                        state_ = State::FinalHeight ;                                                
                    }
                }
                break ;

            case State::RetractFinger:
                if (retract_finger_->isDone()) {
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_level_one_) ;
                    state_ = State::FinalHeight ;                          
                }

            case State::FinalHeight:
                if (set_lifter_level_one_->isDone()) 
                    state_ = State::Idle ;

            case State::Idle:
                break ;
            }
        }
        
        bool ResetIntakeAction::isDone() {
            return state_ == State::Idle ;
        }

        void ResetIntakeAction::cancel() {
        }

        std::string ResetIntakeAction::toString() {
            std::string ret("ResetIntakeAction") ;
            return ret ;
        }
    }
}