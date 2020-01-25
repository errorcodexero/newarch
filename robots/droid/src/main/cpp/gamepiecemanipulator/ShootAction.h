#pragma once

#include "GamePieceManipulator.h"

#include "shooter/Shooter.h"
#include "intake/Intake.h"
#include "conveyor/Conveyor.h"

namespace xero {
    namespace droid {
        class ShootAction : public xero::base::Action {
        public:
            ShootAction(GamePieceManipulator &subsystem);

            GamePieceManipulator &getSubsystem() {
                return subsystem_ ;
            }

            void start() override ;
            void run() override ;
            
        private:
            GamePieceManipulator &subsystem_ ;

        };
    }

}
