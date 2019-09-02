#pragma once

#include <oi/OIDevice.h>
#include <Action.h>

namespace xero
{
namespace gopigo3
{
class GoPiGo3OISubsystem;

class GoPiGo3OIDevice : public xero::base::OIDevice
{
  public:
    GoPiGo3OIDevice(GoPiGo3OISubsystem &sub, int index);
    virtual ~GoPiGo3OIDevice();

    virtual int getAutoModeSelector();
    virtual void generateActions(xero::base::ActionSequence &seq);

  private:
    void initialize();

  private:
    size_t automode_;
    size_t servo_ ;
    double angle_;
};
} // namespace gopigo3
} // namespace xero