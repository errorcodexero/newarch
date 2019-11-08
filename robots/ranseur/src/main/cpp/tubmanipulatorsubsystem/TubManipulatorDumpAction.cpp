#include "TubManipulatorDumpAction.h"
#include "TubManipulatorAction.h"
#include "TubManipulatorSubsystem.h"
#include <actions/Action.h>
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::string TubManipulatorDumpAction::action_name("TubManipulatorDumpAction") ;

        TubManipulatorDumpAction::TubManipulatorDumpAction(TubManipulator &tubm) : TubManipulatorAction(tubm) {
        }

        TubManipulatorDumpAction::~TubManipulatorDumpAction() {            
        }
        
        void TubManipulatorDumpAction::start() {
        }

        void TubManipulatorDumpAction::run() {
        }

        bool TubManipulatorDumpAction::isDone() {
            return true;  // TEMPORARY
        }

        void TubManipulatorDumpAction::cancel() {
        }

    }
}
