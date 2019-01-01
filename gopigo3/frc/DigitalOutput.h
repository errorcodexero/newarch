#pragma once

namespace frc
{
    class DigitalOutput
    {
    public:
        DigitalOutput(int channel);
        virtual ~DigitalOutput();

        void Set(bool value);
        bool Get() const {
            return value_;
        }
        int GetChannel() const {
            return channel_;
        }

    private:
        bool value_;
        int channel_;
    };
}