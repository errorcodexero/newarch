#pragma once

#include "GamePieceManipulator.h"
#include "GamePieceManipulatorAction.h"
#include "shooter/Shooter.h"
#include "intake/Intake.h"
#include "conveyor/Conveyor.h"

namespace xero {
    namespace droid {
        class ShooterVelocityAction;
        class ConveyorEmitAction;
        class DroidSubsystem;
        class FireAction : public GamePieceManipulatorAction {
        public:
            FireAction(GamePieceManipulator &subsystem);

            void start() override ;
            void run() override ;
            void cancel() override;

            std::string toString() override {
                return "FireAction" ;
            }
            
        private:
            DroidSubsystem &droidSubsystem_;

            void stopChildActions();

            double drivebaseVelocityThreshold_;
            double cameraSampleAgeThreshold_;

            std::shared_ptr<ShooterVelocityAction> shooterVelocityAction_;
            std::shared_ptr<ConveyorEmitAction> conveyorEmitAction_;
            bool isFiring_;
        };
    }

}
