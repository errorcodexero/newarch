#pragma once

#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <optional>

namespace xero {
    namespace droid {
        class ControlPanelRotator : public xero::base::MotorEncoderSubsystem {
        public:
            enum class Color {
                Blue = 0,
                Green,
                Red,
                Yellow,

                COUNT
            };

            ControlPanelRotator(xero::base::Subsystem *parent);
            virtual ~ControlPanelRotator() {}

            void computeState() override;

            // Returns true iff we received a valid color sample from the sensor this tick.
            bool hasNewData() { return hasNewData_; }

            // Returns the last color detected by the sensor.
            Color getLastColor() { return lastColor_; }

            // Returns the color after the given color
            static Color colorAfter(Color color);

            // Returns the number of color wedges from `first` to `second`, in the increasing direction.
            // For example, distanceToColor(Color::Red, Color::Green) == 3.
            static int distanceToColor(Color first, Color second);

            static const int COLORS_PER_REVOLUTION = static_cast<int>(Color::COUNT) * 2;

        private:
            std::optional<Color> sampleSensor();

            bool hasNewData_;
            Color lastColor_ = Color::Blue;
        };
    }
}