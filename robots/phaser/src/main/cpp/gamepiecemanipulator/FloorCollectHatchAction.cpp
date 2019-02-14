#include "FloorCollectHatchAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "hatchintake/HatchIntakeAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        FloorCollectHatchAction::FloorCollectHatchAction(GamePieceManipulator &subsystem):GamePieceAction(subsystem)
        {
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto hatch_holder = getGamePiece().getHatchHolder() ;
            auto hatch_intake = getGamePiece().getHatchIntake();

            //
            // Pre-create all of the actions needed so that we are not constantly creating and
            // destroying this during the robot loop
            //

            set_lifter_safe_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "set_lifter_safe_height_") ;
            set_turntable_hatch_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "set_turntable_hatch_angle_") ;
            set_lifter_hatch_intake_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "set_lifter_hatch_intake_height_") ;            
            set_lifter_undock_hatch_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "set_lifter_undock_hatch_height_") ;

            set_retract_arm_ = std::make_shared<HatchHolderAction>(*hatch_holder, "set_retract_arm_") ;
            set_retract_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, "set_retract_hatch_finger_") ;
            set_deploy_hatch_finger_ = std::make_shared<HatchHolderAction>(*hatch_holder, "set_deploy_hatch_finger_") ;

            deploy_hatch_intake_ = std::make_shared<HatchIntakeAction>(*hatch_intake, "deploy_hatch_intake_") ;
            retract_hatch_intake_ = std::make_shared<HatchIntakeAction>(*hatch_intake, "retract_hatch_intake_") ;
    
            set_hatch_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*hatch_intake, "set_hatch_intake_motor_") ;
            stop_hatch_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*hatch_intake, 0.0) ;
        }

        FloorCollectHatchAction::~FloorCollectHatchAction() {
        }

        void FloorCollectHatchAction::start() {

            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(set_lifter_safe_height_) ;
   
            auto arm = getGamePiece().getHatchHolder() ;
            // i need to set the arm and the finger to retract, but i don't know how, and i'm tired, and i'm going home

            state_ = State::LifterGoToSafeHeight ;


        }

        void FloorCollectHatchAction::run() {
            auto hatch_intake = getGamePiece().getHatchIntake() ;

            switch(state_) {
            case State::LifterGoToSafeHeight:

                if (set_lifter_safe_height_->isDone()) {

                    auto turntable = getGamePiece().getTurntable();
                    turntable->setAction(set_turntable_hatch_angle_) ;
                    state_ = State::TurntableGoToCollectAngle ;
                }
                break ;

            case State::TurntableGoToCollectAngle:

                if (set_turntable_hatch_angle_->isDone()) {

                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_hatch_intake_height_) ;

                    auto hatch_intake = getGamePiece().getHatchIntake() ;
                    hatch_intake->setAction(deploy_hatch_intake_) ;                   

                    state_ = State::LifterGoToCollectHeightDeployIntake ;                     
                }
                break ;

            case State::LifterGoToCollectHeightDeployIntake:
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
                    hatch_holder->setAction(set_hatch_holder_motor_) ;

                    state_ = State::WaitForHatch ;
                }
                break ;

            case State::WaitForHatch:
                //
                // Check the hatch holder subsystem is see if it has hatch.  If not, we keep
                // waiting.  When the limit switch does have hatch, we continue
                if (hatch_holder->hasHatch()) {
                    //
                    // 5. The hatch holder has detected hatch.  Stop the motors on the intake
                    //    and the holder.
                    //
                    auto hatch_intake = getGamePiece().getHatchIntake() ;
                    hatch_intake->setAction(stop_hatch_intake_motor_) ;
                    hatch_holder->setAction(stop_hatch_holder_motor_) ;

                    state_ = State::StopAllMotors ;
                }
                break ;

            case State::StopAllMotors:
                //
                // Ensure that the motors have been stopped.  Setting motor power will happen in
                // the start() function for the action so this is guaranteed.
                //
                if (stop_hatch_intake_motor_->isDone() && stop_hatch_holder_motor_->isDone()) {
                    //
                    // 6. The motors are stopped, retract the intake back into the robot.  The state
                    //    RetrackIntake means we are waiting on the intake to retract.
                    //
                    auto hatch_intake = getGamePiece().getHatchIntake() ;                    
                    hatch_intake->setAction(retract_hatch_intake_) ;
                    state_ = State::RetractIntake ;
                }
                break ;

            case State::RetractIntake:
                //
                // Check that the intake has been retracted
                //
                if (retract_hatch_intake_->isDone()) {
                    //
                    // 7. We are done, move to the idle state
                    //
                    state_ = State::Idle ;
                }
                break ;

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

            case State::LifterGoToCollectHeightDeployIntake:
                // We are deploying the intake, must get it back into the robot before
                // the cancel operation is done
                hatch_intake->setAction(stop_hatch_intake_motor_) ;
                hatch_holder->setAction(stop_hatch_holder_motor_) ;                
                state_ = State::StopAllMotors ;
                break ;

            case State::WaitForHatch:
                // We are deploying the intake, must get it back into the robot before
                // the cancel operation is done
                hatch_intake->setAction(stop_hatch_intake_motor_) ;
                hatch_holder->setAction(stop_hatch_holder_motor_) ;                
                state_ = State::StopAllMotors ;            
                break ;

            case State::StopAllMotors:
                // We are already putting things away, just let it finish
                break ;

            case State::RetractIntake:
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