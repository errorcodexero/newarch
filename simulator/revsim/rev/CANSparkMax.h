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
        CANSparkMax(int devid, MotorType type) : CANSparkMaxLowLevel(devid, type) {
        }

        virtual ~CANSparkMax() {            
        }

        void Follow(const CANSparkMax &leader, bool invert = false) {                
        }

        void SetInverted(bool b) {
            inverted_ = b ;
        }

        bool GetInverted() const {
            return inverted_ ;
        }

        void SetIdleMode(IdleMode mode) {
            idle_mode_ = mode ;
        }

    private:

        bool inverted_ ;
        IdleMode idle_mode_ ;
    } ;
}