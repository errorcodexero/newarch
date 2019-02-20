#include "FloorCollectCargoAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include <lifter/LifterHoldHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "turntable/TurntableHoldAngleAction.h"
#include "cargointake/CargoIntakeAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        FloorCollectCargoAction::FloorCollectCargoAction(GamePieceManipulator &subsystem):GamePieceAction(subsystem)
        {
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto cargo_intake = getGamePiece().getCargoIntake();
            auto cargo_holder = getGamePiece().getCargoHolder();

            //
            // Pre-create all of the actions needed so that we are not constantly creating and
            // destroying this during the robot loop
            //
            set_lifter_safe_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:safe_turn") ;
            hold_lifter_safe_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:safe_turn") ;            
            set_turntable_cargo_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "turntable:angle:cargo:floor_collect") ;
            hold_turntable_cargo_angle_ = std::make_shared<TurntableHoldAngleAction>(*turntable, "turntable:angle:cargo:floor_collect") ;            
            set_lifter_cargo_intake_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:cargo:floor_collect") ;            
            hold_lifter_cargo_intake_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:cargo:floor_collect") ;
            deploy_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, true) ;
            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, false) ;
            set_lifter_cargo_collected_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "lifter:height:cargo:collected") ;            
            hold_lifter_cargo_collected_height_ = std::make_shared<LifterHoldHeightAction>(*lifter, "lifter:height:cargo:collected") ;

            
            set_cargo_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_intake, "cargointake:power") ;
            stop_cargo_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_intake, 0.0) ;
            set_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, "cargoholder:power") ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
        }

        FloorCollectCargoAction::~FloorCollectCargoAction() {
        }

        void FloorCollectCargoAction::start() {
            //
            // 1. Start this action by lifting the lifter to a safe height to rotate. Setting
            //    our internal state (state_) to LifterGoToSafeHeight means we are in the process
            //    of moving the lifter to this safe height
            //
            auto lifter = getGamePiece().getLifter() ;
            lifter->setAction(set_lifter_safe_height_) ;
            state_ = State::LifterGoToSafeHeight ;
        }

        void FloorCollectCargoAction::run() {
            auto cargo_holder = getGamePiece().getCargoHolder() ;
            auto cargo_intake = getGamePiece().getCargoIntake() ;   

            switch(state_) {
            case State::LifterGoToSafeHeight:
                //
                // Check to see if the lifter is at the desired height.  This current state is not
                // complete until the lifter is done with its action.
                //
                if (set_lifter_safe_height_->isDone()) {
                    auto cargo_intake = getGamePiece().getCargoIntake() ;

                    //
                    // 2. The lifter action to move the lifter to a safe height has
                    //    completed.  We perform the next step by rotating the turntable to
                    //    the right angle.  The TurntableGoToCollectAngle state means we are in
                    //    the middle of doing this.
                    //
                    auto turntable = getGamePiece().getTurntable();
                    auto lifter = getGamePiece().getLifter() ;
                    turntable->setAction(set_turntable_cargo_angle_) ;
                    lifter->setAction(hold_lifter_safe_height_) ;
                    cargo_intake->setAction(deploy_cargo_intake_) ;       

                    state_ = State::TurntableGoToCollectAngle ;
                }
                break ;

            case State::TurntableGoToCollectAngle:
                //
                // Check to see if the turntable is at the desired angle.  The current state is not
                // complete until the turntable is done with its action
                //
                if (set_turntable_cargo_angle_->isDone() && deploy_cargo_intake_->isDone()) {
                    //
                    // 3. The turntable action to move into the position to collect cargo from the
                    //    floor is complete.  Move the lifter to the right height to capture cargo
                    //    from the floor while simultaneously deploying the cargo intake. 
                    //
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_cargo_intake_height_) ;

                    auto turntable = getGamePiece().getTurntable() ;
                    turntable->setAction(hold_turntable_cargo_angle_) ;

                    state_ = State::LifterGoToCollectHeightDeployIntake ;                     
                }
                break ;

            case State::LifterGoToCollectHeightDeployIntake:
                //
                // Check to see if the lifeter is at the collect height and the cargo intake has been
                // deployed.  The current state is not done until both of these things are true
                //
                if (set_lifter_cargo_intake_height_->isDone() && deploy_cargo_intake_->isDone()) {
                    //
                    // 4. The intake is deployed and the lifter is at the righ theight.  Turn on both
                    //    the intake and the holder motors.  The WaitForCargo state means we are waiting
                    //    for cargo to arrive in the Cargo Holder.
                    //
                    auto cargo_intake = getGamePiece().getCargoIntake() ;
                    auto lifter = getGamePiece().getLifter() ;
                    cargo_intake->setAction(set_cargo_intake_motor_) ;
                    cargo_holder->setAction(set_cargo_holder_motor_) ;
                    lifter->setAction(hold_lifter_cargo_intake_height_) ;

                    state_ = State::WaitForCargo ;
                }
                break ;

            case State::WaitForCargo:
                //
                // Check the cargo holder subsystem is see if it has cargo.  If not, we keep
                // waiting.  When the limit switch does have cargo, we continue
                if (cargo_holder->hasCargo()) {
                    //
                    // 5. The cargo holder has detected cargo.  Stop the motors on the intake
                    //    and the holder.
                    //
                    cargo_intake->setAction(stop_cargo_intake_motor_) ;
                    cargo_holder->setAction(stop_cargo_holder_motor_) ;

                    state_ = State::StopAllMotors ;
                }
                else if (cargo_intake->hasCargo()) {
                    cargo_intake->setAction(retract_cargo_intake_) ;
                }
                break ;

            case State::StopAllMotors:
                //
                // Ensure that the motors have been stopped.  Setting motor power will happen in
                // the start() function for the action so this is guaranteed.
                //
                if (stop_cargo_intake_motor_->isDone() && stop_cargo_holder_motor_->isDone()) {
                    //
                    // 6. The motors are stopped, retract the intake back into the robot.  The state
                    //    RetrackIntake means we are waiting on the intake to retract.
                    //
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(set_lifter_cargo_collected_height_) ;
                    state_ = State::RaiseLifter ;
                }
                break ;

            case State::RaiseLifter:
                if (set_lifter_cargo_collected_height_->isDone()) {
                    auto cargo_intake = getGamePiece().getCargoIntake() ;                    
                    cargo_intake->setAction(retract_cargo_intake_) ;
                    auto lifter = getGamePiece().getLifter() ;
                    lifter->setAction(hold_lifter_cargo_collected_height_) ;                    
                    state_ = State::RetractIntake ;
                }
                break ;

            case State::RetractIntake:
                //
                // Check that the intake has been retracted
                //
                if (retract_cargo_intake_->isDone()) {
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
        
        bool FloorCollectCargoAction::isDone() {
            return state_ == State::Idle ;
        }

        void FloorCollectCargoAction::cancel() {
            auto cargo_intake = getGamePiece().getCargoIntake() ;
            auto cargo_holder = getGamePiece().getCargoHolder() ;            

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
                cargo_intake->setAction(stop_cargo_intake_motor_) ;
                cargo_holder->setAction(stop_cargo_holder_motor_) ;                
                state_ = State::StopAllMotors ;
                break ;

            case State::WaitForCargo:
                // We are deploying the intake, must get it back into the robot before
                // the cancel operation is done
                cargo_intake->setAction(stop_cargo_intake_motor_) ;
                cargo_holder->setAction(stop_cargo_holder_motor_) ;                
                state_ = State::StopAllMotors ;            
                break ;

            case State::StopAllMotors:
                // We are already putting things away, just let it finish
                break ;

            case State::RetractIntake:
                // We are already putting things away, just let it finish            
                break ;

            case State::RaiseLifter:
                break ;

            case State::Idle:
                // We are already putting things away, just let it finish            
                break ;
            }
        }

        std::string FloorCollectCargoAction::toString() {
            std::string ret("FloorCollectCargoAction") ;
            return ret ;
        }
    }
}