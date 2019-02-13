#include "FloorCollectCargoAction.h"
#include <singlemotorsubsystem/SingleMotorPowerAction.h>
#include <lifter/LifterGoToHeightAction.h>
#include "turntable/TurntableGoToAngleAction.h"
#include "cargointake/CargoIntakeAction.h"

using namespace xero::base ;

namespace xero {
    namespace phaser {
        FloorCollectCargoAction::FloorCollectCargoAction(GamePieceManipulator &subsystem):GamePieceAction(subsystem)
        {
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto cargo_intake = getGamePiece().getCargoIntake();
            auto cargo_holder = getGamePiece().getCargoHolder();

            set_lifter_safe_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "set_lifter_safe_height_") ;
            set_turntable_cargo_angle_ = std::make_shared<TurntableGoToAngleAction>(*turntable, "set_turntable_cargo_angle_") ;
            set_lifter_cargo_intake_height_ = std::make_shared<LifterGoToHeightAction>(*lifter, "set_lifter_cargo_intake_height_") ;
            deploy_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, "deploy_cargo_intake_") ;
            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, "retract_cargo_intake_") ;

            
            set_cargo_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_intake, "set_cargo_intake_motor_") ;
            stop_cargo_intake_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
            set_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, "set_cargo_holder_motors_") ;
            stop_cargo_holder_motor_ = std::make_shared<SingleMotorPowerAction>(*cargo_holder, 0.0) ;
        }

        FloorCollectCargoAction::~FloorCollectCargoAction() {

        }

        void FloorCollectCargoAction::start() {

        }

        void FloorCollectCargoAction::run() {

        }
        
        bool FloorCollectCargoAction::isDone() {
            return true ;
        }

        void FloorCollectCargoAction::cancel() {

        }

        std::string FloorCollectCargoAction::toString() {
            std::string ret("FloorCollectCargoAction") ;
            return ret ;
        }
    }
}