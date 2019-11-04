#pragma once

#include "SimulatedObject.h"

namespace frc {
    class Relay : public xero::sim::SimulatedObject
    {
    public:
        enum Value { kOff, kOn, kForward, kReverse };
        enum Direction { kBothDirections, kForwardOnly, kReverseOnly };

    public:
        Relay(int channel, Direction dir = kBothDirections) ;
        virtual ~Relay() ;

        void Set(Value v) ;

        Value Get() {
            return state_ ;
        }

        int GetChannel() {
            return channel_ ;
        }

    private:
        int channel_ ;
        Value state_ ;
        Direction dir_ ;
    } ;
}