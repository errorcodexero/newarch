#pragma once

#include "GamePieceManipulatorAction.h"
#include <gamepiecemanipulator/shooter/ShooterVelocityAction.h>
#include <memory>

namespace xero {
    namespace droid {
        class ShootTestingAction : public GamePieceManipulatorAction {
        public:
            ShootTestingAction(GamePieceManipulator &subsystem);
            virtual ~ShootTestingAction();

            void start() override ;
            void run() override ;

            std::string toString() override {
                return "ShootTestingAction";
            }            

        private:
            enum class FireState
            {
                WaitPrepareReceive,
                WaitReceive,
                WaitPrepareShoot,
                WaitShootDelay,
                WaitShoot,
            } ;
            
        private:
            std::shared_ptr<ShooterVelocityAction> fire_;
            frc::SimpleWidget widget_;     
            FireState state_ ;       
            double shoot_delay_ ;
            double start_ ;
        };
    }

}
