#pragma once


#include <ctre/phoenix/MotorControl/SensorCollection.h>
#include <ctre/phoenix/MotorControl/ControlMode.h>
#include <ctre/phoenix/MotorControl/NeutralMode.h>
#include <ctre/phoenix/MotorControl/IMotorController.h>
#include <ctre/phoenix/MotorControl/InvertType.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            namespace can {
                class TalonSRX : public IMotorController
                {
                public:
                    TalonSRX(int index);
                    virtual ~TalonSRX();

                private:

                };
            }
        }
    }
}
