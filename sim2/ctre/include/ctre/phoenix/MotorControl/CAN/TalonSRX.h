#pragma once

#include <ctre/phoenix/MotorControl/IMotorController.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            namespace can {
                class TalonSRX : public IMotorController
                {
                public:
                    TalonSRX(int id) : IMotorController(id) {
                    }

                    virtual ~TalonSRX() {                        
                    }
                };
            }
        }
    }
}