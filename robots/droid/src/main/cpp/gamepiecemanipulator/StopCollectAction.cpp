#include "CollectAction.h"
#include "shooter/FireAction.h"
#include <gamepiecemanipulator/GamePieceManipulator.h>

namespace xero {
    namespace droid {

        StopCollectAction::StopCollectAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem)
        {
        }

        StopCollectAction::~StopCollectAction()
        {
        }

        void StopCollectAction::start() 
        {
        }

        void StopCollectAction::run() 
        {
        }
    }
}