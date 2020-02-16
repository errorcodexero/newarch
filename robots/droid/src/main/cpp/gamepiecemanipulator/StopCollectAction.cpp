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

        StopCollectAction::StopCollectAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem), parallel_(getMessageLogger())
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto intake = droid.getDroidSubsystem()->getGamePieceManipulator()->getIntake() ;   
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            
            collect_off_action_ = std::make_shared<CollectOffAction>(*intake) ;
            conveyor_stop_action_ = std::make_shared<ConveyorStopAction>(*conveyor) ;
    
            parallel_.addAction(collect_off_action_) ;
            parallel_.addAction(conveyor_stop_action_) ;            
        }

        StopCollectAction::~StopCollectAction()
        {
        }

        void StopCollectAction::start() 
        {
            xero::base::Action::start() ;
            parallel_.start() ;
        }

        void StopCollectAction::run() 
        {
            parallel_.run() ;
            if (parallel_.isDone())
                setDone() ;
        }

        void StopCollectAction::cancel()
        {
            parallel_.cancel() ;
            setDone() ;
        }
    }
}