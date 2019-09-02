#include "oi/GoPiGo3OIDevice.h"
#include "GoPiGo3Xero.h"
#include "gopigo3sub/GoPiGo3Subsystem.h"
#include "oi/GoPiGo3OISubsystem.h"
#include <cmath>
#include <iostream>

using namespace xero::base;

namespace xero
{
namespace gopigo3
{

GoPiGo3OIDevice::GoPiGo3OIDevice(GoPiGo3OISubsystem &sub, int index) : OIDevice(sub, index)
{
    initialize();
}

GoPiGo3OIDevice::~GoPiGo3OIDevice()
{
}

void GoPiGo3OIDevice::initialize()
{
    std::vector<double> mapping = {-0.9, -0.75, -0.5, -0.25, 0, 0.2, 0.4, 0.6, 0.8, 1.0};
    automode_ = mapAxisScale(6, mapping);

    angle_ = std::numeric_limits<double>::max();
}

int GoPiGo3OIDevice::getAutoModeSelector()
{
    int res = getValue(automode_) ;
    return res;
}

void GoPiGo3OIDevice::generateActions(xero::base::ActionSequence &seq) {

    frc::DriverStation &ds = frc::DriverStation::GetInstance();
    double axisv = ds.GetStickAxis(getIndex(), 0);
    double desired = (axisv + 1.0) * 90.0;

    GoPiGo3Xero &xerorobot = dynamic_cast<GoPiGo3Xero &>(getSubsystem().getRobot());
    auto sub = xerorobot.getGoPiGoSubsystem();
}

} // namespace bunny2018
} // namespace xero
