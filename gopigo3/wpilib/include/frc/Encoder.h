#pragma once

#include "CounterBase.h"
#include <cstdint>

namespace frc
{
    class Encoder : CounterBase
    {
    public:
        Encoder(uint32_t aChannel, uint32_t bChannel, bool reverseDirection = false, EncodingType encodingType = k1X);
        virtual ~Encoder();

        int32_t Get() const;
        int32_t GetEncodingScale() const;
        void Reset();
        void SetReverseDirection(bool v) {
            inverted_ = v ;
        }

    private:
        uint32_t m_a;
        uint32_t m_b;
        uint32_t m_index;
        EncodingType m_type;

        bool inverted_ ;

        uint8_t m_hw_channel;
    };
}
