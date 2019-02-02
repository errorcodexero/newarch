#pragma once

#include <frc/SubsystemModel.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class CargoIntakeModel : public SubsystemModel {
            public:
                CargoIntakeModel(xero::sim::RobotSimBase &simbase) ;

                virtual ~CargoIntakeModel() ;

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

                bool isDeployed() const {
                    return deploy_state_ ;
                }

                double getPower() const {
                    return power_ ;
                }

                virtual void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                frc::Solenoid *deploy_solenoid_ ;
                ctre::phoenix::motorcontrol::can::VictorSPX * motor_ ;                

                int deploy_channel_ ;
                int motor_channel_ ;

                double power_ ;
                bool deploy_state_ ;
            } ;
        }
    }
}