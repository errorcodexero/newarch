#include "TubManipulatorCollectAction.h"
#include "TubManipulatorAction.h"
#include "TubManipulatorSubsystem.h"
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::string TubManipulatorCollectAction::action_name("TubManipulatorCollectAction");

        TubManipulatorCollectAction::TubManipulatorCollectAction(TubManipulator &tubm) : TubManipulatorAction(tubm) {
        }

        TubManipulatorCollectAction::~TubManipulatorCollectAction() {            
        }
        
        void TubManipulatorCollectAction::start() {
        }

        void TubManipulatorCollectAction::run() {
        }

        bool TubManipulatorCollectAction::isDone() {
            return true;  // TEMPORARY
        }

        void TubManipulatorCollectAction::cancel() {
        }

    }
}
