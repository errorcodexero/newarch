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

        StartCollectAction::StartCollectAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem), parallel_(getMessageLogger())
        {
        }

        StartCollectAction::~StartCollectAction()
        {
        }

        void StartCollectAction::start() 
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            
            collect_on_action_ = std::make_shared<CollectOnAction>(*intake) ;
            conveyor_receive_action_ = std::make_shared<ConveyorReceiveAction>(*conveyor) ;
            
            parallel_.addAction(collect_on_action_) ;
            parallel_.addAction(conveyor_receive_action_) ;
        }

        void StartCollectAction::run() 
        {
            parallel_.run() ;
        }

    }
}