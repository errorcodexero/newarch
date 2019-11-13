#pragma once

namespace frc
{
    class Counter
    {
    public:
        enum Mode { kTwoPulse = 0, kSemiperiod = 1, kPulseLength = 2, kExternalDirection = 3 } ;

        Counter(Mode mode=kTwoPulse) ;
        Counter(int channel) ;
        virtual ~Counter() ;

        void SetSemiPeriodMode(bool highSemiPeriod) ;

        int GetPeriod() ;

    private:
        Mode mode_ ;
        int channel_ ;
        int period_ ;
    } ;
}