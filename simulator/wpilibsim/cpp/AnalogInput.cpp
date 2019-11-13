#include <frc/AnalogInput.h>
#include <frc/RobotSimBase.h>

using namespace xero::sim ;

namespace frc
{
    AnalogInput::AnalogInput(int index) 
    {
        index_ = index ;
        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.connect(this) ;            
    }

    AnalogInput::~AnalogInput() 
    {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator() ;
        sim.disconnect(this) ;             
    }

    int AnalogInput::SimulatorGetChannel() const {
        return index_ ;
    }

    void AnalogInput::SimulatorSetVoltage(double v) {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;            
        voltage_ = v ;
    }

    double AnalogInput::GetVoltage() {
        std::lock_guard<std::mutex> lock(getLockMutex()) ;            
        return voltage_ ;
    }
}