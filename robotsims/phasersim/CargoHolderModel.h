#pragma once

#include <frc/SubsystemModel.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class CargoHolderModel : public SubsystemModel {
            public:
                CargoHolderModel(xero::sim::RobotSimBase &simbase) ;

                virtual ~CargoHolderModel() ;

                /// \brief Convert the model to a string.
                /// \returns details about the lifter's state
                virtual std::string toString() ;

                /// \brief Run the model.
                /// \param dt the time difference since the last code loop
                virtual void run(double dt);

                /// \brief Update the state of the model.  Called when one of its inputs changes.
                /// \param obj the input object which changed state
                virtual void inputChanged(SimulatedObject *obj);

                /// \brief Add a digital input to the model.
                /// \param input the digital input to add
                virtual void addDigitalInput(frc::DigitalInput *input) ;

                virtual void addTalonSRX(ctre::phoenix::motorcontrol::can::TalonSRX *motor);                

                bool hasCargo() const  {
                    return has_cargo_ ;
                }

                double getPower() const {
                    return power_ ;
                }

                virtual void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                ctre::phoenix::motorcontrol::can::TalonSRX * motor_ ;
                frc::DigitalInput *cargo_sensor_ ;

                int motor_channel_ ;
                int cargo_sensor_channel_ ;

                double power_ ;
                bool has_cargo_ ;
            } ;
        }
    }
}