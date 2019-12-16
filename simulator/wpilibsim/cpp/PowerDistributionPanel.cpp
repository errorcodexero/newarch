#include <frc/PowerDistributionPanel.h>
#include <frc/RobotSimBase.h>
#include <cassert>

using namespace xero::sim;

namespace frc
{
    PowerDistributionPanel::PowerDistributionPanel(int index)
    {
        assert(index == 0) ;

        RobotSimBase &sim = RobotSimBase::getRobotSimulator();
        sim.connect(this);
    }

    PowerDistributionPanel::~PowerDistributionPanel()
    {
        RobotSimBase &sim = RobotSimBase::getRobotSimulator();
        sim.disconnect(this);
    }

    double PowerDistributionPanel::GetCurrent(int n)
    {
        assert(n < static_cast<int>(currents_.size())) ;
        std::lock_guard<std::mutex> lock(getLockMutex());
        return currents_[n];
    }

    void PowerDistributionPanel::SimulatorSetCurrent(int n, double value) {
        assert(n < static_cast<int>(currents_.size())) ;    
        std::lock_guard<std::mutex> lock(getLockMutex());
        currents_[n] = value ;
    }
} // namespace frc
