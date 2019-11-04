#pragma once

#include "SimulatedObject.h"

namespace frc
{
    class AnalogInput : public xero::sim::SimulatedObject {
    public:
        AnalogInput(int index) {
            index_ = index ;
        }

        int SimulatorGetChannel() const {
            return index_ ;
        }

        void SimulatorSetVoltage(double v) {
            voltage_ = v ;
        }

        double GetVoltage() {
            return voltage_ ;
        }

    private:
        int index_ ;
        double voltage_ ;
    } ;
}