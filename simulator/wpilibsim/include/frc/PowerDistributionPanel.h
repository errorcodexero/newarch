#pragma once

#include "SimulatedObject.h"
#include <array>

namespace frc {
    class PowerDistributionPanel  : public xero::sim::SimulatedObject
    {
    public:
        PowerDistributionPanel(int index) ;
        
        virtual ~PowerDistributionPanel() ;

        double GetCurrent(int index) ;

        void SimulatorSetCurrent(int index, double value) ;

    private:
        std::array<double, 16> currents_ ;
    } ;
}
