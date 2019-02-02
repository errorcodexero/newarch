#pragma once


#include <ctre/phoenix/MotorControl/SensorCollection.h>
#include <ctre/phoenix/MotorControl/ControlMode.h>
#include <ctre/phoenix/MotorControl/NeutralMode.h>
#include <ctre/phoenix/MotorControl/IMotorController.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            namespace can {
                class VictorSPX : public IMotorController
                {
                public:
                    VictorSPX(int index);
                    virtual ~VictorSPX();

                private:

                };
            }
        }
    }
}
