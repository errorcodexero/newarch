# include "ScoreCargo.h"
# include <singlemotorsubsystem/SingleMotorPowerAction.h>
# include <lifter/LifterGoToHeightAction.h>
# include <lifter/LifterHoldHeightAction.h>
# include "turntable/TurntableGoToAngleAction.h"
# include "cargoholder/CargoHolder.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
       ScoreCargo::ScoreCargo(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {

            auto lifter = getGamePiece().getLifter() ; 
            auto turntable = getGamePiece().getTurntable();
            auto cargo_holder = getGamePiece().getCargoHolder() ;

            set_lifter_score_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:score:cargo") ;
            hold_lifter_score_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:score:cargo") ;
            set_turntable_cargo_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "turntable:angle:score:cargo") ;
            
            set_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, "score_cargo:power", "score_cargo:time") ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
       }

       ScoreCargo::~ScoreCargo() {
        }

        void ScoreCargo::start() {

            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(set_lifter_score_height_) ;

            state_ = State::LifterGoToScoreHeight ;
        }

        void ScoreCargo::run() {
            auto lifter = getGamePiece().getLifter() ;

            switch(state_) {
            case State::LifterGoToScoreHeight:
                if (set_lifter_score_height_->isDone()) {

                    auto turntable = getGamePiece().getTurntable();
                    turntable->setAction(set_turntable_cargo_angle_) ;
                    lifter->setAction(hold_lifter_score_height_) ;

                    state_ = State::TurntableGoToAngle ;
                }
                break ;

            case State::TurntableGoToAngle:
                if (set_turntable_cargo_angle_->isDone()) {

                    state_ = State::ScoreCargo ;
                }
                break ;

            case State::ScoreCargo:
                if (set_lifter_score_height_->isDone() && set_turntable_cargo_angle_->isDone()) {

                    auto cargo_holder = getGamePiece().getCargoHolder() ;
                    cargo_holder->setAction(set_cargo_holder_motor_) ;

                    state_ = State::StopMotor ;
                }
                break ;

            case State::StopMotor:
                if (set_cargo_holder_motor_->isDone()){
                    
                    auto cargo_holder = getGamePiece().getCargoHolder() ;
                    cargo_holder->setAction(stop_cargo_holder_motor_) ;
                    state_ = State::Idle;
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

            switch(state_) {
            case State::LifterGoToScoreHeight:
                // The lifter is changing heights, but that is ok, let the lift
                // finish its height action
                state_ = State::Idle ;
                break ;

            case State::TurntableGoToAngle:
                // The turntable is changing angles, but that is ok, let the lift
                // finish its height action
                state_ = State::Idle ;            
                break ;

            case State::ScoreCargo:
                cargo_holder->setAction(stop_cargo_holder_motor_) ;
                state_ = State::Idle ;
                break ;     

            case State::StopMotor:
                state_ = State::Idle ;

            case State::Idle:
                // We are already putting things away, just let it finish            
                break ;
            }
        }

        std::string ScoreCargo::toString() {
            std::string ret("ScoreCargo") ;
            return ret ;
        }
    }
}