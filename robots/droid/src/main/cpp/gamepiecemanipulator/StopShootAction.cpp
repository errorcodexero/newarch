#include "StopShootAction.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include "intake/Intake.h"
#include "intake/CollectOnAction.h"
#include "conveyor/Conveyor.h"
#include "conveyor/ConveyorStopAction.h"
#include "actions/ParallelAction.h"
#include "Droid.h"

namespace xero {
    namespace droid {

        StopShootAction::StopShootAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem), parallel_(getMessageLogger())
        {
        }

        StopShootAction::~StopShootAction()
        {
        }

        void StopShootAction::start() 
        {
            auto &droid = dynamic_cast<Droid &>(getSubsystem().getRobot()) ;            
            auto conveyor = droid.getDroidSubsystem()->getGamePieceManipulator()->getConveyor() ;
            
            conveyor_stop_action_ = std::make_shared<ConveyorStopAction>(*conveyor) ;
            
            parallel_.addAction(conveyor_stop_action_) ;
        }

        void StopShootAction::run() 
        {
            parallel_.run() ;
        }
    }
}