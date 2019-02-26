#include "CompleteLSCargoCollect.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CompleteLSCargoCollect::CompleteLSCargoCollect(GamePieceManipulator &subsystem):GamePieceAction(subsystem)
        {
            auto cargo_holder = getGamePiece().getCargoHolder();

            set_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, "cargoholder:collect:power") ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
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
                if (set_cargo_holder_motor_->isDone() && cargo_holder->hasCargo()) {
       
                    cargo_holder->setAction(stop_cargo_holder_motor_) ;
                    state_ = State::StopAllMotors ;
                }
                break ;

            case State::StopAllMotors:
                if (stop_cargo_holder_motor_->isDone()) {

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