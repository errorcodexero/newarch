#include "GamePieceManipulator.h"

#include "shooter/Shooter.h"
#include "collector/Collector.h"
#include "conveyor/Conveyor.h"

using namespace xero::base;

namespace xero {
    namespace droid {
        GamePieceManipulator::GamePieceManipulator(Subsystem *parent): Subsystem(parent, "gamepiecemanipulator") {
            shooter_ = std::make_shared<Shooter>(this);
            addChild(shooter_);
            
            collector_ = std::make_shared<Collector>(this);
            addChild(collector_);

            conveyor_ = std::make_shared<Conveyor>(this);
            addChild(conveyor_);
        }
    }
}