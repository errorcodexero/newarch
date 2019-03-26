#include "GamePieceManipulator.h"
#include "GamePieceAction.h"
#include "phaserids.h"
#include <Robot.h>
#include <list>

using namespace xero::base ;
using namespace xero::misc ;

namespace xero {
    namespace phaser {
        GamePieceManipulator::GamePieceManipulator(Robot &robot) : Subsystem(robot, "gamepiecemanipulator") 
        {
            SettingsParser &settings = robot.getSettingsParser() ;            
            bool victor = true ;

            hatch_holder_ = std::make_shared<CarlosHatch>(robot) ;
            addChild(hatch_holder_) ;

            if (settings.isDefined("cargoholder:TalonSRX"))
                victor = !settings.getBoolean("cargoholder:TalonSRX") ;

            cargo_holder_ = std::make_shared<CargoHolder>(robot, MSG_GROUP_CARGO_HOLDER, victor) ;
            addChild(cargo_holder_) ;

            cargo_intake_ = std::make_shared<CargoIntake>(robot, MSG_GROUP_CARGO_INTAKE) ;
            addChild(cargo_intake_) ;

            lifter_ = std::make_shared<Lifter>(robot, MSG_GROUP_PHASER_LIFTER) ;
            addChild(lifter_) ;

            turntable_ = std::make_shared<Turntable>(robot, *lifter_, MSG_GROUP_PHASER_TURNTABLE, MSG_GROUP_PHASER_TURNTABLE_VERBOSE) ;
            addChild(turntable_) ;
        }

        GamePieceManipulator::~GamePieceManipulator() 
        {
        }

        void GamePieceManipulator::init(LoopType ltype)
        {
            if (ltype == LoopType::OperatorControl) {
                auto action = getAction() ;
                if (action != nullptr && !action->isDone())
                    setAction(action) ;
            }
        }

        void GamePieceManipulator::run() 
        {
            Subsystem::run() ;
        }

        bool GamePieceManipulator::canAcceptAction(ActionPtr action) {
            std::shared_ptr<GamePieceAction> gact = std::dynamic_pointer_cast<GamePieceAction>(action) ;
            return gact != nullptr ;
        }

        GamePieceType GamePieceManipulator::getGamePieceType() {
            GamePieceType ret = GamePieceType::Invalid ;
            bool hatch = hatch_holder_->hasHatch() ;
            bool cargo = cargo_holder_->hasCargo() ;

            if (hatch && !cargo)
                ret = GamePieceType::Hatch ;
            else if (!hatch && cargo)
                ret = GamePieceType::Cargo ;
            else if (!hatch && !cargo)
                ret = GamePieceType::None ;

            return ret ;
        }
    }
}
