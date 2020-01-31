#include "CANSparkMaxLowLevel.h"
#include <REVManager.h>
#include <SimulatorEngine.h>

void rev::CANSparkMaxLowLevel::Set(double value) {
    if (value > 1.0)
        value = 1.0 ;
    else if (value < -1.0)
        value = -1.0 ;
        
    power_ = value ;
    
    xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
    auto mgr = engine.getREVManager();
    mgr->set(GetDeviceID(), value);
}