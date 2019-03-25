#include "ScoreHatch.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "hatchholder/HatchHolderAction.h"
#include "oi/DriverGamepad.h"
#include <oi/DriverGamepadRumbleAction.h>

using namespace xero::base ;

namespace xero {
    namespace phaser {
        ScoreHatch::ScoreHatch(GamePieceManipulator &subsystem, std::shared_ptr<xero::base::LightSensorSubsystem> lines):GamePieceAction(subsystem) {
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto lifter = getGamePiece().getLifter() ;
            auto oi = getGamePiece().getRobot().getOI() ;
            lines_ = lines ;
            
            set_lifter_shift_down_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:score:hatch:shift_down", true) ;

            set_extend_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_ARM, "hatchholder:place:delay", "hatchholder:place:totaldelay") ;
            set_retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM, "hatchholder:default:delay") ;
            set_retract_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER, "hatchholder:default:delay") ;
            set_deploy_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_FINGER, "hatchholder:default:delay") ;
            rumble_ = std::make_shared<DriverGamepadRumbleAction>(*oi, true, 1, 1.0, 1.0) ;            
        }

        ScoreHatch::~ScoreHatch(){
        }

        void ScoreHatch::start() {
            auto turntable = getGamePiece().getTurntable() ;

            static double angle[8] = { 
                0.0,
                2.0,
                0.0,
                1.0,
                -2.0,
                0.0,
                -1.0,
                0.0
            } ;
            if (lines_ != nullptr) {
                uint32_t off = lines_->getSensorsState() ;
                rotate_ = std::make_shared<TurntableGoToAngleAction>(*turntable, turntable->getAngleValue() + angle[off]) ;
                turntable->setAction(rotate_) ;
                state_ = State::Rotate ;
                std::cout << "Adjusting angle" ;
                std::cout << ": off " << off ;
                std::cout << " angle " << angle[off] << std::endl ;
            }
            else {
                auto hatch_holder = getGamePiece().getHatchHolder() ;
                hatch_holder->setAction(set_extend_arm_) ;
                state_ = State::ExtendArm ;
            }
        }

        void ScoreHatch::run(){
            auto lifter = getGamePiece().getLifter() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;

            switch(state_) {
            case State::Rotate:
                if (rotate_->isDone()) {
                    auto hatch_holder = getGamePiece().getHatchHolder() ;
                    hatch_holder->setAction(set_extend_arm_) ;
                    state_ = State::ExtendArm ;                    
                }
                break ;
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
                    auto oi = getGamePiece().getRobot().getOI() ;
                    oi->setAction(rumble_) ;                    
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