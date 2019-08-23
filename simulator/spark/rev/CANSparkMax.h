#pragma once

#include "rev/CANSparkMaxLowLevel.h"

namespace rev
{
    class CANSparkMax
    {
    public:
        CANSparkMax(int id, CANSparkMaxLowLevel::MotorType mt) ;
        virtual ~CANSparkMax() ;

    private:
        int id_ ;
        double power_ ;
        CANSparkMaxLowLevel::MotorType mtype_ ;
    } ;
}