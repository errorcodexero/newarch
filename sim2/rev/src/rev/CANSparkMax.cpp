#include "CANSparkMax.h"

namespace rev
{
    CANSparkMax::CANSparkMax(int devid, MotorType type) : CANSparkMaxLowLevel(devid, type) {
    }

    CANSparkMax::~CANSparkMax() {  
    }

}

