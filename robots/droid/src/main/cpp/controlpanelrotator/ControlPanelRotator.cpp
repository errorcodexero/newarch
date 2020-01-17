#include "controlpanelrotator/ControlPanelRotator.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        ControlPanelRotator::ControlPanelRotator(Subsystem *parent):
          MotorEncoderSubsystem(parent, "controlpanelrotator", MSG_GROUP_CONTROL_PANEL_ROTATOR) {

        }

        void ControlPanelRotator::computeState() {
          auto newColor = sampleSensor();
          hasNewData_ = newColor.has_value();
          if (newColor) lastColor_ = *newColor;
        }

        ControlPanelRotator::Color ControlPanelRotator::colorAfter(Color color)  {
          assert(color != Color::COUNT);
          color = static_cast<Color>(static_cast<int>(color) + 1);
          if (color == Color::COUNT) return static_cast<Color>(0);
          else return color;
        }

        int ControlPanelRotator::distanceToColor(Color a, Color b) {
          int first = static_cast<int>(a);
          int second = static_cast<int>(b);

          if (first <= second) return second - first;
          // Handle color wheel wraparound
          else return static_cast<int>(Color::COUNT) - (first - second);
        }

        std::optional<ControlPanelRotator::Color> ControlPanelRotator::sampleSensor() {
          return std::nullopt; // TODO
        }
    }
}