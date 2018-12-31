#pragma once

#include <functional>

namespace frc
{
    class SendableBuilder
    {
    public:
        virtual void SetSmartDashboardType(const std::string &type) {           
        }

        virtual void AddDoubleProperty(const std::string &name, std::function<double()> getter, std::function<void(double)> setter) {
        }
    } ;
}