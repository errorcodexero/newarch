#include "GoPiGo3OIDevice.h"
#include "GoPiGo3OISubsystem.h"
using namespace xero::base;

namespace xero
{
namespace gopigo
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
}

int GoPiGo3OIDevice::getAutoModeSelector()
{
    int res = getValue(automode_) ;
    return res;
}

void GoPiGo3OIDevice::generateActions(xero::base::ActionSequence &seq) {
}

} // namespace bunny2018
} // namespace xero
