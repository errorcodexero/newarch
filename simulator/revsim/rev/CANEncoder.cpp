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
        std::lock_guard<std::mutex> lock(getLockMutex());
        return pos_;
    }

    void CANEncoder::SimulatorSetPosition(double pos)
    {
        std::lock_guard<std::mutex> lock(getLockMutex());
        if (reverse_)
            pos_ = -pos ;
        else
            pos_ = pos ;
    }
}