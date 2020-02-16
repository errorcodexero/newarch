#include "StartShootAction.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include "intake/Intake.h"
#include "intake/CollectOffAction.h"
#include "conveyor/Conveyor.h"
#include "conveyor/ConveyorReceiveAction.h"
#include "actions/ParallelAction.h"
#include "Droid.h"
#include "FireAction.h"

namespace xero {
    namespace droid {
        StartShootAction::StartShootAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto game_piece_manip = droid.getDroidSubsystem()->getGamePieceManipulator() ;            
            collect_off_action_ = std::make_shared<CollectOffAction>(*intake) ;
            fire_action_ = std::make_shared<FireAction>(*game_piece_manip) ;            
        }

        StartShootAction::~StartShootAction()
        {
        }

        void StartShootAction::start() 
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto game_piece_manip = droid.getDroidSubsystem()->getGamePieceManipulator() ; 

            intake->setAction(collect_off_action_) ;
            game_piece_manip->setAction(fire_action_) ;            
        }

        void StartShootAction::run() 
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto game_piece_manip = droid.getDroidSubsystem()->getGamePieceManipulator() ; 

            if (!intake->isBusy() && !game_piece_manip->isBusy())
                setDone() ;
        }
    }
}