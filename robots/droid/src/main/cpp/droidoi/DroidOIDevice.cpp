#include "DroidOIDevice.h"
#include "DroidOISubsystem.h"
#include "Droid.h"
#include "droidids.h"
#include "turret/FollowTargetAction.h"
#include "turret/Turret.h"
#include "gamepiecemanipulator/StartCollectAction.h"
#include "gamepiecemanipulator/StopCollectAction.h"
#include "gamepiecemanipulator/FireAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToEmitAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorPrepareToReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorReceiveAction.h"
#include "gamepiecemanipulator/conveyor/ConveyorEmitAction.h"
#include "gamepiecemanipulator/intake/Intake.h"
#include "gamepiecemanipulator/intake/CollectOnAction.h"
#include "gamepiecemanipulator/intake/CollectOffAction.h"
#include "gamepiecemanipulator/GamePieceManipulator.h"
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
            size_t coll_v_shoot_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:collvshoot") ;      //collect or shoot mode
            size_t collect_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:collect") ;            //collect/no collect
            size_t shoot_b = getSubsystem().getRobot().getSettingsParser().getInteger("oi:shoot") ;            //shoot/no-shoot 
            
            //
            // Actions
            //
            coll_v_shoot_ = mapButton(coll_v_shoot_b, OIButton::ButtonType::Level) ;                // Toggle switch
            collect_ = mapButton(collect_b, OIButton::ButtonType::LowToHigh) ;                      // push button
            shoot_ = mapButton(shoot_b, OIButton::ButtonType::LowToHigh) ;                      // push button
        
        }
        
        int DroidOIDevice::getAutoModeSelector() 
        {
            automode_ = 0 ;
            return getValue(automode_) ;    
        }

        void DroidOIDevice::generateActions(xero::base::SequenceAction &seq)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto limelight = droid.getDroidSubsystem()->getLimeLight() ;
            auto target_tracker = droid.getDroidSubsystem()->getTargetTracker() ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;
            auto droid_robot_subsystem = droid.getDroidSubsystem() ;

            //actions and buttons corresponding with the actions
            //actions found on wiki under droidoi (robot)

            /////// Actioning! ///////
            
            if (getValue(coll_v_shoot_) != flag_coll_v_shoot_) {
                if(getValue(coll_v_shoot_)) {
                    // Setup the game piece manipulator to collect
                    if (!game_piece_manipulator->isBusy()) {
                        seq.pushSubActionPair(conveyor, queue_prep_collect_, false) ;
                        flag_coll_v_shoot_ true ;
                    }
                }
                else
                {
                    seq.pushSubActionPair(conveyor, queue_prep_shoot_, false) ;
                    flag_coll_v_shoot_ = false ;
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
                if (game_piece_manipulator->isDone()) {

                    //
                    // Check to see if we are in collect or shoot mode
                    // 
                    if (flag_coll_v_shoot_ == true)
                    {
                        //
                        // We are in collect mode, check the collect button
                        //
                        if (getValue(collect_) && flag_collect_ == false)
                        {
                            //
                            // Button is down, but we are not collecting, assign start collect action
                            //
                            seq.pushSubActionPair(game_piece_manipulator, start_collect_action_, false) ;
                            flag_collect_ = true ;
                        }
                        else if (getValue(collect_) == false && flag_collect_ == true) 
                        {
                            //
                            // Button is up and we are collecting, assign stop collect action
                            //
                            seq.pushSubActionPair(game_piece_manipulator, stop_collect_action_, false) ;
                            flag_collect_ = false ;
                        }
                    }
                    else
                    {
                        //
                        // We are in shoot mode, check the shoot button
                        //
                        if (getValue(shoot_) && flag_shoot_ == false)
                        {
                            seq.pushSubActionPair(game_piece_manipulator, start_shoot_action_, false) ;
                            flag_shoot_ = true ;
                        }
                        else if (getValue(shoot_) == false && flag_shoot_ == true)
                        {
                            seq.pushSubActionPair(game_piece_manipulator, stop_shoot_action_, false) ;
                            flag_shoot_ = false ;
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
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter() ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto limelight = droid.getDroidSubsystem()->getLimeLight() ;
            auto target_tracker = droid.getDroidSubsystem()->getTargetTracker() ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;
            auto droid_robot_subsystem = droid.getDroidSubsystem() ;

            intake_collect_ = std::make_shared<CollectOnAction>(*intake) ;
            intake_retract_ = std::make_shared<CollectOffAction>(*intake) ;          
            queue_collect_ = std::make_shared<ConveyorReceiveAction>(*conveyor) ;
            queue_shoot_ = std::make_shared<ConveyorEmitAction>(*conveyor) ;
            queue_prep_collect_ = std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor) ;
            queue_prep_shoot_ = std::make_shared<ConveyorPrepareToEmitAction>(*conveyor) ;
            fire_yes_ = std::make_shared<FireAction>(*game_piece_manipulator) ;
            turret_follow_ = std::make_shared<FollowTargetAction>(*turret) ;
            start_collect_action_ = std::make_shared<StartCollectAction>(*game_piece_manipulator) ;
            stop_collect_action_ = std::make_shared<StopCollectAction>(*game_piece_manipulator) ;
            start_shoot_action_ = std::make_shared<StartCollectAction>(*game_piece_manipulator) ;
            stop_shoot_action_ = std::make_shared<StopCollectAction>(*game_piece_manipulator) ;

            sequence_ = std::make_shared<SequenceAction>(log) ;
        }

    }
}
