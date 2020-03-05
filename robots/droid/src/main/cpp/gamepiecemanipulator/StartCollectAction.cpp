#include "StartCollectAction.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include "intake/Intake.h"
#include "intake/CollectOnAction.h"
#include "conveyor/Conveyor.h"
#include "conveyor/ConveyorReceiveAction.h"
#include "actions/ParallelAction.h"
#include "Droid.h"

namespace xero {
    namespace droid {

        StartCollectAction::StartCollectAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem)
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            conveyor_receive_action_ = std::make_shared<ConveyorReceiveAction>(*conveyor) ;
        }

        StartCollectAction::~StartCollectAction()
        {
            
        }

        void StartCollectAction::start() 
        {
            xero::base::Action::start() ;
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            auto collect_on_action = std::make_shared<CollectOnAction>(*intake) ;
            intake->setAction(collect_on_action, true) ;
            conveyor->setAction(conveyor_receive_action_, true) ;            
        }

        void StartCollectAction::run() 
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            if (!intake->isBusy() && !conveyor->isBusy())
                setDone() ;
        }

        void StartCollectAction::cancel()
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