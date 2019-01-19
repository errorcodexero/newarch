#pragma once

#include <frc/SubsystemModel.h>

namespace xero {
    namespace sim {
        namespace phaser {
            class HatchIntakeModel : public SubsystemModel {
            public:
                HatchIntakeModel(xero::sim::RobotSimBase &simbase) ;

                virtual ~HatchIntakeModel() ;

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
                virtual void addSolenoid(frc::Solenoid *solenoid) ;

                bool hasHatch() const  {
                    return has_hatch_ ;
                }

                bool isDeployed() const {
                    return deploy_state_ ;
                }

                double getPower() const {
                    return power_ ;
                }

                virtual void generateDisplayInformation(std::list<std::string> &lines) ;

            private:
                frc::Solenoid *deploy_solenoid_ ;
                ctre::phoenix::motorcontrol::can::TalonSRX * motor_ ;                
                frc::DigitalInput *hatch_sensor_ ;

                int deploy_channel_ ;
                int motor_channel_ ;
                int hatch_sensor_channel_ ;

                double power_ ;
                bool deploy_state_ ;
                bool has_hatch_ ;
            } ;
        }
    }
}