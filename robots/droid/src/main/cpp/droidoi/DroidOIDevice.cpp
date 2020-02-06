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
#include <Robot.h>
#include <TeleopController.h>
#include <SettingsParser.h>
#include <frc/smartdashboard/SmartDashboard.h>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace droid {

/*
Intake, Conveyor, Turret, and Shooter
One button for collecting
 - Push and hold to collect
 - If in shoot queue, flash a light on the drivers station
One two position switch for queuing to shoot or queuing to collect
One two position switch for shooting mode
 - Switch up to start scanning, switch down to stop
 - When limelight finds the target, aims, and the drive base is stopped, automatically start shooting
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
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter() ;
            auto turret = droid.getDroidSubsystem()->getTurret() ;
            auto limelight = droid.getDroidSubsystem()->getLimeLight() ;
            auto target_tracker = droid.getDroidSubsystem()->getTargetTracker() ;
            auto game_piece_manipulator = droid.getDroidSubsystem()->getGamePieceManipulator() ;
            auto droid_robot_subsystem = droid.getDroidSubsystem() ;

            //actions and buttons corresponding with the actions
            //actions found on wiki under droidoi (robot)

        /////// Actioning! ///////
            
            

            if (getValue(coll_v_shoot_)) {
                if (flag_coll_v_shoot_ == false || (flag_collect_ == true && getValue(collect_) == false)) {
                    clearQueue() ;
                    conveyorActionQueue.push(queue_prep_collect_) ;
                }
                else if (flag_coll_v_shoot_ == true && (flag_collect_ == false && getValue(collect_) == true)) {
                    conveyorActionQueue.push(queue_collect_) ;
                    conveyorActionQueue.push(intake_collect_) ;
                }
            }
            else if (getValue(coll_v_shoot_) == false) {
                if (flag_coll_v_shoot_ == true || (flag_shoot_ == true && getValue(shoot_) == false)) {
                    clearQueue() ;
                    conveyorActionQueue.push(queue_prep_shoot_) ;  
                    if (flag_coll_v_shoot_ == true)
                        conveyorActionQueue.push(intake_retract_) ;
                }
                else if (flag_coll_v_shoot_ == false && (flag_shoot_ == false && getValue(shoot_) == true)) {
                    conveyorActionQueue.push(queue_shoot_) ;
                    conveyorActionQueue.push(fire_yes_) ;
                }
            }
            
            if (!conveyorActionQueue.empty()) {
                seq.pushSubActionPair(game_piece_manipulator, conveyorActionQueue.front(), false) ;
                conveyorActionQueue.pop() ;
            }

            flag_coll_v_shoot_ = getValue(coll_v_shoot_) ; //true = collect, false = shoot
            flag_collect_ = getValue(collect_) ;           //true = collect, false = no collect
            flag_shoot_ = getValue(shoot_) ;               //true = shoot, false = no shoot

         //       seq.pushSubActionPair(game_piece_manipulator, fire_yes_, false) ;

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

            //TBD list : 
            //finish this by looking @ Ranseur OI device for example
            //Stop/Start collecting actions in GPM

        }

    }
}
