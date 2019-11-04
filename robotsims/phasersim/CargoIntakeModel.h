#pragma once

#include <frc/SubsystemModel.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class CargoIntakeModel : public SubsystemModel {
            public:
                CargoIntakeModel(xero::sim::RobotSimBase &simbase) ;

                virtual ~CargoIntakeModel() ;

                virtual bool processEvent(const std::string &name, int value) ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj);

                virtual void addDevice(ctre::phoenix::motorcontrol::can::VictorSPX *motor);      
                virtual void addDevice(frc::Solenoid *solenoid) ;

                /// \brief Add a digital input to the model.
                /// \param input the digital input to add
                virtual void addDevice(frc::DigitalInput *input) ;                

                bool isDeployed() const {
                    return deploy_state_ ;
                }

                double getPower() const {
                    return power_ ;
                }

                virtual void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                frc::Solenoid *deploy_solenoid_ ;
                frc::DigitalInput *cargo_sensor_ ;
                ctre::phoenix::motorcontrol::can::VictorSPX * motor_ ;                

                int deploy_channel_ ;
                int motor_channel_ ;

                double power_ ;
                bool deploy_state_ ;

                int cargo_sensor_channel_ ;
                bool has_cargo_ ;
            } ;
        }
    }
}