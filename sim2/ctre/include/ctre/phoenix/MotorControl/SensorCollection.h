#pragma once

#include <SimulatorEngine.h>
#include <CTREManager.h>

namespace ctre {
    namespace phoenix {
        namespace motorcontrol {
            class SensorCollection {
            public:
                SensorCollection(int index) {
                    index_ = index ;
                }

                int GetQuadraturePosition() { return 0; }

                double GetIntegratedSensorPosition() {
                    double ret ;

                    xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
                    auto mgr = engine.getCTREManager();

                    mgr->getEncoder(index_, ret) ;

                    return ret ;
                }

            private:
                int index_ ;
            };
        }
    }
}
