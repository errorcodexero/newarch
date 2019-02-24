#include "FloorCollectHatchAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "hatchintake/HatchIntakeAction.h"
#include "hatchholder/HatchHolderAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        FloorCollectHatchAction::FloorCollectHatchAction(GamePieceManipulator &subsystem):GamePieceAction(subsystem) {

            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto hatch_intake = getGamePiece().getHatchIntake();

            //
            // Pre-create all of the actions needed so that we are not constantly creating and
            // destroying this during the robot loop
            //

            set_lifter_safe_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:safe_turn") ;
            set_turntable_hatch_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "turntable:angle:hatch:floor_collect") ;
            set_lifter_hatch_intake_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:hatch:floor_collect") ;            
            set_lifter_undock_hatch_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:hatch:undock_intake") ;

            set_extend_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_ARM, "hatcholder:default:delay") ;
            set_retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_ARM, "hatcholder:default:delay") ;
            set_retract_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::RETRACT_FINGER, "hatcholder:default:delay") ;
            set_deploy_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, HatchHolderAction::Operation::EXTEND_FINGER, "hatcholder:default:delay") ;

            deploy_hatch_intake_ = std::make_shared<HatchIntakeAction>(*hatch_intake, true) ;
            retract_hatch_intake_ = std::make_shared<HatchIntakeAction>(*hatch_intake, false) ;
    
            set_hatch_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*hatch_intake, "hatchintake:power") ;
            stop_hatch_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*hatch_intake, 0.0) ;
        }

        FloorCollectHatchAction::~FloorCollectHatchAction() {
        }

        void FloorCollectHatchAction::start() {

            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(set_lifter_safe_height_) ;
   
            auto hatchholder = getGamePiece().getHatchHolder() ;
            hatchholder->setAction(set_retract_arm_) ;
            state_ = State::LifterGoToSafeHeight ;
        }

        void FloorCollectHatchAction::run() {
            auto hatch_intake = getGamePiece().getHatchIntake() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto lifter = getGamePiece().getLifter() ;

            switch(state_) {
            case State::LifterGoToSafeHeight:
                if (set_lifter_safe_height_->isDone() && set_retract_arm_->isDone()) {
                    auto turntable = getGamePiece().getTurntable();
                    turntable->setAction(set_turntable_hatch_angle_) ;
                    hatch_holder->setAction(set_retract_hatch_finger_) ;

                    state_ = State::TurntableGoToCollectAngle ;
                }
                break ;

            case State::TurntableGoToCollectAngle:
                if (set_turntable_hatch_angle_->isDone() && set_retract_hatch_finger_->isDone()) {

                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_hatch_intake_height_) ;

                    auto hatch_intake = getGamePiece().getHatchIntake() ;
                    hatch_intake->setAction(deploy_hatch_intake_) ;

                    state_ = State::LifterGoToCollectHeightDeployIntakeRunMotor ;
                }
                break ;

            case State::LifterGoToCollectHeightDeployIntakeRunMotor:
                //
                // Check to see if the lifeter is at the collect height and the hatch intake has been
                // deployed.  The current state is not done until both of these things are true
                //
                if (set_lifter_hatch_intake_height_->isDone() && deploy_hatch_intake_->isDone()) {
                    //
                    // 4. The intake is deployed and the lifter is at the righ theight.  Turn on both
                    //    the intake and the holder motors.  The WaitForHatch state means we are waiting
                    //    for hatch to arrive in the Hatch Holder.
                    //
                    auto hatch_intake = getGamePiece().getHatchIntake() ;
                    hatch_intake->setAction(set_hatch_intake_motor_) ;

                    state_ = State::WaitForHatch ;
                }
                break ;

            case State::WaitForHatch:
                //
                // Check the hatch holder subsystem is see if it has hatch.  If not, we keep
                // waiting.  When the limit switch does have hatch, we continue
                if (hatch_intake->hasHatch()) {
                    //
                    // 5. The hatch holder has detected hatch.  Stop the motors on the intake
                    //    and the holder.
                    //
                    auto hatch_intake = getGamePiece().getHatchIntake() ;
                    hatch_intake->setAction(retract_hatch_intake_) ;

                    state_ = State::FlipIntake ;
                }
                break ;

            case State::FlipIntake:
                //
                // Check that the intake has been retracted
                //
                if (retract_hatch_intake_->isDone()) {
                    //
                    // The hatch intake has been retracted, lift the lifter so that we remove the
                    // hatch from the hatch intake.
                    //
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_undock_hatch_height_) ;

                    state_ = State::RaiseLiftFromIntake ;
                }
                break ;

            case State::RaiseLiftFromIntake:
                if (set_lifter_undock_hatch_height_->isDone()) {
                    auto hatchholder = getGamePiece().getHatchHolder() ;
                    hatchholder->setAction(set_deploy_hatch_finger_) ;

                    state_ = State::DeployFinger ;
                }
                break ;

            case State::DeployFinger:
                if (set_deploy_hatch_finger_->isDone()) {
                    state_ = State::Idle ;
                }

            case State::Idle:
                break ;
            }
        }
        
        bool FloorCollectHatchAction::isDone() {
            return state_ == State::Idle ;
        }

        void FloorCollectHatchAction::cancel() {
            auto hatch_intake = getGamePiece().getHatchIntake() ;
            auto hatch_holder = getGamePiece().getHatchHolder() ;            

            switch(state_) {
            case State::LifterGoToSafeHeight:
                // The lifter is changing heights, but that is ok, let the lift
                // finish its height action
                state_ = State::Idle ;
                break ;

            case State::TurntableGoToCollectAngle:
                // The turntable is changing angles, but that is ok, let the lift
                // finish its height action
                state_ = State::Idle ;            
                break ;

            case State::LifterGoToCollectHeightDeployIntakeRunMotor:
                // We are deploying the intake, must get it back into the robot before
                // the cancel operation is done
                    hatch_intake->setAction(retract_hatch_intake_) ;
                state_ = State::FlipIntake ;
                break ;

            case State::WaitForHatch:
                // We are deploying the intake, must get it back into the robot before
                // the cancel operation is done
                hatch_intake->setAction(retract_hatch_intake_) ;
                state_ = State::FlipIntake ;            
                break ;

            case State::FlipIntake:
                // We are already putting things away, just let it finish            
                break ;

            case State::RaiseLiftFromIntake:                
                // We are already putting things away, just let it finish            
                break ;

            case State::DeployFinger:
                // We are already putting things away, just let it finish            
                break ;            

            case State::Idle:
                // We are already putting things away, just let it finish            
                break ;
            }
        }

        std::string FloorCollectHatchAction::toString() {
            std::string ret("FloorCollectHatchAction") ;
            return ret ;
        }
    }
}