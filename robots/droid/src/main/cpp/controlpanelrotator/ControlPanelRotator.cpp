#include "controlpanelrotator/ControlPanelRotator.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        ControlPanelRotator::ControlPanelRotator(Subsystem *parent):
          MotorEncoderSubsystem(parent, "controlpanelrotator", MSG_GROUP_CONTROL_PANEL_ROTATOR) {

        }
    }
}