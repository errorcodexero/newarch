#pragma once

#include <Subsystem.h>
#include <PIDCtrl.h>
#include <motors/MotorController.h>

#include <frc/Encoder.h>

#include <memory>
#include <limits>
#include <vector>

namespace xero {
    namespace ranseur {
        class TubArm : public xero::base::Subsystem {
        //public:
        //private:
        };
        /*
        class TubArm : public xero::base::Subsystem {
        public:
            enum class ArmPosition {
                LOWERED,
                OFF_GROUND,
                RAISED
            };
            enum class WristPosition {
                LOWERED,
                RAISED
            };

            TubArm(xero::base::Robot &robot, std::string configID);
            virtual ~TubArm();
            
            virtual void computeState();

            std::shared_ptr<xero::base::MotorController> getArmMotor() { return armMotor_; }
            std::shared_ptr<xero::base::MotorController> getWristMotor() { return armMotor_; }

        private:
            std::shared_ptr<xero::base::MotorController> armMotor_;
            std::shared_ptr<xero::base::MotorController> wristMotor_;

            std::shared_ptr<frc::Encoder> armEncoder_;
            std::shared_ptr<frc::Encoder> wristEncoder_;

            // Encoder values for arm & wrist positions
            int armPositionLowered_;
            int armPositionOffGround_;
            int armPositionRaised_;

            int wristPositionLowered_;
            int wristPositionRaised_;
        };
        */
    }
}
