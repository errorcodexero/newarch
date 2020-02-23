#include "controlpanelrotator/ControlPanelRotator.h"
#include "droidids.h"

using namespace xero::misc;
using namespace xero::base;

namespace xero {
    namespace droid {
        ControlPanelRotator::ControlPanelRotator(Subsystem *parent):
          MotorEncoderSubsystem(parent, "controlpanelrotator", MSG_GROUP_CONTROL_PANEL_ROTATOR) {
              auto &settings = getRobot().getSettingsParser();
              servo_ = std::make_shared<frc::Servo>(settings.getDouble("hw:controlpanelrotator:servo"));
              armUp_ = settings.getDouble("controlpanelrotator:arm:up");
              armDown_ = settings.getDouble("controlpanelrotator:arm:down");
        }

        void ControlPanelRotator::init(LoopType ltype) {
            getArmServo()->Set(armDown_);
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
            // TODO: actually sample the sensor
            // We can return null if we somehow detect we can't get a good reading
            float r = 0, g = 0, b = 0;

            // Find hue
            // https://en.wikipedia.org/wiki/HSL_and_HSV#From_RGB
            float hue;

            float max = std::max(r, std::max(g, b));
            float min = std::min(r, std::min(g, b));

            if (max == 0) // r = g = b = 0
                hue = 0;
            else if (max == r)
                hue = 60*(0 + (g-b)/(max-min));
            else if (max == g)
                hue = 60*(2 + (b-r)/(max-min));
            else if (max == b)
                hue = 60*(4 + (r-g)/(max-min));
            if (hue < 0) hue += 360;
            
            Color result;
            if      (hue < 75)                  result = Color::Red;
            else if (hue > 115 && hue < 140)    result = Color::Green;
            else if (hue > 140)                 result = Color::Blue;
            else if (hue > 80 && hue < 110)     result = Color::Yellow;
            else                                return std::nullopt;

            // Offset by 2 to compensate for the sensor position
            result = static_cast<Color>((static_cast<int>(result) + 2) % static_cast<int>(Color::COUNT));

            return result;
        }
    }
}