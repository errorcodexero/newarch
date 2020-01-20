#pragma once

#include <ctre/phoenix/MotorControl/IMotorController.h>
#include <SimulatorEngine.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            namespace can {
                class TalonSRX : public IMotorController
                {
                public:
                    TalonSRX(int id) : IMotorController(id) 
                    {
                        auto &engine = xero::sim2::SimulatorEngine::getEngine() ;
                        auto &msg = engine.getMessageOutput();

                        msg.startMessage(xero::sim2::SimulatorMessages::MessageType::Debug, 5) ;
                        msg << "TalonSRX: creating motor controller, index " << id;
                        msg.endMessage(engine.getSimulationTime());
                    }

                    virtual ~TalonSRX() {                        
                    }
                };
            }
        }
    }
}