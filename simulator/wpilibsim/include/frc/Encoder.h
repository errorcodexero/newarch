#pragma once

#include "SimulatedObject.h"
#include <cstdint>

namespace frc
{
    class Encoder : public xero::sim::SimulatedObject
    {
    public:
        Encoder(int first, int second);
        virtual ~Encoder();

        void Reset()  ;
        int32_t Get()  ;

        void SimulatorGetDigitalIOs(int &first, int &second) {
            first = first_ ;
            second = second_ ;
        }

        void SimulatorSetValue(int value) ;

        void SetReverseDirection(bool b) {
            reverse_ = b ;
        }

    private:
        int value_ ;
        int first_;
        int second_;
        bool reverse_ ;
    };
}

