#include "CompleteLSHatchCollect.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "lifter/LifterGoToHeightAction.h"
#include "hatchholder/HatchHolderAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CompleteLSHatchCollect::CompleteLSHatchCollect(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {
        
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            set_lifter_shift_up_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:LS:hatch:shift_up", true) ;

            set_extend_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_ARM) ;
            set_retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM) ;
            set_retract_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER) ;
            set_deploy_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_FINGER) ;
        }

        CompleteLSHatchCollect::~CompleteLSHatchCollect() {
        }
        
        void CompleteLSHatchCollect::start() {
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            hatch_holder->setAction(set_retract_hatch_finger_) ;            

            state_ = State::RetractFinger ;
        }

        void CompleteLSHatchCollect::run(){
            auto lifter = getGamePiece().getLifter() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            switch(state_) {
            case State::RetractFinger:
                if (set_retract_hatch_finger_->isDone()) {
                    hatch_holder->setAction(set_extend_arm_) ;
                    state_ = State::ExtendArm ;
                }
                break ;

            case State::ExtendArm:
                if(set_extend_arm_->isDone()){
                    lifter->setAction(set_lifter_shift_up_height_) ;
                    state_ = State::LifterShiftUpHeight ;
                }
                break ;
            
            case State::LifterShiftUpHeight:
                if(set_lifter_shift_up_height_->isDone()){
                    hatch_holder->setAction(set_deploy_hatch_finger_) ;
                    state_ = State::DeployFinger ;
                }
                break ;
            
            case State::DeployFinger:
                if(set_deploy_hatch_finger_->isDone()){
                    hatch_holder->setAction(set_retract_arm_) ;
                    state_ = State::RetractArm ;
                }
                break ;
                
            case State::RetractArm:
                if(set_retract_arm_->isDone()){
                    state_ = State::Idle ;
                }
                break ;
                
            case State::Idle:
                break ;
            }
        }

        bool CompleteLSHatchCollect::isDone() {
            return state_ == State::Idle ;
        }

        void CompleteLSHatchCollect::cancel() {
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            hatch_holder->setAction(set_retract_arm_) ;            
            state_ = State::RetractArm ;
        }

        std::string CompleteLSHatchCollect::toString() {
            std::string ret("CompleteLSHatchCollect") ;
            return ret ;
        }
    }
}