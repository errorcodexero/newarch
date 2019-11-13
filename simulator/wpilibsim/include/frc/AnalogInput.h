#pragma once

#include "SimulatedObject.h"

namespace frc
{
    class AnalogInput : public xero::sim::SimulatedObject {
    public:
        AnalogInput(int index) ;
        virtual ~AnalogInput() ;
        int SimulatorGetChannel() const  ;
        void SimulatorSetVoltage(double v)  ;
        double GetVoltage()  ;

    private:
        int index_ ;
        double voltage_ ;
    } ;
}