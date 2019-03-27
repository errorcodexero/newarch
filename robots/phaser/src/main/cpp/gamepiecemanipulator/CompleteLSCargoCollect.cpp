#include "CompleteLSCargoCollect.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <oi/OISubsystem.h>
#include <oi/DriverGamepadRumbleAction.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CompleteLSCargoCollect::CompleteLSCargoCollect(GamePieceManipulator &subsystem):GamePieceAction(subsystem)
        {
            auto cargo_holder = getGamePiece().getCargoHolder();
            auto oi = getGamePiece().getRobot().getOI() ;

            set_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, "cargoholder:collect:power") ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
            rumble_ = std::make_shared<DriverGamepadRumbleAction>(*oi, true, 1, 1.0, 1.0) ;

            state_ = State::Idle ;
        }

        CompleteLSCargoCollect::~CompleteLSCargoCollect() {
        }

        void CompleteLSCargoCollect::start() {
            auto cargo_holder = getGamePiece().getCargoHolder() ; 
            cargo_holder->setAction(set_cargo_holder_motor_) ;

            state_ = State::WaitForCargo ;
        }

        void CompleteLSCargoCollect::run() {
            auto cargo_holder = getGamePiece().getCargoHolder() ;

            switch(state_) {
            case State::WaitForCargo:
                if (set_cargo_holder_motor_->isDone()) {
                    if (cargo_holder->hasCargo()) {
                        cargo_holder->setAction(stop_cargo_holder_motor_) ;
                        state_ = State::StopAllMotors ; 
                    }
                }
                break ;

            case State::StopAllMotors:
                if (stop_cargo_holder_motor_->isDone()) {
                    auto oi = getGamePiece().getRobot().getOI() ;
                    oi->setAction(rumble_) ;
                    state_ = State::Idle ;
                }

            case State::Idle:
                break ;
            }
        }
        
        bool CompleteLSCargoCollect::isDone() {
            return state_ == State::Idle ;
        }

        void CompleteLSCargoCollect::abort() {
            state_ = State::Idle ;
        }

        void CompleteLSCargoCollect::cancel() {
            auto cargo_intake = getGamePiece().getCargoIntake() ;
            auto cargo_holder = getGamePiece().getCargoHolder() ;            

            switch(state_) {

            case State::WaitForCargo:
                cargo_holder->setAction(stop_cargo_holder_motor_) ;                
                state_ = State::StopAllMotors ;   
                break ;

            case State::StopAllMotors:
                break ;

            case State::Idle:           
                break ;
            }
        }

        std::string CompleteLSCargoCollect::toString() {
            std::string ret("CompleteLSCargoCollect") ;
            return ret ;
        }
    }
}