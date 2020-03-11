#include "CANEncoder.h"
#include "CANSparkMaxLowLevel.h"
#include <REVManager.h>

namespace rev
{
    CANEncoder::CANEncoder(CANSparkMaxLowLevel &motor) 
    {
        motor_ = &motor;
        reverse_ = false;
    }

    CANEncoder::~CANEncoder()
    {        
    }

    double CANEncoder::GetPosition()
    {
        double v ;

        xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
        auto mgr = engine.getREVManager();
        if (!mgr->getEncoder(motor_->GetDeviceID(), v))
            v = 0.0 ;

        return v ;
    }

    void CANEncoder::SetPosition(double pos)
    {
        xero::sim2::SimulatorEngine &engine = xero::sim2::SimulatorEngine::getEngine();
        auto mgr = engine.getREVManager();
        mgr->setEncoder(motor_->GetDeviceID(), pos) ;
    }
}
