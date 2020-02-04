#pragma once

#include <optional>

#include <frc/DigitalInput.h>
#include <SettingsParser.h>
#include <Subsystem.h>
#include <motors/MotorController.h>

namespace xero {
    namespace droid {
        class Conveyor : public xero::base::Subsystem {
            friend class ConveyorAction;
            friend class ConveyorStopAction;
            friend class ConveyorOnAction ;
        public:
            Conveyor(xero::base::Subsystem *parent);
            virtual ~Conveyor() {}

            void postHWInit() override;
            bool canAcceptAction(xero::base::ActionPtr) override;
            bool canAcceptDefaultAction(xero::base::ActionPtr) override;

            int getBallCount() { return ballCount_; }

            static const int MAX_BALLS = 1;
            bool isEmpty() { return ballCount_ == 0; }
            bool isFull() { return ballCount_ == MAX_BALLS; }

            // A reference to a motor state defined in the config file.
            enum class MotorState {
                Stopped,
                MoveTowardsShooter,
                MoveTowardsIntake,
            };

            typedef std::shared_ptr<frc::DigitalInput> SensorPtr;

            enum class Sensor {
                A,  // The sensor closest to the intake.
                B,
                C   // The sensor closest to the shooter.

                //                    (shooter)
                //                      |   |
                //                      |   < C
                //                      |   |
                //          *-----------/   |
                // (intake) *              /
                //          *-^---^------ /
                //            A   B
            };
            SensorPtr getSensor(Sensor sensor) { return sensors_[static_cast<int>(sensor)]; }
            bool readSensor(Sensor sensor) { return getSensor(sensor)->Get(); }

        private:
            /// Runs the motors in the specified direction, or stops them if direction is Stopped.
            /// \param MotorState The direction to run the motors.
            /// \param speedConfig The name of the speed at which the motors should be run.
            ///     This is used to look up the params file at conveyor:speed:<speedID>.
            void setMotors(MotorState state);
            
        private:

            static SensorPtr createSensor(int channel);
            static SensorPtr createSensor(xero::misc::SettingsParser &settings, std::string configName);

        private:
            int ballCount_ = 0;
            std::array<SensorPtr, 3> sensors_;

            xero::base::MotorPtr intakeMotor_;
            xero::base::MotorPtr shooterMotor_;
            std::vector<std::pair<double, double>> motorStates_;
        };
    }
}
