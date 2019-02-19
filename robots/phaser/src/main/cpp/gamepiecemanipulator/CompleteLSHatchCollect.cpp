#include "CompleteLSHatchCollect.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <lifter/LifterHoldHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "hatchholder/HatchHolderAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        CompleteLSHatchCollect::CompleteLSHatchCollect(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {
        
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            set_lifter_collect_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:LS:hatch") ;
            hold_lifter_collect_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:LS_hatch") ;
            set_turntable_hatch_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "turntable:angle:hatch:LS") ;
            set_lifter_shift_up_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:LS:hatch:shift_up") ;

            set_extend_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_ARM) ;
            set_retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM) ;
            set_retract_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER) ;
            set_deploy_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_FINGER) ;
        }
        CompleteLSHatchCollect::~CompleteLSHatchCollect() {
        }

        
        void CompleteLSHatchCollect::start() {
         
            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(set_lifter_collect_height_) ; 

            state_ = State::LifterGoToCollectHeight ;
            
        }

        void CompleteLSHatchCollect::run(){
            auto lifter = getGamePiece().getLifter() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            switch(state_) {
            case State::LifterGoToCollectHeight:
                if(set_lifter_collect_height_->isDone()){
                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(set_turntable_hatch_angle_) ;
                    state_ = State::TurntableGoToAngle ; 
                }
                break ;

            case State::TurntableGoToAngle:
                if(set_turntable_hatch_angle_->isDone()){
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
                    state_ = State::CheckHolder ;
                }
                break ;
                
            case State::CheckHolder:
                if(!hatch_holder->hasHatch()){
                    hatch_holder->setAction(set_retract_hatch_finger_) ;
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
            auto lifter = getGamePiece().getLifter() ;

            switch(state_) {
            case State::TurntableGoToAngle:
                state_ = State::Idle ;
                break ;

            case State::LifterGoToCollectHeight:
                state_ = State::Idle ;
                break ;

            case State::ExtendArm:
                hatch_holder->setAction(set_retract_arm_) ;
                state_ = State::Idle ;
                break ;

            case State::LifterShiftUpHeight: 
                lifter->setAction(set_lifter_collect_height_) ;
                hatch_holder->setAction(set_retract_arm_) ;
                state_ = State::CheckHolder ;
                break ;

            case State::DeployFinger:
                hatch_holder->setAction(set_retract_hatch_finger_) ;
                lifter->setAction(set_lifter_collect_height_) ;
                hatch_holder->setAction(set_retract_arm_) ;
                state_ = State::CheckHolder ;
                break ;

            case State::RetractArm:
                hatch_holder->setAction(set_retract_hatch_finger_) ;
                lifter->setAction(set_lifter_collect_height_) ;
                hatch_holder->setAction(set_retract_arm_) ;
                state_ = State::CheckHolder ;
                break ;

            case State::CheckHolder:
                if(!hatch_holder->hasHatch()){
                    hatch_holder->setAction(set_retract_hatch_finger_) ;
                    state_ = State::Idle ;
                }
                if(hatch_holder->hasHatch()) {
                    hatch_holder->setAction(set_deploy_hatch_finger_) ;
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }

        std::string CompleteLSHatchCollect::toString() {
            std::string ret("CompleteLSHatchCollect") ;
            return ret ;
        }
    }
}