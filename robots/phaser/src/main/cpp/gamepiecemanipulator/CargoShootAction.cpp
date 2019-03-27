#include "CargoShootAction.h"
#include "cargoholder/CargoHolder.h"
#include "turntable/TurntableGoToAngleAction.h"
#include "turntable/TurntableCalibrateAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterPowerAction.h>
#include <lifter/LifterCalibrateAction.h>
#include <oi/DriverGamepadRumbleAction.h>
#include <Robot.h>
#include <xeromath.h>
#include <iostream>
#include <cmath>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CargoShootAction::CargoShootAction(GamePieceManipulator &subsystem, PhaserCameraTracker &vision):GamePieceAction(subsystem), vision_(vision) 
        {
            auto turntable = getGamePiece().getTurntable();
            auto cargo_holder = getGamePiece().getCargoHolder() ;
            auto oi = getGamePiece().getRobot().getOI() ; 

            set_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, "cargoholder:score:power", "cargoholder:score:time") ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
            rumble_ = std::make_shared<DriverGamepadRumbleAction>(*oi, true, 1, 1.0, 1.0) ;   

            width_ = subsystem.getRobot().getSettingsParser().getDouble("cargotracking:width") ;

            state_ = State::Idle ;
        }

        CargoShootAction::~CargoShootAction(){
        }

        void CargoShootAction::start() {
            if (vision_.isValid()) {
                auto turntable = getGamePiece().getTurntable() ;

                state_ = State::Rotating ;
                double d = vision_.getDistance() ;
                double angle = std::atan(width_ / d)  + turntable->getAngleValue() ;
                rotate_turntable_ = std::make_shared<TurntableGoToAngleAction>(*turntable, angle) ;
                turntable->setAction(rotate_turntable_) ;
            }
            else {
                state_ = State::Idle ;
            }
        }

        void CargoShootAction::run() {
            auto cargo_holder = getGamePiece().getCargoHolder() ;

            switch(state_) {
            case State::Rotating:
                if (rotate_turntable_->isDone()) {
                    cargo_holder->setAction(set_cargo_holder_motor_) ;
                    state_ = State::Shooting ;
                }
                break ;
            case State::Shooting:
                if (set_cargo_holder_motor_->isDone()) {
                    cargo_holder->setAction(stop_cargo_holder_motor_) ;
                    state_ = State::Idle ;
                }
            case State::Idle:
                break ;
            }
        }

        bool CargoShootAction::isDone() {
            return state_ == State::Idle ;
        }

        void CargoShootAction::cancel() {
        }

        std::string CargoShootAction::toString() {
            std::string ret("CargoShootAction") ;
            return ret ;
        }
    }
}