#include "ShootTestingAction.h"
#include "shooter/ShooterVelocityAction.h"
#include "conveyor/ConveyorPrepareToReceiveAction.h"
#include "conveyor/ConveyorPrepareToEmitAction.h"
#include "conveyor/ConveyorEmitAction.h"
#include "conveyor/ConveyorReceiveAction.h"
#include <gamepiecemanipulator/shooter/Shooter.h>
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include <frc/smartdashboard/SmartDashboard.h>

namespace xero {
    namespace droid {

        static frc::SimpleWidget makeWidget(GamePieceManipulator &subsystem) {
            wpi::StringMap<std::shared_ptr<nt::Value>> propmap;
            propmap.insert(std::make_pair("min", nt::Value::MakeDouble(0.0)));
            propmap.insert(std::make_pair("max", nt::Value::MakeDouble(10000.0)));
            double defaultValue = subsystem.getRobot().getSettingsParser().getDouble("shoottest:velocity");
            return frc::Shuffleboard::GetTab("SmartDashboard").Add("Velocity", static_cast<double>(defaultValue)).WithWidget(frc::BuiltInWidgets::kNumberSlider).WithProperties(propmap);
        }

        ShootTestingAction::ShootTestingAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem), widget_(makeWidget(subsystem)) {
            fire_ = std::make_shared<ShooterVelocityAction>(*getSubsystem().getShooter(), 0.0, Shooter::HoodPosition::Up);
            shoot_delay_ = subsystem.getRobot().getSettingsParser().getDouble("shoottest:shoot_delay") ;
        }

        ShootTestingAction::~ShootTestingAction()
        {
        }

        void ShootTestingAction::start() 
        {
            auto conveyor = getSubsystem().getConveyor() ;
            auto shooter = getSubsystem().getShooter();

            state_ = FireState::WaitPrepareReceive ;
            fire_->setTarget(0.0);
            shooter->setAction(fire_, true);
            conveyor->setAction(std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor), true) ;            
        }

        void ShootTestingAction::run() 
        {
            auto conveyor = getSubsystem().getConveyor() ;
            auto shooter = getSubsystem().getShooter();
            double current = fire_->getTarget();
            double target = widget_.GetEntry().GetDouble(current);

            if (std::fabs(current - target) > 10)
                fire_->setTarget(target);
            frc::SmartDashboard::PutBoolean("ReadyShoot", shooter->isReadyToFire()) ;

            switch(state_)
            {
            case FireState::WaitPrepareReceive:
                if (!conveyor->isBusy())
                {
                    state_ = FireState::WaitReceive ;
                    conveyor->setAction(std::make_shared<ConveyorReceiveAction>(*conveyor), true) ;
                }
                break ;

            case FireState::WaitReceive:
                if (!conveyor->isBusy())
                {
                    state_ = FireState::WaitPrepareShoot ;
                    conveyor->setAction(std::make_shared<ConveyorPrepareToEmitAction>(*conveyor), true) ;
                }
                break ;

            case FireState::WaitPrepareShoot:
                if (!conveyor->isBusy())
                {
                    state_ = FireState::WaitShootDelay ;
                    start_ = getSubsystem().getRobot().getTime() ;
                }
                break ;

            case FireState::WaitShootDelay:
                if (getSubsystem().getRobot().getTime() - start_ > shoot_delay_)
                {
                    state_ = FireState::WaitShoot ;
                    conveyor->setAction(std::make_shared<ConveyorEmitAction>(*conveyor), true) ;
                }
                break ;

            case FireState::WaitShoot:
                if (!conveyor->isBusy())
                {
                    state_ = FireState::WaitPrepareReceive ;
                    conveyor->setAction(std::make_shared<ConveyorPrepareToReceiveAction>(*conveyor), true) ;
                }
                break ;
            }
        }
    }
}