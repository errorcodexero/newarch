#include "frc/Counter.h"
#include "frc/RobotSimBase.h"

using namespace xero::sim ;

namespace frc
{
    Counter::Counter(Mode mode)
    {
        mode_ = mode ;
        channel_ = -1 ;
        high_ = true ;

        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;          
    }

    Counter::Counter(int channel)
    {
        mode_ = Mode::kTwoPulse ;
        channel_ = channel ;
        high_ = true ;

        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;           
    }

    Counter::~Counter()
    {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.disconnect(this) ; 
    }

    void Counter::SetSemiPeriodMode(bool highSemiPeriod)
    {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;            
        mode_ = Mode::kPulseLength ;   
    }

    void Counter::SimulatorSetPeriod(int period)
    {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;    
        period_ = period ;        
    }

    int Counter::GetPeriod()
    {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;    
        return period_ ;
    }
}
