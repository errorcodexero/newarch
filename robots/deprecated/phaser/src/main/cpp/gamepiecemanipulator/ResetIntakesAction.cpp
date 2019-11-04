#include "ResetIntakesAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <turntable/TurntableGoToAngleAction.h>
#include <cargointake/CargoIntake.h>
#include <cargointake/CargoIntakeAction.h>
#include <carloshatch/CarlosHatchArmAction.h>
#include "ReadyAction.h"
#include <cmath>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ResetIntakeAction::ResetIntakeAction(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {

            auto lifter = getGamePiece().getLifter() ; 
            auto turntable = getGamePiece().getTurntable() ;
            auto cargo_intake = getGamePiece().getCargoIntake() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto cargo_holder = getGamePiece().getCargoHolder() ;

            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, false) ;
            retract_arm_ = std::make_shared<CarlosHatchArmAction>(*hatch_holder, CarlosHatchArmAction::Operation::RETRACT) ;

            stop_cargo_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_intake, 0.0) ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;

            goto_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:turtle") ;
            goto_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "turntable:angle:hatch:collect:north") ;

            state_ = State::Idle ;           
        }

        ResetIntakeAction::~ResetIntakeAction() {
        }

        void ResetIntakeAction::start() {
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable() ;
            auto cargo_intake = getGamePiece().getCargoIntake() ;
            auto cargo_holder = getGamePiece().getCargoHolder() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            cargo_intake->setAction(stop_cargo_intake_motor_) ;
            cargo_holder->setAction(stop_cargo_holder_motor_) ;
            hatch_holder->setAction(retract_arm_) ;

            lifter->setAction(goto_height_) ;
            state_ = State::GoToHeight ;
        }

        void ResetIntakeAction::run() {
            switch(state_) {
            case State::GoToHeight:
                if (goto_height_->isDone()) {
                    getGamePiece().getTurntable()->setAction(goto_angle_) ;
                    state_ = State::GoToAngle ;
                }
                break ;

            case State::GoToAngle:
                if (goto_angle_->isDone()) {
                    getGamePiece().getCargoIntake()->setAction(retract_cargo_intake_) ;
                    state_ = State::WaitForIntake ;                    
                }
                break ;
            case State::WaitForIntake:
                if (retract_cargo_intake_->isDone()) {
                    state_ = State::Idle ;
                }
                break;
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