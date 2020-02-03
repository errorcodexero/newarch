#include "ShootTestingAction.h"
#include "shooter/FireAction.h"
#include "conveyor/ConveyorOnAction.h"
#include <gamepiecemanipulator/shooter/Shooter.h>
#include <gamepiecemanipulator/GamePieceManipulator.h>
#include <frc/smartdashboard/SmartDashboard.h>

namespace xero {
    namespace droid {

        static frc::SimpleWidget makeWidget() {
            wpi::StringMap<std::shared_ptr<nt::Value>> propmap;
            propmap.insert(std::make_pair("min", nt::Value::MakeDouble(0.0)));
            propmap.insert(std::make_pair("max", nt::Value::MakeDouble(6000.0)));
            return frc::Shuffleboard::GetTab("SmartDashboard").Add("Velocity", static_cast<double>(0.0)).WithWidget(frc::BuiltInWidgets::kNumberSlider).WithProperties(propmap);
        }

        ShootTestingAction::ShootTestingAction(GamePieceManipulator &subsystem) : GamePieceManipulatorAction(subsystem), widget_(makeWidget())
        {
            fire_ = std::make_shared<FireAction>(*getSubsystem().getShooter(), 0.0);            
        }

        ShootTestingAction::~ShootTestingAction()
        {
        }

        void ShootTestingAction::start() 
        {
            auto conact = std::make_shared<ConveyorOnAction>(*getSubsystem().getConveyor());
            getSubsystem().getConveyor()->setAction(conact);

            fire_->setTarget(0.0);
            getSubsystem().getShooter()->setAction(fire_);
        }

        void ShootTestingAction::run() 
        {
            auto shooter = getSubsystem().getShooter();
            double current = fire_->getTarget();
            double target = widget_.GetEntry().GetDouble(current);

            if (std::fabs(current - target) > 10)
                fire_->setTarget(target);

            frc::SmartDashboard::PutBoolean("ReadyShoot", shooter->isReadyToShoot());
        }
    }
}