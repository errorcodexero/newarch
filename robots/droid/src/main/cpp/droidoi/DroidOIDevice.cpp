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
            // Get button numbers
            //
            size_t coll_v_shoot_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:shoot_collect_mode") ;      //collect or shoot mode
            size_t collect_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:collect_onoff") ;            //collect/no collect
            size_t eject_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:eject");
            size_t automode_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:automode") ;
            
            //
            // Actions
            //
            coll_v_shoot_ = mapButton(coll_v_shoot_b, OIButton::ButtonType::Level) ;                // Toggle switch
            collect_ = mapButton(collect_b, OIButton::ButtonType::Level) ;                          // Push button
            eject_ = mapButton(eject_b, OIButton::ButtonType::Level);
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
            if (gpad->isCancelPressed())
                return true ;

            return false ;
        }

        void DroidOIDevice::generateActions(xero::base::SequenceAction &seq)
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
                    flag_coll_v_shoot_ = CollectShootMode::InvalidMode;
                }
            } else if (getValue(eject_)) {
                flag_eject_ = true;
                seq.pushSubActionPair(intake,  std::make_shared<CollectOffAction>(*intake), false);
                seq.pushSubActionPair(shooter, hood_down_, false);
                seq.pushSubActionPair(turret, turret_goto_zero_, false);
                seq.pushSubActionPair(conveyor, eject_action_, false);
            }

            if (flag_eject_) return;

            if (flag_coll_v_shoot_ == CollectShootMode::InvalidMode || getSwitchMode() != flag_coll_v_shoot_) {
                if(getSwitchMode() == CollectShootMode::CollectMode) {
                    // Setup the game piece manipulator to collect
                    if (game_piece_manipulator->isDone()  ||
                            game_piece_manipulator->getAction() == start_collect_action_ || 
                            game_piece_manipulator->getAction() == stop_collect_action_ ||
                            game_piece_manipulator->getAction() == fire_yes_ ||
                            game_piece_manipulator->getAction() == stop_shoot_action_) {
                        
                        seq.pushSubActionPair(conveyor, queue_prep_collect_, false) ;
                        seq.pushSubActionPair(turret, turret_goto_zero_, false) ;    
                        seq.pushSubActionPair(shooter, hood_down_, false);
                        flag_coll_v_shoot_ = CollectShootMode::CollectMode ;                                            
                        frc::SmartDashboard::PutString("Mode", "Collect") ;
                    }
                }
                else
                {
                    seq.pushSubActionPair(conveyor, queue_prep_shoot_, false) ;
                    seq.pushSubActionPair(turret, turret_follow_, false) ;
                    seq.pushSubActionPair(game_piece_manipulator, fire_yes_, false) ;
                    flag_coll_v_shoot_ = CollectShootMode::ShootMode ;
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
                if (game_piece_manipulator->isDone() || 
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
                            seq.pushSubActionPair(game_piece_manipulator, stop_collect_action_, false) ;
                            flag_collect_ = false ;
                        }
                    }
                }
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
            auto droid_robot_subsystem = droid.getDroidSubsystem() ;

            queue_prep_collect_ = std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor) ;
            queue_prep_shoot_ = std::make_shared<ConveyorPrepareToEmitAction>(*conveyor) ;

            conveyor_receive_ = std::make_shared<ConveyorReceiveAction>(*conveyor) ;
            intake_on_ = std::make_shared<CollectOnAction>(*intake) ;

            turret_follow_ = std::make_shared<FollowTargetAction>(*turret) ;
            fire_yes_ = std::make_shared<FireAction>(*game_piece_manipulator) ;

            start_collect_action_ = std::make_shared<StartCollectAction>(*game_piece_manipulator) ;
            stop_collect_action_ = std::make_shared<StopCollectAction>(*game_piece_manipulator) ;
            stop_shoot_action_ = std::make_shared<StopShootAction>(*game_piece_manipulator) ;

            turret_goto_zero_ = std::make_shared<MotorEncoderGoToAction>(*turret, 0.0) ;

            flag_coll_v_shoot_ = CollectShootMode::InvalidMode;
            flag_eject_ = false;

            hood_down_ = std::make_shared<SetHoodAction>(*shooter, true);
            eject_action_ = std::make_shared<ConveyorEjectAction>(*conveyor);
        }
    }
}
