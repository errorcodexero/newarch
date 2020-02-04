#pragma once

#include "CANSparkMaxLowLevel.h"

namespace rev
{
    class CANSparkMax : public CANSparkMaxLowLevel
    {
    public:
        enum IdleMode { 
            kCoast = 0, 
            kBrake = 1 
        } ;

    public:
        CANSparkMax(int devid, MotorType type)  ;
        virtual ~CANSparkMax() ;

        void Follow(const CANSparkMax &leader, bool invert = false) {                
        }

        void SetIdleMode(IdleMode mode) {
            idle_mode_ = mode ;
        }

    private:
        IdleMode idle_mode_ ;
    } ;
}
