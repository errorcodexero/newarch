#include "CANSparkMax.h"

namespace rev
{
    CANSparkMax::CANSparkMax(int index, CANSparkMaxLowLevel::MotorType mtype)
    {
        id_ = index ;
        power_ = 0.0 ;
        mtype_ = mtype ;
    }

    CANSparkMax::~CANSparkMax()
    {        
    }
}