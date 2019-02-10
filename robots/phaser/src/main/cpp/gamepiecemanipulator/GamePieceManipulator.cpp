#include "GamePieceManipulator.h"
#include "phaserids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        GamePieceManipulator::GamePieceManipulator(Robot &robot) : RobotSubsystem(robot, "phaser") {

            oi_ = std::make_shared<PhaserOISubsystem>(robot) ;
            addChild(oi_) ;        

            hatch_holder_ = std::make_shared<HatchHolder>(robot) ;
            addChild(hatch_holder_) ;

            // hatch_intake_ = std::make_shared<HatchIntake>(robot, MSG_GROUP_HATCH_INTAKE) ;
            // addChild(hatch_intake_) 

            cargo_holder_ = std::make_shared<CargoHolder>(robot) ;
            addChild(cargo_holder_) ;

            cargo_intake_ = std::make_shared<CargoIntake>(robot, MSG_GROUP_CARGO_INTAKE) ;
            addChild(cargo_intake_) ;

            lifter_ = std::make_shared<Lifter>(robot, MSG_GROUP_PHASER_LIFTER) ;
            addChild(lifter_) ;

            turntable_ = std::make_shared<Turntable>(robot, MSG_GROUP_PHASER_TURNTABLE) ;
            addChild(turntable_) ;
            
        }

        GamePieceManipulator::~GamePieceManipulator() {
        }
    }
}
