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
#include "limelight/DroidLimeLight.h"
#include "cameratracker/LimeLight.h"
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

            shoot_collect_state_ = CollectShootState::InvalidMode ;
            climber_deployed_ = false ;
            started_deploy_ = false;

            rumbled_ = false ;
            spinner_state_ = SpinnerState::Start ;
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
            size_t manual_mode_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:manual_shoot_mode") ;
            size_t manual_fire_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:manual_shoot_fire") ;            

            climb_lock_ = mapButton(climb_lock_b, OIButton::ButtonType::Level) ;
            climb_deploy_ = mapButton(climb_deploy_b, OIButton::ButtonType::LowToHigh) ;
            climb_secure_ = mapButton(climb_secure_b, OIButton::ButtonType::LowToHigh) ;
            climb_up_ = mapButton(climb_up_b, OIButton::ButtonType::Level) ;
            climb_down_ = mapButton(climb_down_b, OIButton::ButtonType::Level) ;
            climb_left_ = mapButton(climb_left_b, OIButton::ButtonType::Level) ;
            climb_right_ = mapButton(climb_right_b, OIButton::ButtonType::Level) ;  

            manual_shoot_ = mapButton(manual_fire_b, OIButton::ButtonType::LowToHigh) ;
            manual_mode_ = mapButton(manual_mode_b, OIButton::ButtonType::Level) ;

#ifdef NOTYET
            //
            // Spinning things
            //
            size_t spin_deploy_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:spin_deploy") ;
            spin_deploy_ = mapButton(spin_deploy_b, OIButton::ButtonType::Level) ;
#endif
        }
        
        int DroidOIDevice::getAutoModeSelector() 
        {
            automode_ = 0 ;
            return getValue(automode_) ;    
        }

        bool DroidOIDevice::isCollectButtonPressed()
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            // Don't collect if the conveyor is full
            if (conveyor->isFull()) 
                return false;

            if (getValue(collect_))
                return true ;

            OISubsystem &sub = getSubsystem() ;
            auto gpad = sub.getDriverGamepad() ;
            if (gpad->isRTriggerPressed())
                return true ;

            return false ;
        }

        void DroidOIDevice::startEject(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;  
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;            
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;

            shoot_collect_state_ = CollectShootState::Ejecting ;            
            seq.pushSubActionPair(intake,  intake_off_, false);
            seq.pushSubActionPair(turret, turret_goto_zero_, false);
            seq.pushSubActionPair(conveyor, eject_action_, false);
            seq.pushSubActionPair(intake, intake_off_, false);
            seq.pushSubActionPair(shooter, shooter_eject_action_, false);
        }

        void DroidOIDevice::stopEject(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;  
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();                      
            shoot_collect_state_ = CollectShootState::InvalidMode ;
            seq.pushSubActionPair(shooter, shooter_stop_, false) ;
        }

        void DroidOIDevice::collectMode(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;  
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;            
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;

            shoot_collect_state_ = CollectShootState::PreparingForCollect ;
            seq.pushSubActionPair(conveyor, queue_prep_collect_, false) ;
            seq.pushSubActionPair(turret, turret_goto_zero_, false) ;    
            seq.pushSubActionPair(shooter, hood_down_, false);
            frc::SmartDashboard::PutString("Mode", "Collect") ;
        }

        void DroidOIDevice::shootMode(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;  
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;            
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;

            shoot_collect_state_ = CollectShootState::PreparingForShoot ;

            // Queue the conveyor to prepare to fire.
            if (getValue(manual_mode_))
            {
                //
                // In manual shoot mode, turn turret to zero degrees
                //
                seq.pushSubActionPair(turret, turret_goto_zero_, false) ;      
                seq.pushSubActionPair(shooter, std::make_shared<ShooterVelocityAction>(*shooter, 4886.0, Shooter::HoodPosition::Down), false) ;                                  
            }
            else
            {
                //
                // In auto shoot mode, have turret follow limelight
                //
                seq.pushSubActionPair(turret, turret_follow_, false) ;
            }

            seq.pushSubActionPair(conveyor, queue_prep_shoot_, false);
            seq.pushSubActionPair(shooter, shooter_spinup_, false) ;
            seq.pushSubActionPair(intake,  intake_off_, false);
            frc::SmartDashboard::PutString("Mode", "Shoot") ;               
        }

        void DroidOIDevice::processPreparingForCollect(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            if (conveyor->isStagedForCollect())
                shoot_collect_state_ = CollectShootState::CollectReady ;
            else
            {
                assert(conveyor->getAction() == queue_prep_collect_) ;
                assert(queue_prep_collect_->isDone() == false) ;
            }
        }

        void DroidOIDevice::processFinishingCollect(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto manip = droid.getDroidSubsystem()->getGamePieceManipulator() ;

            if (!conveyor->isCollecting())
            {
                seq.pushSubActionPair(droid.getDroidSubsystem()->getGamePieceManipulator(), stop_collect_action_, false) ;
                shoot_collect_state_ = CollectShootState::WaitForIntake ;
            }
        }

        void DroidOIDevice::processWaitingForIntake(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto manip = droid.getDroidSubsystem()->getGamePieceManipulator() ;

            if (!manip->isBusy())
                shoot_collect_state_ = CollectShootState::CollectReady ;
        }

        void DroidOIDevice::processCollectReady(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;

            if (getValue(coll_v_shoot_))
            {
                //
                // The toggle switch has been switched to shoot mode, we switch to shoot mode
                //
                shootMode(seq) ;
            }
            else if (isCollectButtonPressed())
            {
                seq.pushSubActionPair(game_piece_manipulator, start_collect_action_, false) ;
                shoot_collect_state_ = CollectShootState::Collecting ;
            }
        }

        void DroidOIDevice::processCollecting(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            if (!isCollectButtonPressed())
            {
                //
                // The user let off the collect button
                //
                if (conveyor->isCollecting())
                {
                    //
                    // The conveyor is still in the process of collecting, let it finish before we
                    // stop collecting
                    //
                    shoot_collect_state_ = CollectShootState::FinishingCollect ;
                }
                else
                {
                    //
                    // The collect operation is done, we probably collected five balls
                    //
                    shoot_collect_state_ = CollectShootState::CollectReady ;
                    seq.pushSubActionPair(droid.getDroidSubsystem()->getGamePieceManipulator(), stop_collect_action_) ;
                }
            }
        }

        void DroidOIDevice::processPrepareForShoot(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;

            if (conveyor->isStagedForFire())
            {
                shoot_collect_state_ = CollectShootState::ShootReady ;
                if (!getValue(manual_mode_))
                {
                    //
                    // In auto mode, assign the fire action so we shoot as soon as
                    // conditions allow
                    //
                    seq.pushSubActionPair(game_piece_manipulator, fire_yes_, false);                    
                }
            }
        }

        void DroidOIDevice::processShootReady(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            if (!getValue(coll_v_shoot_))
            {
                //
                // This is a question,  once we start firing, how do we stop it if the switch 
                // gets flipped to collect
                //
                collectMode(seq) ;
            }
            else if (getValue(manual_mode_) && getValue(manual_shoot_))
            {
                seq.pushSubActionPair(conveyor, std::make_shared<ConveyorEmitAction>(*conveyor), false) ;
            }
        }

        void DroidOIDevice::processEjecting(SequenceAction &seq)
        {
            //
            // The eject buttons has been released
            //
            if (!getValue(eject_))
                stopEject(seq) ;            
        }

        void DroidOIDevice::processInvalidMode(SequenceAction &seq)
        {
            if (getValue(coll_v_shoot_))
                shootMode(seq) ;
            else
                collectMode(seq) ;
        }

        std::string DroidOIDevice::toString(DroidOIDevice::CollectShootState st)
        {
            std::string ret = "???" ;

            switch(st)
            {
                case CollectShootState::PreparingForCollect:
                    ret = "PreparingForCollect" ;
                    break ;
                case CollectShootState::FinishingCollect:
                    ret = "FinishingCollect" ;                
                    break ;
                case CollectShootState::WaitForIntake:
                    ret = "WaitForIntake" ;
                    break ;
                case CollectShootState::CollectReady:
                    ret = "CollectReady" ;                
                    break ;
                case CollectShootState::Collecting:
                    ret = "Collecting" ;                
                    break ;
                case CollectShootState::PreparingForShoot:
                    ret = "PreparingForShoot" ;                
                    break ;
                case CollectShootState::ShootReady:
                    ret = "ShootReady" ;                
                    break ;
                case CollectShootState::Ejecting:
                    ret = "Ejecting" ;                
                    break ;
                case CollectShootState::InvalidMode:
                    ret = "InvalidMode" ;                
                    break ;
            }

            return ret ;
        }

        void DroidOIDevice::generateCollectShootActions(SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto target_tracker = droid.getDroidSubsystem()->getTargetTracker() ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;
            auto droid_robot_subsystem = droid.getDroidSubsystem() ;
            auto ll = droid.getDroidSubsystem()->getLimeLight() ;

            if (conveyor->getBallCount() == Conveyor::MAX_BALLS && rumbled_ == false)
            {
                getSubsystem().getDriverGamepad()->rumble(true, 1.0, 1.0) ;
                rumbled_ = true ;
            }
            else if (conveyor->getBallCount() < Conveyor::MAX_BALLS && rumbled_ == true)
            {
                rumbled_ = false ;
            }

            CollectShootState prev = shoot_collect_state_ ;            

            if (getValue(eject_))
            {
                //
                // Eject is valid in all modes and takes precedence over anything else.  In addition
                // to ejecting balls, it reset the conveyor in case the state is off.
                //
                startEject(seq) ;
            }
            else
            {

                switch(shoot_collect_state_)
                {
                    case CollectShootState::PreparingForCollect:
                        processPreparingForCollect(seq) ;
                        break ;                
                    case CollectShootState::FinishingCollect:
                        processFinishingCollect(seq) ;
                        break ;
                    case CollectShootState::WaitForIntake:                    
                        processWaitingForIntake(seq) ;
                        break ;
                    case CollectShootState::CollectReady:
                        processCollectReady(seq) ;
                        break ;                
                    case CollectShootState::Collecting:
                        processCollecting(seq) ;
                        break ;                
                    case CollectShootState::PreparingForShoot:
                        processPrepareForShoot(seq) ;
                        break ;                
                    case CollectShootState::ShootReady:
                        processShootReady(seq) ;
                        break ;                
                    case CollectShootState::Ejecting:  
                        processEjecting(seq) ;
                        break ;
                    case CollectShootState::InvalidMode:
                        processInvalidMode(seq) ;
                        break ;
                }
            }

            if (prev != shoot_collect_state_)
            {
                auto &logger = getSubsystem().getRobot().getMessageLogger() ;
                logger.startMessage(MessageLogger::MessageType::debug, MSG_GROUP_DROID_OI) ;
                logger << "OI shoot/collect: " << toString(prev) << " --> " << toString(shoot_collect_state_) ;
                logger.endMessage() ;
            }
        }

        void DroidOIDevice::generateClimbActions(xero::base::SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto climber = droid.getDroidSubsystem()->getClimber() ;

            if (getValue(climb_lock_))
                return ;

            if (!climber_deployed_)
            {
                if (!climber->isInFieldMode() && !started_deploy_) {
                    climber_deployed_ = true;
                    started_deploy_ = true;
                } else {
                    //
                    // The climber is not deployed, the only thing we can do is
                    // deploy the climber
                    //
                    if (getValue(climb_deploy_) && !climber->isBusy() && !climber->getLifter()->isBusy()) {
                        climber->setDefaultAction(nullptr) ;
                        climber->setAction(nullptr) ;
                        seq.pushSubActionPair(climber->getLifter(), deploy_climber_, false) ;
                        started_deploy_ = true;
                    } else if (started_deploy_ && !climber->isBusy() && !climber->getLifter()->isBusy()) {
                        climber_deployed_ = true;
                    }
                }
            }
            else
            {
                if (getValue(climb_deploy_) && !climber->isBusy() && !climber->getLifter()->isBusy())
                {
                    //
                    // If something did not go right, they may want to try and 
                    // deploy again.
                    //
                    seq.pushSubActionPair(climber->getLifter(), deploy_climber_, false) ;
                    started_deploy_ = true;
                    climber_deployed_ = false;
                }
                else
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
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto spinner = droid.getDroidSubsystem()->getControlPanelRotator() ;

            switch(spinner_state_)
            {
            case SpinnerState::Start:
                if (getValue(spin_deploy_))
                {
                    spinner_state_ = SpinnerState::WaitingForIntakeDown;
                    seq.pushSubActionPair(intake, std::make_shared<CollectOnAction>(*intake, false), false) ;
                }
                break ;

            case SpinnerState::WaitingForIntakeDown:
                if (!intake->isBusy())
                {
                    spinner_state_ = SpinnerState::RaisingArm ;
                    start_time_ = getSubsystem().getRobot().getTime() ;
                    seq.pushSubActionPair(spinner, std::make_shared<ControlPanelArmAction>(*spinner, true), false) ;
                }
                break ;

            case SpinnerState::RaisingArm:
                if (getSubsystem().getRobot().getTime() - start_time_ > 2.0)
                {
                    spinner_state_ = SpinnerState::Raised ;
                }
                break ;

            case SpinnerState::Raised:
                if (getValue(spin_deploy_) == false)
                {
                    spinner_state_ = SpinnerState::LoweringArm ;
                    start_time_ = getSubsystem().getRobot().getTime() ;
                    seq.pushSubActionPair(spinner, std::make_shared<ControlPanelArmAction>(*spinner, false), false) ;
                }
                break ;

            case SpinnerState::LoweringArm:
                if (getSubsystem().getRobot().getTime() - start_time_ > 2.0)
                {
                    spinner_state_ = SpinnerState::WaitingForIntakeUp;
                    seq.pushSubActionPair(intake, std::make_shared<CollectOffAction>(*intake), false) ;
                }
                break ;

            case SpinnerState::WaitingForIntakeUp:
                if (!intake->isBusy())
                {
                    spinner_state_ = SpinnerState::Start ;
                }
                break ;
            }
        }

        void DroidOIDevice::generateActions(xero::base::SequenceAction &seq)
        {
            generateCollectShootActions(seq) ;
            generateClimbActions(seq) ;
            // generatePanelSpinnerActions(seq) ;

            if (getValue(manual_shoot_) && getValue(manual_mode_))
            {
                auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;                
                auto turret = droid.getDroidSubsystem()->getTurret() ;
                auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();
                auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;



            }
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
            shooter_spinup_ = std::make_shared<ShooterVelocityAction>(*shooter, 4500, Shooter::HoodPosition::Down) ;
            fire_yes_ = std::make_shared<FireAction>(*game_piece_manipulator);

            conveyor_receive_ = std::make_shared<ConveyorReceiveAction>(*conveyor) ;
            intake_on_ = std::make_shared<CollectOnAction>(*intake) ;
            intake_off_ = std::make_shared<CollectOffAction>(*intake);

            turret_follow_ = std::make_shared<FollowTargetAction>(*turret) ;

            start_collect_action_ = std::make_shared<StartCollectAction>(*game_piece_manipulator) ;
            stop_collect_action_ = std::make_shared<StopCollectAction>(*game_piece_manipulator) ;
            stop_shoot_action_ = std::make_shared<StopShootAction>(*game_piece_manipulator) ;

            turret_goto_zero_ = std::make_shared<MotorEncoderGoToAction>(*turret, 0.0) ;

            hood_down_ = std::make_shared<SetHoodAction>(*shooter, Shooter::HoodPosition::Down);
            eject_action_ = std::make_shared<ConveyorEjectAction>(*conveyor);
            shooter_eject_action_ = std::make_shared<ShooterVelocityAction>(*shooter, -3000, Shooter::HoodPosition::Down);
            shooter_stop_ = std::make_shared<ShooterVelocityAction>(*shooter, 0, Shooter::HoodPosition::Down);

            deploy_climber_ = std::make_shared<MotorEncoderGoToAction>(*climber->getLifter(), "climber:climb_height") ;
            stop_ = std::make_shared<ClimberUpDownAction>(*climber, 0.0, 0.0) ;
        }
    }
}
