#pragma once

#include <frc/AnalogOutput.h>
#include <memory>
namespace xero
{
    namespace gopigo
    {
        class SingleLED {
        public:
            SingleLED(int r, int g, int b) {
                r_ = std::make_shared<frc::AnalogOutput>(r);
                g_ = std::make_shared<frc::AnalogOutput>(g);
                b_ = std::make_shared<frc::AnalogOutput>(b);
            }
            ~SingleLED() {                
            }

            void setColor(double r, double g, double b) {
                r_->SetVoltage(r);
                g_->SetVoltage(g);
                b_->SetVoltage(b);
            }

        private:
            std::shared_ptr<frc::AnalogOutput> r_;
            std::shared_ptr<frc::AnalogOutput> g_;
            std::shared_ptr<frc::AnalogOutput> b_;
        };
    }
}