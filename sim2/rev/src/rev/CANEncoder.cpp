#include "CANEncoder.h"

namespace rev
{
    CANEncoder::CANEncoder(CANSparkMaxLowLevel &motor) 
    {
        motor_ = &motor;
        pos_ = 0.0;
        reverse_ = false;
    }

    CANEncoder::~CANEncoder()
    {        
    }

    double CANEncoder::GetPosition()
    {
        return pos_;
    }

    void CANEncoder::SimulatorSetPosition(double pos)
    {
        if (reverse_)
            pos_ = -pos ;
        else
            pos_ = pos ;
    }

    void CANEncoder::SetPosition(double pos)
    {
        pos_ = pos ;
    }
}