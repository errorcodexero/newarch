#include "ScoreHatch.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "hatchholder/HatchHolderAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ScoreHatch::ScoreHatch(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto lifter = getGamePiece().getLifter() ;
            
            set_lifter_shift_down_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:score:hatch:shift_down", true) ;

            set_extend_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_ARM, "hatchholder:place:delay") ;
            set_retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM, "hatcholder:default:delay") ;
            set_retract_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER, "hatcholder:default:delay") ;
            set_deploy_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_FINGER, "hatcholder:default:delay") ;
        }

        ScoreHatch::~ScoreHatch(){
        }

        void ScoreHatch::start() {
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            hatch_holder->setAction(set_extend_arm_) ;
            state_ = State::ExtendArm ;
        }

        void ScoreHatch::run(){
            auto lifter = getGamePiece().getLifter() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            switch(state_) {
            case State::ExtendArm:
                if (set_extend_arm_->isDone()) {
                    auto hatch_holder = getGamePiece().getHatchHolder() ;                    
                    hatch_holder->setAction(set_retract_hatch_finger_) ;
                    state_ = State::ReleaseFinger ;
                }
                break ;

            case State::ReleaseFinger:
                if (set_retract_hatch_finger_->isDone()) {
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_shift_down_height_) ;
                    state_ = State::LifterShiftDownHeight ;
                }
                break ;

            case State::LifterShiftDownHeight:
                if(set_lifter_shift_down_height_->isDone()) {
                    hatch_holder->setAction(set_retract_arm_) ;
                    state_ = State::RetractArm ;
                }
                break ;
                
            case State::RetractArm:
                if(set_retract_arm_->isDone()) {
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
        }

        std::string ScoreHatch::toString() {
            std::string ret("ScoreHatch") ;
            return ret ;
        }
    }
}