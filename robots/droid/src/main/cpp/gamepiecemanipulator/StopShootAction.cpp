#include "StopShootAction.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include "intake/Intake.h"
#include "intake/CollectOnAction.h"
#include "conveyor/Conveyor.h"
#include "conveyor/ConveyorStopAction.h"
#include "shooter/SetHoodAction.h"
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
            auto shooter = droid.getDroidSubsystem()->getGamePieceManipulator()->getShooter();
            
            conveyor_stop_action_ = std::make_shared<ConveyorStopAction>(*conveyor) ;
            hood_down_action_ = std::make_shared<SetHoodAction>(*shooter, true);
            
            parallel_.addSubActionPair(conveyor, conveyor_stop_action_) ;
            parallel_.addSubActionPair(shooter, hood_down_action_);
        }

        void StopShootAction::run() 
        {
            parallel_.run() ;
            if (parallel_.isDone()) setDone();
        }

        void StopShootAction::cancel() {
            parallel_.cancel();
            setDone();
        }
    }
}