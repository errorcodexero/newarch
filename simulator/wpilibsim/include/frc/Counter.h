#pragma once

#include "SimulatedObject.h"

namespace frc
{
    class Counter : public xero::sim::SimulatedObject {
    public:
        enum Mode { kTwoPulse = 0, kSemiperiod = 1, kPulseLength = 2, kExternalDirection = 3 } ;

        Counter(Mode mode=kTwoPulse) ;
        Counter(int channel) ;
        virtual ~Counter() ;

        void SetSemiPeriodMode(bool highSemiPeriod) ;
        void SimulatorSetPeriod(int period) ;

        int GetPeriod() ;

    private:
        Mode mode_ ;
        int channel_ ;
        int period_ ;
        bool high_ ;
    } ;
}