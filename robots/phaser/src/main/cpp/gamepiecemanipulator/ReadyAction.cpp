#include "ReadyAction.h"
#include <lifter/Lifter.h>
#include <lifter/LifterGoToHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "hatchholder/HatchHolderAction.h"
#include <Robot.h>
#include <xeromath.h>
#include <cmath>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ReadyAction::ReadyAction(GamePieceManipulator &subsystem, const std::string &height, const std::string &angle):GamePieceAction(subsystem) {
        
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            height_ = height ;
            angle_ = angle ;

            set_lifter_safe_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:safe_turn") ;
            set_lifter_final_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, height) ;
            set_turntable_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, angle) ;
            extend_hatch_holder_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_ARM, "hatchholder:specialflow:delay") ;
            retract_hatch_holder_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM, "hatchholder:default:delay") ;

            safe_height_ = subsystem.getRobot().getSettingsParser().getDouble("lifter:height:safe_turn")  ;
        }

        ReadyAction::~ReadyAction(){
        }

        bool ReadyAction::alreadyOnCorrectSide() {
            auto turntable = getGamePiece().getTurntable() ;
            double diff = xero::math::normalizeAngleDegrees(turntable->getAngleValue() - set_turntable_angle_->getTargetAngle()) ;
            return std::fabs(diff) < 10.0 ;
        }

        bool ReadyAction::isSpecialHatchFlow() {
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto turntable = getGamePiece().getTurntable() ;
            double target_angle = set_turntable_angle_->getTargetAngle() ;

            if (!hatch_holder->hasHatch())
                return false ;

            return std::fabs(xero::math::normalizeAngleDegrees(target_angle - turntable->getAngleValue())) > 15.0 ;
        }

        void ReadyAction::start() {
            special_hatch_flow_ = false ;

            if (isSpecialHatchFlow()) {
                auto hatch_holder = getGamePiece().getHatchHolder() ;
                auto lifter = getGamePiece().getLifter() ;
                hatch_holder->setAction(extend_hatch_holder_) ;
                lifter->setAction(set_lifter_safe_height_) ;
                special_hatch_flow_ = true ;
                state_ = State::LifterSafeHeight ;
            }
            else {
                auto lifter = getGamePiece().getLifter() ;
                if (lifter->getHeight() > safe_height_ || alreadyOnCorrectSide()) {
                    //
                    // IF we are already above the safe height, do not go to the safe 
                    // height
                    //
                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(set_turntable_angle_) ;
                    state_ = State::TurntableGoToAngle ;
                }
                else {
                    lifter->setAction(set_lifter_safe_height_) ; 
                    state_ = State::LifterSafeHeight ;
                }
            }
        }

        void ReadyAction::run(){
            switch(state_) {
            case State::ExtendHatchHolder:
                if (extend_hatch_holder_->isDone()) {
                    auto lifter = getGamePiece().getLifter() ;
                    if (lifter->getHeight() > safe_height_ || alreadyOnCorrectSide()) {
                        //
                        // IF we are already above the safe height, do not go to the safe 
                        // height
                        //
                        auto turntable = getGamePiece().getTurntable() ;
                        turntable->setAction(set_turntable_angle_) ;
                        state_ = State::TurntableGoToAngle ;
                    }
                    else {
                        lifter->setAction(set_lifter_safe_height_) ; 
                        state_ = State::LifterSafeHeight ;
                    }                    
                }
                break ;

            case State::LifterSafeHeight:
                if(set_lifter_safe_height_->isDone()){
                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(set_turntable_angle_) ;

                    auto lifter = getGamePiece().getLifter() ;
                    state_ = State::TurntableGoToAngle ; 
                }
                break ;

            case State::TurntableGoToAngle:
                if(set_turntable_angle_->isDone()){
                    auto turntable = getGamePiece().getTurntable() ;

                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_final_height_) ;
                    state_ = State::LifterGoToFinalHeight ;                     
                }
                break ;
             
            case State::LifterGoToFinalHeight:
                if(set_lifter_final_height_->isDone()){
                    if (special_hatch_flow_) {
                        auto hatch_holder = getGamePiece().getHatchHolder() ;
                        hatch_holder->setAction(retract_hatch_holder_) ;
                        state_ = State::RetractHatchHolder ;
                    }
                    else {
                        state_ = State::Idle ;                     
                    }
                }            
                break ;

            case State::RetractHatchHolder:
                if (retract_hatch_holder_->isDone()) {
                    state_ = State::Idle ;
                }
                break ;

            case State::Idle:
                break ;
            }
        }

        bool ReadyAction::isDone() {
            return state_ == State::Idle ;
        }

        void ReadyAction::cancel() {
            if (state_ != State::Idle) {
                auto turntable = getGamePiece().getTurntable() ;
                turntable->cancelAction() ;

                auto lifter = getGamePiece().getLifter() ;
                lifter->cancelAction() ;

                state_ = State::Idle ;
            }
        }

        std::string ReadyAction::toString() {
            std::string ret("ReadyAction") ;
            ret += " " + height_ ;
            ret += " " + angle_ ;
            return ret ;
        }
    }
}