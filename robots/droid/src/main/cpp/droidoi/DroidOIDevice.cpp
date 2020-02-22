#include "DroidOIDevice.h"
#include "DroidOISubsystem.h"
#include "Droid.h"
#include "droidids.h"
#include "turret/FollowTargetAction.h"
#include "turret/Turret.h"
#include "gamepiecemanipulator/StartCollectAction.h"
#include "gamepiecemanipulator/StopCollectAction.h"
#include "gamepiecemanipulator/StartShootAction.h"
#include "gamepiecemanipulator/StopShootAction.h"
#include "gamepiecemanipulator/FireAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorEjectAction.h"
#include "gamepiecemanipulator/intake/CollectOnAction.h"
#include "gamepiecemanipulator/intake/Intake.h"
#include "gamepiecemanipulator/intake/CollectOffAction.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
#include "gamepiecemanipulator/shooter/SetHoodAction.h"
#include "gamepiecemanipulator/shooter/ShooterVelocityAction.h"
#include "controlpanelrotator/ControlPanelArmAction.h"
#include "controlpanelrotator/ControlPanelColorAction.h"
#include "controlpanelrotator/ControlPanelDistanceAction.h"
#include "climber/ClimberUpDownAction.h"
#include <Subsystem.h>
#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {

        /* From the wiki
         * Intake, Conveyor, Turret, and Shooter
         * One button for collecting
         * - Push and hold to collect
         * - If in shoot queue, flash a light on the drivers station
         * One two position switch for queuing to shoot or queuing to collect
         * One two position switch for shooting mode
         * - Switch up to start scanning, switch down to stop
         * - When limelight finds the target, aims, and the drive base is stopped, automatically start shooting
         */


        DroidOIDevice::DroidOIDevice(DroidOISubsystem &sub, int index) : OIDevice(sub, index) 
        {
            initialize() ;

            flag_coll_v_shoot_ = CollectShootMode::InvalidMode ;
            flag_collect_ = false ;
            climber_deployed_ = false ;
        }

        DroidOIDevice::~DroidOIDevice() 
        {
        }

        void DroidOIDevice::initialize() 
        {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_DROID_OI) ;
            log << "OI: initializing button/axis mapping" ;
            log.endMessage() ;

            //
            // Auto modes
            //
            size_t automode_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:automode") ;
            std::vector<double> mapping = { -0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0 } ;
            automode_ = mapAxisScale(automode_b, mapping);

            // Collect and shoot
            //
            size_t coll_v_shoot_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:shoot_collect_mode") ;      //collect or shoot mode
            size_t collect_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:collect_onoff") ;            //collect/no collect
            size_t eject_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:eject");

            coll_v_shoot_ = mapButton(coll_v_shoot_b, OIButton::ButtonType::Level) ;                    // Toggle switch
            collect_ = mapButton(collect_b, OIButton::ButtonType::Level) ;                              // Push button
            eject_ = mapButton(eject_b, OIButton::ButtonType::Level);

            //
            // Climb
            //
            size_t climb_lock_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:climb_lock") ;
            size_t climb_deploy_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:climb_deploy") ;
            size_t climb_secure_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:climb_secure") ;
            size_t climb_up_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:climb_up") ;
            size_t climb_down_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:climb_down") ;
            size_t climb_left_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:traverse_left") ;
            size_t climb_right_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:traverse_right") ;

            climb_lock_ = mapButton(climb_lock_b, OIButton::ButtonType::Level) ;
            climb_deploy_ = mapButton(climb_deploy_b, OIButton::ButtonType::LowToHigh) ;
            climb_secure_ = mapButton(climb_secure_b, OIButton::ButtonType::LowToHigh) ;
            climb_up_ = mapButton(climb_up_b, OIButton::ButtonType::Level) ;
            climb_down_ = mapButton(climb_down_b, OIButton::ButtonType::Level) ;
            climb_left_ = mapButton(climb_left_b, OIButton::ButtonType::Level) ;
            climb_right_ = mapButton(climb_right_b, OIButton::ButtonType::Level) ;                        
        }
        
        int DroidOIDevice::getAutoModeSelector() 
        {
            automode_ = 0 ;
            return getValue(automode_) ;    
        }

        DroidOIDevice::CollectShootMode DroidOIDevice::getSwitchMode()
        {
            int sw = getValue(coll_v_shoot_) ;
            return sw ? CollectShootMode::ShootMode : CollectShootMode::CollectMode ;
        }

        bool DroidOIDevice::isCollectButtonPressed()
        {
            if (getValue(collect_))
                return true ;

            OISubsystem &sub = getSubsystem() ;
            auto gpad = sub.getDriverGamepad() ;
            if (gpad->isRTriggerPressed())
                return true ;

            return false ;
        }

        void DroidOIDevice::generateCollectShootActions(xero::base::SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto limelight = droid.getDroidSubsystem()->getLimeLight() ;
            auto target_tracker = droid.getDroidSubsystem()->getTargetTracker() ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;
            auto droid_robot_subsystem = droid.getDroidSubsystem() ;

            if (flag_eject_) {
                if (!getValue(eject_)) {
                    flag_eject_ = false;   
                    waitingForConveyorPrepShoot_ = false;
                    flag_coll_v_shoot_ = CollectShootMode::InvalidMode;
                    shooter->setAction(nullptr);
                }
            } else if (getValue(eject_)) {
                flag_eject_ = true;
                seq.pushSubActionPair(intake,  intake_off_, false);
                seq.pushSubActionPair(turret, turret_goto_zero_, false);
                seq.pushSubActionPair(conveyor, eject_action_, false);
                seq.pushSubActionPair(intake, intake_off_, false);
                seq.pushSubActionPair(shooter, shooter_eject_action_, false);
            }

            if (flag_eject_) return;

            if (flag_coll_v_shoot_ == CollectShootMode::InvalidMode || getSwitchMode() != flag_coll_v_shoot_) {
                if(getSwitchMode() == CollectShootMode::CollectMode) {
                    // Setup the game piece manipulator to collect
                    if (game_piece_manipulator->isDone()  || waitingForConveyorPrepShoot_ ||
                            game_piece_manipulator->getAction() == start_collect_action_ || 
                            game_piece_manipulator->getAction() == stop_collect_action_ ||
                            game_piece_manipulator->getAction() == fire_yes_ ||
                            game_piece_manipulator->getAction() == stop_shoot_action_) 
                    {
                        seq.pushSubActionPair(conveyor, queue_prep_collect_, false) ;
                        seq.pushSubActionPair(turret, turret_goto_zero_, false) ;    
                        seq.pushSubActionPair(shooter, hood_down_, false);
                        flag_collect_ = false;
                        flag_coll_v_shoot_ = CollectShootMode::CollectMode ;                                   
                        frc::SmartDashboard::PutString("Mode", "Collect") ;
                    }
                }
                else {
                    // Queue the conveyor to prepare to fire.
                    seq.pushSubActionPair(turret, turret_follow_, false) ;
                    seq.pushSubActionPair(conveyor, queue_prep_shoot_, false);
                    seq.pushSubActionPair(shooter, shooter_spinup_, false) ;
                    seq.pushSubActionPair(intake,  intake_off_, false);
                    flag_coll_v_shoot_ = CollectShootMode::ShootMode ;      
                    waitingForConveyorPrepShoot_ = true;
                    frc::SmartDashboard::PutString("Mode", "Shoot") ;                    
                }
            }
            else
            {
                //
                // If the mode is not setup, but we want a different mode, we should never
                // actually shoot or collect, so this is in an else clause
                //

                //
                // Check to see that the game piece manipulator can actually accept new actions
                //           
                if (game_piece_manipulator->isDone() || waitingForConveyorPrepShoot_ ||
                            game_piece_manipulator->getAction() == start_collect_action_ || 
                            game_piece_manipulator->getAction() == stop_collect_action_ ||
                            game_piece_manipulator->getAction() == fire_yes_ ||
                            game_piece_manipulator->getAction() == stop_shoot_action_)                                                        
                {
                    //
                    // Check to see if we are in collect or shoot mode
                    // 
                    if (flag_coll_v_shoot_ == CollectShootMode::CollectMode)
                    {
                        //
                        // We are in collect mode, check the collect button
                        //
                        if (isCollectButtonPressed() && flag_collect_ == false)
                        {
                            //
                            // Button is down, but we are not collecting, assign start collect action
                            //
                            seq.pushSubActionPair(game_piece_manipulator, start_collect_action_, false) ;
                            flag_collect_ = true ;
                        }
                        else if (!isCollectButtonPressed() && flag_collect_ == true) 
                        {
                            //
                            // Button is up and we are collecting, assign stop collect action
                            //

                            // If the conveyor is still collecting, wait for it to finish (with a timeout)
                            if (waitingForConveyorFinishCollect_) {
                                if (!conveyor->isCollecting() || droid.getTime() > conveyorTimeout_) {
                                    waitingForConveyorFinishCollect_ = false;
                                    flag_collect_ = false ;
                                    seq.pushSubActionPair(game_piece_manipulator, stop_collect_action_, false) ;
                                }
                            } else {
                                if (conveyor->isCollecting()) {
                                    waitingForConveyorFinishCollect_ = true;
                                    conveyorTimeout_ = droid.getTime() + 1;
                                } else {
                                    flag_collect_ = false ;
                                    seq.pushSubActionPair(game_piece_manipulator, stop_collect_action_, false) ;
                                }
                            }
                        }
                    } else if (waitingForConveyorPrepShoot_ 
                               && queue_prep_shoot_->isDone()
                               && intake_off_->isDone()) {
                        seq.pushSubActionPair(game_piece_manipulator, fire_yes_);
                        waitingForConveyorPrepShoot_ = false;
                    }
                }
            }
        }

        void DroidOIDevice::generateClimbActions(xero::base::SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto climber = droid.getDroidSubsystem()->getClimber() ;

            if (getValue(climb_lock_) == false)
                return ;

            if (!climber_deployed_)
            {
                //
                // The climber is not deployed, the only thing we can do is
                // deploy the climber
                //
                if (getValue(climb_deploy_) && !climber->isBusy())
                    seq.pushSubActionPair(climber->getLifter(), deploy_climber_) ;
            }
            else
            {
                if (getValue(climb_deploy_) && !climber->isBusy())
                {
                    //
                    // If something did not go right, they may want to try and 
                    // deploy again.
                    //
                    seq.pushSubActionPair(climber->getLifter(), deploy_climber_) ;
                }
                else if (!getValue(climb_secure_))
                {
                    bool up = getValue(climb_up_);
                    bool down = getValue(climb_down_);
                    bool left = getValue(climb_left_);
                    bool right = getValue(climb_right_);
                    ActionPtr action;
                    if (up) {
                        if (left) 
                            action = std::make_shared<ClimberUpDownAction>(*climber, "climber:power:up", "climber:power:left");
                        else if (right)
                            action = std::make_shared<ClimberUpDownAction>(*climber, "climber:power:up", "climber:power:right");
                        else
                            action = std::make_shared<ClimberUpDownAction>(*climber, "climber:power:up", 0.0);
                    } else if (down) {
                        if (left) 
                            action = std::make_shared<ClimberUpDownAction>(*climber, "climber:power:down", "climber:power:left");
                        else if (right)
                            action = std::make_shared<ClimberUpDownAction>(*climber, "climber:power:down", "climber:power:right");
                        else
                            action = std::make_shared<ClimberUpDownAction>(*climber, "climber:power:down", 0.0);
                    } else if (left) {
                        action = std::make_shared<ClimberUpDownAction>(*climber, 0.0, "climber:power:left");
                    } else if (right) {
                        action = std::make_shared<ClimberUpDownAction>(*climber, 0.0, "climber:power:right");
                    } else {
                        action = stop_;
                    }
                    seq.pushSubActionPair(climber, action, false);
                }
                
            }
            
        }

        void DroidOIDevice::generatePanelSpinnerActions(xero::base::SequenceAction &seq)
        {

        }

        void DroidOIDevice::generateActions(xero::base::SequenceAction &seq)
        {
            generateCollectShootActions(seq) ;
            generateClimbActions(seq) ;
            generatePanelSpinnerActions(seq) ;
        }

        void DroidOIDevice::init() 
        {
            MessageLogger &log = getSubsystem().getRobot().getMessageLogger() ;
            log.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_DROID_OI) ;
            log << "OI: creating static actions" ;
            log.endMessage() ;

            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto target_tracker = droid.getDroidSubsystem()->getTargetTracker() ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;
            auto shooter = game_piece_manipulator->getShooter();
            auto climber = droid.getDroidSubsystem()->getClimber() ;
            auto droid_robot_subsystem = droid.getDroidSubsystem() ;

            queue_prep_collect_ = std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor) ;
            queue_prep_shoot_ = std::make_shared<ConveyorPrepareToEmitAction>(*conveyor) ;
            shooter_spinup_ = std::make_shared<ShooterVelocityAction>(*shooter, 4500, true);
            fire_yes_ = std::make_shared<FireAction>(*game_piece_manipulator);
            waitingForConveyorPrepShoot_ = false;

            conveyor_receive_ = std::make_shared<ConveyorReceiveAction>(*conveyor) ;
            intake_on_ = std::make_shared<CollectOnAction>(*intake) ;
            intake_off_ = std::make_shared<CollectOffAction>(*intake);

            turret_follow_ = std::make_shared<FollowTargetAction>(*turret) ;

            start_collect_action_ = std::make_shared<StartCollectAction>(*game_piece_manipulator) ;
            stop_collect_action_ = std::make_shared<StopCollectAction>(*game_piece_manipulator) ;
            stop_shoot_action_ = std::make_shared<StopShootAction>(*game_piece_manipulator) ;

            turret_goto_zero_ = std::make_shared<MotorEncoderGoToAction>(*turret, 0.0) ;

            flag_coll_v_shoot_ = CollectShootMode::InvalidMode;
            flag_eject_ = false;

            hood_down_ = std::make_shared<SetHoodAction>(*shooter, true);
            eject_action_ = std::make_shared<ConveyorEjectAction>(*conveyor);
            shooter_eject_action_ = std::make_shared<ShooterVelocityAction>(*shooter, -3000, true);

            deploy_climber_ = std::make_shared<MotorEncoderGoToAction>(*climber->getLifter(), "climber:climb_height") ;
            stop_ = std::make_shared<ClimberUpDownAction>(*climber, 0.0, 0.0) ;
        }
    }
}
