#pragma once

#include "GamePieceManipulator.h"

#include "shooter/Shooter.h"
#include "intake/Intake.h"
#include "conveyor/Conveyor.h"

namespace xero {
    namespace droid {
        class ShooterVelocityAction;
        class FireAction : public xero::base::Action {
        public:
            FireAction(GamePieceManipulator &subsystem);

            GamePieceManipulator &getSubsystem() {
                return subsystem_ ;
            }

            void start() override ;
            void run() override ;
            
        private:
            GamePieceManipulator &subsystem_ ;

            std::shared_ptr<ShooterVelocityAction> shooterVelocityAction_;
        };
    }

}
