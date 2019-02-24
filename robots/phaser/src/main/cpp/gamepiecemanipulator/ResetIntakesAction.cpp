#include "ResetIntakesAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <turntable/TurntableGoToAngleAction.h>
#include <cargointake/CargoIntake.h>
#include <cargointake/CargoIntakeAction.h>
#include <hatchintake/HatchIntake.h>
#include <hatchintake/HatchIntakeAction.h>
#include <hatchholder/HatchHolderAction.h>

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

            stop_hatch_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*hatch_intake, 0.0) ;
            stop_cargo_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_intake, 0.0) ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;

            state_ = State::Idle ;           
        }

        ResetIntakeAction::~ResetIntakeAction() {
        }

        void ResetIntakeAction::start() {
            auto lifter = getGamePiece().getLifter() ;
            auto hatch_intake = getGamePiece().getHatchIntake();
            auto cargo_intake = getGamePiece().getCargoIntake() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            if (!hatch_intake->isDeployed() && !cargo_intake->isDeployed() && !hatch_holder->isArmDeployed()) 
                state_ = State::StopMotors ;
            else 
            {
                lifter->setAction(set_lifter_safe_height_) ;
                state_ = State::Lifting ;
            }
        }

        void ResetIntakeAction::run() {
            switch(state_) {
            case State::Lifting:
                if (set_lifter_safe_height_->isDone()) {
                    auto hatch_intake = getGamePiece().getHatchIntake();
                    hatch_intake->setAction(retract_hatch_intake_) ;

                    auto cargo_intake = getGamePiece().getCargoIntake();  
                    cargo_intake->setAction(retract_cargo_intake_) ;

                    auto hatch_holder = getGamePiece().getHatchHolder() ;
                    hatch_holder->setAction(retract_arm_) ;

                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_turntable_zero_) ;

                    state_ = State::Retracting ;
                }
                break ;

            case State::Retracting:
                if (retract_hatch_intake_->isDone() && retract_cargo_intake_->isDone() && retract_arm_->isDone()) {
                    auto hatch_intake = getGamePiece().getHatchIntake() ;
                    auto cargo_intake = getGamePiece().getCargoIntake() ;
                    auto cargo_holder = getGamePiece().getCargoHolder() ;

                    hatch_intake->setAction(stop_hatch_intake_motor_) ;
                    cargo_intake->setAction(stop_cargo_intake_motor_) ;
                    cargo_holder->setAction(stop_cargo_holder_motor_) ;

                    state_ = State::StopMotors ;
                }
                break ;

            case State::StopMotors:
                if (stop_hatch_intake_motor_->isDone() && stop_cargo_intake_motor_->isDone() && 
                                stop_hatch_intake_motor_->isDone() && set_turntable_zero_->isDone()) {
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_level_one_) ;
                    state_ = State::DropLift ;
                }
                break ;

            case State::DropLift:
                if (set_lifter_level_one_->isDone())
                    state_ = State::Idle ;
                break  ;

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