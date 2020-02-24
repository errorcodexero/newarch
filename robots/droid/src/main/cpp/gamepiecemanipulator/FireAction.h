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

            void setTargetVelocity();

            double drivebaseVelocityThreshold_;
            double cameraSampleAgeThreshold_;

            std::shared_ptr<ShooterVelocityAction> shooterVelocityAction_;
            std::shared_ptr<ConveyorEmitAction> conveyorEmitAction_;
            bool isFiring_;

            double hoodDown_a_;
            double hoodDown_b_;
            double hoodDown_c_;

            double hoodUp_a_;
            double hoodUp_b_;
            double hoodUp_c_;

            Shooter::HoodPosition  hoodIsDown_;
            double maxHoodUpDistance_;
            double minHoodDownDistance_;

            double startTime_;

            static std::vector<std::string> columns_;
            uint64_t plotid_;

        };
    }

}
