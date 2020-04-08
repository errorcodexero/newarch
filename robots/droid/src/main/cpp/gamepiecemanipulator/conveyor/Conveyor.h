#pragma once

#include <optional>

#include <frc/DigitalInput.h>
#include <SettingsParser.h>
#include <Subsystem.h>
#include <motors/MotorController.h>

namespace xero {
    namespace droid {
        class Conveyor : public xero::base::Subsystem {
            friend class ConveyorStateAction;
            friend class ConveyorStopAction;
            friend class ConveyorOnAction ;
            friend class ConveyorEjectAction;
            friend class ConveyorSetBallAction ;
            
        public:
            Conveyor(xero::base::Subsystem *parent);
            virtual ~Conveyor() {}

            void postHWInit() override;
            bool canAcceptAction(xero::base::ActionPtr) override;
            bool canAcceptDefaultAction(xero::base::ActionPtr) override;

            void computeMyState() override;

            int getBallCount() { return ballCount_; }

            static const int MAX_BALLS;
            bool isEmpty() { return ballCount_ == 0; }
            bool isFull() { return ballCount_ == MAX_BALLS; }

            bool isStagedForCollect() { return stagedForCollect_; }
            bool isStagedForFire() { return stagedForFire_; }
            bool isCollecting() { return collecting_; }

            // A reference to a motor state defined in the config file.
            enum class MotorState {
                Stopped,
                MoveTowardsShooter,
                MoveTowardsIntake,
                MoveCollectMotorOnly,
                ReverseCollectMotorOnly,
            };

            typedef std::shared_ptr<frc::DigitalInput> SensorPtr;

            enum class Sensor {
                A,  // The sensor closest to the intake.
                B,

                C,
                D   // The sensor closest to the shooter.

                //                    (shooter)
                //                      |   |
                //                      |   < D
                //                      |   |
                //          *-----------/   |
                // (intake) *              /
                //          *-^---^------^/
                //            A   B      C
            };
            SensorPtr getSensor(Sensor sensor) { return sensors_[static_cast<int>(sensor)].first; }
            bool readSensor(Sensor sensor) { return sensors_[static_cast<int>(sensor)].second; }

            bool getIntegerProperty(const std::string &name, int &value) override ;

            bool getBooleanProperty(const std::string &name, bool &value) override ;

        protected:
            /// Runs the motors in the specified direction, or stops them if direction is Stopped.
            /// \param MotorState The direction to run the motors.
            /// \param speedConfig The name of the speed at which the motors should be run.
            ///     This is used to look up the params file at conveyor:speed:<speedID>.
            void setMotors(MotorState state);

            void setStagedForCollect(bool staged);
            void setStagedForFire(bool staged);
            void setCollecting(bool collecting);
            
        private:

            static SensorPtr createSensor(int channel);
            static SensorPtr createSensor(xero::misc::SettingsParser &settings, std::string configName);

            void setBallCount(int n) {
                ballCount_ = n ;
            }

        private:
            int ballCount_ ;
            std::array<std::pair<SensorPtr, bool>, 4> sensors_;
            static std::map<Sensor, std::string> sensorNames_;

            xero::base::MotorPtr intakeMotor_;
            xero::base::MotorPtr shooterMotor_;
            std::vector<std::pair<double, double>> motorStates_;

            bool stagedForCollect_;
            bool stagedForFire_;
            bool collecting_;
        };
    }
}
