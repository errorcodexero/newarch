#include "ScoreHatch.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <lifter/LifterHoldHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "hatchholder/HatchHolderAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ScoreHatch::ScoreHatch(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {
        
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            
            set_lifter_score_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:score_hatch") ;
            hold_lifter_score_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:score_hatch") ;
            set_turntable_cargo_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "turntable:angle:score:hatch") ;
            set_lifter_shift_down_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:score_hatch:shift_down") ;

            set_extend_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_ARM) ;
            set_retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM) ;
            set_retract_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER) ;
            set_deploy_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_FINGER) ;
        }

        ScoreHatch::~ScoreHatch(){
        }

        void ScoreHatch::start() {
         
            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(set_lifter_score_height_) ; 

            state_ = State::LifterGoToScoreHeight ;
            
        }

        void ScoreHatch::run(){
            auto lifter = getGamePiece().getLifter() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            switch(state_) {
            case State::LifterGoToScoreHeight:
                if(set_lifter_score_height_->isDone()){
                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(set_turntable_cargo_angle_) ;
                    state_ = State::TurntableGoToAngle ; 
                }
                break ;

            case State::TurntableGoToAngle:
                if(set_turntable_cargo_angle_->isDone()){
                    hatch_holder->setAction(set_extend_arm_) ;
                    state_ = State::ExtendArm ;
                }
                break ;
            
            case State::ExtendArm:
                if(set_extend_arm_->isDone()){
                    hatch_holder->setAction(set_retract_hatch_finger_) ;
                    state_ = State::ReleaseFinger ;
                }
                break ;
            
            case State::ReleaseFinger:
                if(set_retract_hatch_finger_->isDone()){
                    lifter->setAction(set_lifter_shift_down_height_) ;
                    state_ = State::LifterShiftDownHeight ;
                }
                break ;

            case State::LifterShiftDownHeight:
                if(set_lifter_shift_down_height_->isDone()){
                    hatch_holder->setAction(set_retract_arm_) ;
                    state_ = State::RetractArm ;
                }
                break ;
                
            case State::RetractArm:
                if(set_retract_arm_->isDone()){
                    state_ = State::CheckHolder ;
                }
                break ;
                
            case State::CheckHolder:
                if(hatch_holder->hasHatch()){
                    hatch_holder->setAction(set_deploy_hatch_finger_) ;
                    state_ = State::Idle ;
                }
                break ;
                
            case State::Idle:
                break ;
            }
        }

        bool ScoreHatch::isDone() {
            return state_ == State::Idle ;
        }

        void ScoreHatch::cancel() {
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto lifter = getGamePiece().getLifter() ;

            switch(state_) {
            case State::TurntableGoToAngle:
                state_ = State::Idle ;
                break ;

            case State::LifterGoToScoreHeight:
                state_ = State::Idle ;
                break ;

            case State::ExtendArm:
                hatch_holder->setAction(set_retract_arm_) ;
                state_ = State::ReleaseFinger ;
                break ;

            case State::ReleaseFinger:
                hatch_holder->setAction(set_retract_hatch_finger_) ;
                state_ = State::LifterShiftDownHeight ;
                break ;

            case State::LifterShiftDownHeight: 
                lifter->setAction(set_lifter_shift_down_height_) ;
                state_ = State::RetractArm ;
                break ;

            case State::RetractArm:
                hatch_holder->setAction(set_retract_arm_) ;
                state_ = State::CheckHolder ;
                break ;

            case State::CheckHolder:
                if(hatch_holder->hasHatch()){
                    hatch_holder->setAction(set_deploy_hatch_finger_) ;
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }

        std::string ScoreHatch::toString() {
            std::string ret("ScoreHatch") ;
            return ret ;
        }
    }
}