#pragma once

namespace frc
{
    class AnalogOutput
    {
    public:
        AnalogOutput(int channel);
        virtual ~AnalogOutput();

        void SetVoltage(double v);
        double GetVoltage(double v) const {
            return voltage_;
        }

        int GetChannel() const {
            return channel_;
        }

      private:
        int channel_;
        double voltage_;
    };
}