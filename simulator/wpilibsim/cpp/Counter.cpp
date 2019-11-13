#include "Counter.h"

namespace frc
{
    Counter::Counter(Mode mode=kTwoPulse)
    {
        mode_ = mode ;
        channel_ = -1 ;
    }


    Counter::Counter(int channel)
    {
    }

    Counter::~Counter()
    {

    }

    void Counter::SetSemiPeriodMode(bool highSemiPeriod)
    {        
    }

    int Counter::GetPeriod()
    {

    }
}
