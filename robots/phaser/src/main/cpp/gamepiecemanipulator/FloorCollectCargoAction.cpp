#include "FloorCollectCargoAction.h"

namespace xero {
    namespace phaser {
        FloorCollectCargoAction::FloorCollectCargoAction()
        {
            auto lifter = getGamePiece().getLifter() ;
            auto turntable = getGamePiece().getTurntable();
            auto cargo_intake = getGamePiece().getCargoIntake();
            auto cargo_holder = getGamePiece().getCargoHolder();

            set_lifter_safe_height_ = std::make_shared<LifterGoToHeight>(*lifter, "set_lifter_safe_height_") ;
            set_turntable_cargo_angle_ = std::make_shared<TurntableGoToAngle>(*turntable, "set_turntable_cargo_angle_") ;
            set_lifter_cargo_intake_height_ = std::make_shared<LifterGoToHeight>(*lifter, "set_lifter_cargo_intake_height_") ;
            deploy_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, "deploy_cargo_intake_") ;
            retract_cargo_intake_ = std::make_shared<CargoIntakeAction>(*cargo_intake, "retract_cargo_intake_") ;

            
            set_cargo_intake_motor_ = <SingleMotorPowerAction>(SingleMotorSubsystem &subsystem, const "set_cargo_intake_motor_") ;
            stop_cargo_intake_motor = <SingleMotorPowerAction>(SingleMotorSubsystem &subsystem, double 0.0) ;
            set_cargo_holder_motors_ = <SingleMotorPowerAction>(SingleMotorSubsystem &subsystem, const "set_cargo_holder_motors_") ;
            stop_cargo_holder_motors_ = <SingleMotorPowerAction>(SingleMotorSubsystem &subsystem, double 0.0) ;
        }

        FloorCollectCargoAction::~FloorCollectCargoAction() {

        }
    }
}