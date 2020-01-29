#pragma once

#include <optional>

#include <frc/DigitalInput.h>
#include <SettingsParser.h>
#include <singlemotorsubsystem/SingleMotorSubsystem.h>

namespace xero {
    namespace droid {
        class Conveyor : public xero::base::SingleMotorSubsystem {
            friend class ConveyorAction;
        public:
            Conveyor(xero::base::Subsystem *parent);
            virtual ~Conveyor() {}

            void postHWInit() override;

            int getBallCount() { return ballCount_; }

            static const int MAX_BALLS = 5;

            enum class Direction {
                TowardsShooter,
                TowardsIntake,
            };

            typedef std::shared_ptr<frc::DigitalInput> SensorPtr;

            enum class Sensor {
                A,  // The sensor closest to the intake.
                B,
                C   // The sensor closest to the shooter.
            };
            SensorPtr getSensor(Sensor sensor) { return sensors_[static_cast<int>(sensor)]; }
            bool readSensor(Sensor sensor) { return getSensor(sensor)->Get(); }

        private:
            // Runs the motors in the specified direction, or stops them if direction is null.
            void setMotor(std::optional<Direction> direction);

            static SensorPtr createSensor(int channel);
            static SensorPtr createSensor(xero::misc::SettingsParser &settings, std::string configName);

        private:
            int ballCount_ = 0;
            std::array<SensorPtr, 3> sensors_;
        };
    }
}