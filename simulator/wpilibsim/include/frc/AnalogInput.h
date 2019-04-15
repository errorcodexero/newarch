#pragma once

namespace frc
{
    class AnalogInput {
    public:
        AnalogInput(int index) {
            index_ = index ;
        }

    private:
        int index_ ;
    } ;
}