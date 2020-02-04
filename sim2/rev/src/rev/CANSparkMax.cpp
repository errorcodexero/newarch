#include "CANSparkMax.h"
#include <SimulatorEngine.h>
#include <REVManager.h>
namespace rev
{
    CANSparkMax::CANSparkMax(int devid, MotorType type) : CANSparkMaxLowLevel(devid, type) {
        xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
        auto mgr = engine.getREVManager();
        mgr->create(devid);
    }

    CANSparkMax::~CANSparkMax() { 
        xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
        auto mgr = engine.getREVManager();
        mgr->destroy(GetDeviceID());         
    }
}

