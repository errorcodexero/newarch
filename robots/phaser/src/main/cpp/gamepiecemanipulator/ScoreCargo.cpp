# include "ScoreCargo.h"
# include <singlemotorsubsystem/SingleMotorPowerAction.h>
# include "turntable/TurntableGoToAngleAction.h"
# include "cargoholder/CargoHolder.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
       ScoreCargo::ScoreCargo(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {

            auto lifter = getGamePiece().getLifter() ; 
            auto turntable = getGamePiece().getTurntable();
            auto cargo_holder = getGamePiece().getCargoHolder() ;

            set_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, "cargoholder:score:power", "cargoholder:score:time") ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
       }

       ScoreCargo::~ScoreCargo() {
        }

        void ScoreCargo::start() {
            auto cargo_holder = getGamePiece().getCargoHolder() ;
            cargo_holder->setAction(set_cargo_holder_motor_) ;
            state_ = State::ScoreCargo ;
        }

        void ScoreCargo::run() {
            auto lifter = getGamePiece().getLifter() ;

            switch(state_) {
            case State::ScoreCargo:
                if (set_cargo_holder_motor_->isDone()) {
                    auto cargo_holder = getGamePiece().getCargoHolder() ;
                    cargo_holder->setAction(stop_cargo_holder_motor_) ;
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }
        
        bool ScoreCargo::isDone() {
            return state_ == State::Idle ;
        }

        void ScoreCargo::cancel() {
            auto cargo_holder = getGamePiece().getCargoHolder() ;
            cargo_holder->setAction(stop_cargo_holder_motor_) ;
            state_ = State::Idle ;            
        }

        std::string ScoreCargo::toString() {
            std::string ret("ScoreCargo") ;
            return ret ;
        }
    }
}