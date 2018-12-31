#pragma once
#include <cstdint>
namespace frc {
    class CounterBase
    {
    public:
        enum EncodingType {
            k1X,
            k2X,
            k4X
        };

    public:
        CounterBase() 
        {
        }

        virtual ~CounterBase()
        {
        }
    };

}

