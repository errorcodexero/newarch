#pragma once

#include <motorencodersubsystem/MotorEncoderSubsystem.h>
#include <optional>
#include <frc/Servo.h>

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

            void init(xero::base::LoopType ltype) override;
            void computeState() override;
            
            /// Returns a pointer to the servo which deploys the arm.
            std::shared_ptr<frc::Servo> getArmServo() { return servo_; }

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

            double getArmUpPosition();
            double getArmDownPosition();

        private:
            std::optional<Color> sampleSensor();
            std::shared_ptr<frc::Servo> servo_;

            double armUp_;
            double armDown_;

            bool hasNewData_;
            Color lastColor_ = Color::Blue;
        };
    }
}