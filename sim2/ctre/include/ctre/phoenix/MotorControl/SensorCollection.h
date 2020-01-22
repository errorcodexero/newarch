#pragma once

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            class SensorCollection {
            public:
                int GetQuadraturePosition() { return 0; }

                double GetIntegratedSensorPosition() {
                    return 0.0;
                }                
            };
        }
    }
}
