#include "StopCollectAction.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include "intake/Intake.h"
#include "intake/CollectOffAction.h"
#include "conveyor/Conveyor.h"
#include "conveyor/ConveyorStopAction.h"
#include "actions/ParallelAction.h"
#include "Droid.h"

namespace xero {
    namespace droid {

        StopCollectAction::StopCollectAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            

            conveyor_stop_action_ = std::make_shared<ConveyorStopAction>(*conveyor) ;
        }

        StopCollectAction::~StopCollectAction()
        {
        }

        void StopCollectAction::start() 
        {
            xero::base::Action::start() ;     
                   
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            auto collect_off_action = std::make_shared<CollectOffAction>(*intake) ;            
            intake->setAction(collect_off_action, true) ;
            conveyor->setAction(conveyor_stop_action_, true) ;
        }

        void StopCollectAction::run() 
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            if (!intake->isBusy())
                setDone() ;            
        }

        void StopCollectAction::cancel()
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            if (intake->isBusy())
                intake->cancelAction() ;

            if (conveyor->isBusy())
                conveyor->cancelAction() ;

            setDone() ;
        }
    }
}