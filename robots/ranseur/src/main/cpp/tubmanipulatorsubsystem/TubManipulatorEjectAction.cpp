#include "TubManipulatorEjectAction.h"
#include "TubManipulatorAction.h"
#include "TubManipulatorSubsystem.h"
#include <actions/Action.h>
#include <MessageLogger.h>
#include <Robot.h>

using namespace xero::misc ;

namespace xero {
    namespace ranseur {

        std::string TubManipulatorEjectAction::action_name("TubManipulatorEjectAction");

        TubManipulatorEjectAction::TubManipulatorEjectAction(TubManipulator &tubm) : TubManipulatorAction(tubm) {
        }

        TubManipulatorEjectAction::~TubManipulatorEjectAction() {            
        }
        
        void TubManipulatorEjectAction::start() {
        }

        void TubManipulatorEjectAction::run() {
        }

        bool TubManipulatorEjectAction::isDone() {
            return true;  // TEMPORARY
        }

        void TubManipulatorEjectAction::cancel() {
        }

    }
}
