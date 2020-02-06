#include "GamePieceManipulator.h"

#include "GamePieceManipulatorAction.h"
#include "shooter/Shooter.h"
#include "intake/Intake.h"
#include "conveyor/Conveyor.h"

using namespace xero::base;

namespace xero {
    namespace droid {
        GamePieceManipulator::GamePieceManipulator(Subsystem *parent): Subsystem(parent, "gamepiecemanipulator") {
            shooter_ = std::make_shared<Shooter>(this);
            addChild(shooter_);
            
            intake_ = std::make_shared<Intake>(this);
            addChild(intake_);

            conveyor_ = std::make_shared<Conveyor>(this);
            addChild(conveyor_);
        }
    
        bool GamePieceManipulator::canAcceptAction(xero::base::ActionPtr action) {
            return std::dynamic_pointer_cast<GamePieceManipulatorAction>(action) != nullptr;
        }
    }
}