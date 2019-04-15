#pragma once

namespace frc
{
    class AnalogInput {
    public:
        AnalogInput(int index) {
            index_ = index ;
        }

        double GetVoltage() {
            return 0.0 ;
        }

    private:
        int index_ ;
    } ;
}