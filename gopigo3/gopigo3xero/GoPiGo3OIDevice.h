#pragma once

#include <OIDevice.h>
#include <Action.h>

namespace xero
{
namespace gopigo
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
};
} // namespace gopigo
} // namespace xero