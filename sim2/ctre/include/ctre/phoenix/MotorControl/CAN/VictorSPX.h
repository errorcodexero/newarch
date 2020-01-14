#pragma once

#include <ctre/phoenix/MotorControl/IMotorController.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            namespace can {
                class VictorSPX : public IMotorController 
                {
                public:
                    VictorSPX(int id) : IMotorController(id) {                   
                    }

                    virtual ~VictorSPX() {                        
                    }
                };
            }
        }
    }
}