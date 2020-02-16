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
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;

            collect_on_action_ = std::make_shared<CollectOnAction>(*intake) ;
            conveyor_receive_action_ = std::make_shared<ConveyorReceiveAction>(*conveyor) ;
            
            parallel_.addAction(collect_on_action_) ;
            parallel_.addAction(conveyor_receive_action_) ;            
        }

        StartCollectAction::~StartCollectAction()
        {
        }

        void StartCollectAction::start() 
        {
            xero::base::Action::start() ;
            parallel_.start() ;
        }

        void StartCollectAction::run() 
        {
            xero::base::Action::run() ;
            parallel_.run() ;

            if (parallel_.isDone())
                setDone() ;
        }

        void StartCollectAction::cancel()
        {
            parallel_.cancel() ;
            setDone() ;
        }
    }
}