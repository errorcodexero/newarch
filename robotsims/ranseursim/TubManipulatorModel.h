#pragma once

#include <frc/SubsystemModel.h>
#include <frc/AnalogInput.h>
#include <frc/Solenoid.h>
#include <ctre/Phoenix.h>
#include <frc/Solenoid.h>

namespace xero  {
    namespace sim {
        namespace ranseur {

            class TubManipulatorModel : public SubsystemModel
            {
            public:
                TubManipulatorModel(RobotSimBase &sim) ;
                virtual ~TubManipulatorModel() ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                /// \brief process a simulator event
                virtual bool processEvent(const std::string &name, int value) ;

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj);

                /// \brief Add an analog input to the model.
                /// \param input the digital input to add
                virtual void addDevice(frc::AnalogInput *input) ;

                /// \brief Add an digital input to the model.
                /// \param input the digital input to add
                virtual void addDevice(frc::DigitalInput *input) ;                

                /// \brief Add a solenoid to the model.
                /// \param input the digital input to add
                virtual void addDevice(frc::Solenoid *input) ;

                /// \brief Process when VictorSPX motor controllers are added to the robot
                /// \param motor the VictorSPX motor controller being added
                virtual void addDevice(ctre::phoenix::motorcontrol::can::VictorSPX *motor);   

                void generateDisplayInformation(std::list<std::string> &lines) ;

                bool hasTub() {
                    return !has_tub_inv_ ;
                }

                void hasTub(bool b) {
                    has_tub_inv_ = !b ;
                }

            private:
                void setArmEncoder() ;
                void setWristEncoder() ;
                void setTubSensor() ;

            private:
                static constexpr double ArmMinimumAngle = 00.0 ;
                static constexpr double ArmMaximumAngle = 90.0 ;
                static constexpr double ArmDegreesPerVoltPerSecond = -(ArmMaximumAngle - ArmMinimumAngle) / 1.0 ;
                static constexpr double WristMinAngle = 0.0 ;
                static constexpr double WristMaxAngle = 180.0 ;
                static constexpr double WristDegreesPerVoltPerSecond = (WristMaxAngle - WristMinAngle) / 1.0 ;

            private:
                int arm_motor_channel_ ;
                int wrist_motor_channel_ ;
                int arm_encoder_channel_ ;
                int wrist_encoder_channel_ ;
                int collector_solenoid_number_ ;
                int collector_tub_sensor_channel_ ;
                int collector_motor_channel_ ;

                frc::DigitalInput *tub_sensor_ ;
                frc::AnalogInput *arm_encoder_ ;
                frc::AnalogInput *wrist_encoder_ ;

                double wrist_degrees_per_volt_per_sec_ ;
                double arm_degrees_per_volt_per_sec_ ;

                double arm_motor_power_ ;
                double wrist_motor_power_ ;
                double collector_motor_power_ ;
                double arm_angle_ ;
                double wrist_angle_ ;
                bool collector_sensor_value_ ;
                bool collector_state_ ;

                bool has_tub_inv_ ;

            } ;
        }
    }
}
