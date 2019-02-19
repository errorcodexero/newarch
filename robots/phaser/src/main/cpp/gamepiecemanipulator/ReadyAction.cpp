#include "ReadyAction.h"
#include <lifter/LifterGoToHeightAction.h>
#include <lifter/LifterHoldHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "turntable/TurntableHoldAngleAction.h"

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
            hold_lifter_safe_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:safe_turn") ;
            set_lifter_final_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, height) ;
            hold_lifter_final_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, height) ;
            set_turntable_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, angle) ;
            hold_turntable_angle_ = std::make_shared<TurntableHoldAngleAction>(*turntable, angle) ;            
        }

        ReadyAction::~ReadyAction(){
        }

        void ReadyAction::start() {
         
            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(set_lifter_safe_height_) ; 

            state_ = State::LifterSafeHeight ;
        }

        void ReadyAction::run(){
            switch(state_) {
            case State::LifterSafeHeight:
                if(set_lifter_safe_height_->isDone()){
                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(set_turntable_angle_) ;

                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(hold_lifter_safe_height_) ;
                    state_ = State::TurntableGoToAngle ; 
                }
                break ;

            case State::TurntableGoToAngle:
                if(set_turntable_angle_->isDone()){
                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(hold_turntable_angle_) ;

                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_final_height_) ;
                    state_ = State::LifterGoToFinalHeight ;                     
                }
                break ;
             
            case State::LifterGoToFinalHeight:
                if(set_lifter_final_height_->isDone()){
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(hold_lifter_final_height_) ;
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
            auto turntable = getGamePiece().getTurntable() ;
            turntable->setAction(nullptr) ;

            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(nullptr) ;            
        }

        std::string ReadyAction::toString() {
            std::string ret("ReadyAction") ;
            ret += " " + height_ ;
            ret += " " + angle_ ;
            return ret ;
        }
    }
}